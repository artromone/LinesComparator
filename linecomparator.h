#ifndef LINECOMPARATOR_H
#define LINECOMPARATOR_H

#include <QPointF>
#include <QVector>

class LineComparator {
public:
    LineComparator(const QVector<QPointF> &line1, const QVector<QPointF> &line2);

    struct Thresholds {
        double hausdorff; // максимальное допустимое расстояние между точками
        double dtw;       // максимальное допустимое искажение формы
        double frechet;   // максимальное допустимое отклонение пути
    };

    // Основной метод для проверки схожести
    bool areSimilar(const Thresholds &thresholds = Thresholds{0.15, 0.2, 0.15});

    // Получить все метрики для анализа
    Thresholds getMetrics();

private:
    QVector<QPointF> m_line1;
    QVector<QPointF> m_line2;

    QVector<QPointF> normalizeLine(const QVector<QPointF> &line);
    double euclideanDistance(const QPointF &p1, const QPointF &p2);
    double calculateHausdorff(const QVector<QPointF> &norm1,
                              const QVector<QPointF> &norm2);
    double calculateDTW(const QVector<QPointF> &norm1,
                        const QVector<QPointF> &norm2);
    double calculateFrechet(const QVector<QPointF> &norm1,
                            const QVector<QPointF> &norm2);
};

#endif // LINECOMPARATOR_H
