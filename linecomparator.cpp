#include "linecomparator.h"
#include <QDebug>
#include <limits>

LineComparator::LineComparator(const QVector<QPointF> &line1,
                               const QVector<QPointF> &line2)
    : m_line1(line1), m_line2(line2) {}

QVector<QPointF> LineComparator::normalizeLine(const QVector<QPointF> &line) {
    if (line.isEmpty())
        return line;

    double minX = line[0].x(), minY = line[0].y();
    double maxX = minX, maxY = minY;

    for (const QPointF &p : line) {
        minX = std::min(minX, p.x());
        minY = std::min(minY, p.y());
        maxX = std::max(maxX, p.x());
        maxY = std::max(maxY, p.y());
    }

    QVector<QPointF> normalized;
    double rangeX = maxX - minX;
    double rangeY = maxY - minY;

    for (const QPointF &p : line) {
        double x = rangeX > 0 ? (p.x() - minX) / rangeX : 0;
        double y = rangeY > 0 ? (p.y() - minY) / rangeY : 0;
        normalized.append(QPointF(x, y));
    }

    return normalized;
}

double LineComparator::euclideanDistance(const QPointF &p1, const QPointF &p2) {
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();
    return std::sqrt(dx * dx + dy * dy);
}

double LineComparator::calculateHausdorff(const QVector<QPointF> &norm1,
                                          const QVector<QPointF> &norm2) {
    double maxDistance = 0.0;

    for (const QPointF &p1 : norm1) {
        double minDistance = std::numeric_limits<double>::max();
        for (const QPointF &p2 : norm2) {
            minDistance = std::min(minDistance, euclideanDistance(p1, p2));
        }
        maxDistance = std::max(maxDistance, minDistance);
    }

    return maxDistance;
}

double LineComparator::calculateDTW(const QVector<QPointF> &norm1,
                                    const QVector<QPointF> &norm2) {
    int n = norm1.size();
    int m = norm2.size();

    QVector<QVector<double>> dtw(
                n + 1, QVector<double>(m + 1, std::numeric_limits<double>::max()));
    dtw[0][0] = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            double cost = euclideanDistance(norm1[i - 1], norm2[j - 1]);
            dtw[i][j] =
                    cost + std::min({dtw[i - 1][j], dtw[i][j - 1], dtw[i - 1][j - 1]});
        }
    }

    return dtw[n][m] / std::max(n, m); // Нормализуем по длине
}

double LineComparator::calculateFrechet(const QVector<QPointF> &norm1,
                                        const QVector<QPointF> &norm2) {
    int n = norm1.size();
    int m = norm2.size();

    QVector<QVector<double>> ca(
                n, QVector<double>(m, std::numeric_limits<double>::max()));
    ca[0][0] = euclideanDistance(norm1[0], norm2[0]);
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            double d = euclideanDistance(norm1[i], norm2[j]);
            ca[i][j] =
                    std::max(d, std::min({ca[i - 1][j], ca[i][j - 1], ca[i - 1][j - 1]}));
        }
    }

    return ca[n - 1][m - 1];
}

LineComparator::Thresholds LineComparator::getMetrics() {
    auto norm1 = normalizeLine(m_line1);
    auto norm2 = normalizeLine(m_line2);

    Thresholds metrics;
    metrics.hausdorff = calculateHausdorff(norm1, norm2);
    metrics.dtw = calculateDTW(norm1, norm2);
    metrics.frechet = calculateFrechet(norm1, norm2);

    return metrics;
}

bool LineComparator::areSimilar(const Thresholds &thresholds) {
    Thresholds metrics = getMetrics();

    // Линии считаются похожими, если все метрики в пределах пороговых значений
    bool similar = metrics.hausdorff <= thresholds.hausdorff &&
            metrics.dtw <= thresholds.dtw &&
            metrics.frechet <= thresholds.frechet;

    return similar;
}
