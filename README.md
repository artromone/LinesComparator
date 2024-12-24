# LinesComparator
```cpp
#include "linecomparator.h"
#include <QCoreApplication>
#include <QDebug>
#include <QPointF>

int main() {
    // Создаем две линии
    QVector<QPointF> line1 = {QPointF(0, 0), QPointF(1, 1), QPointF(2, 2),
                              QPointF(3, 3), QPointF(6, 6)};

    QVector<QPointF> line2 = {QPointF(0.1, 0.2), QPointF(1.8, 1.7),
                              QPointF(3, 3),   QPointF(4.1, 4.1),
                              QPointF(5.8, 5.7),   QPointF(6, 6)};

    LineComparator comparator(line1, line2);

    // Используем стандартные пороговые значения
    if (comparator.areSimilar()) {
        qDebug() << "Линии считаются похожими";
    } else {
        qDebug() << "Линии различны";
    }

    // Получаем конкретные значения метрик
    auto metrics = comparator.getMetrics();
    qDebug() << "Расстояние Хаусдорфа:" << metrics.hausdorff;
    qDebug() << "DTW расстояние:" << metrics.dtw;
    qDebug() << "Расстояние Фреше:" << metrics.frechet;

    // Можно использовать свои пороговые значения
    LineComparator::Thresholds customThresholds;
    customThresholds.hausdorff = 0.3; // более мягкий порог
    customThresholds.dtw = 0.3;       // более мягкий порог
    customThresholds.frechet = 0.3;   // более мягкий порог

    if (comparator.areSimilar(customThresholds)) {
        qDebug() << "Линии похожи с учётом строгих порогов";
    } else {
        qDebug() << "Линии различны при строгом сравнении";
    }
}
```

```bash
Линии различны
Расстояние Хаусдорфа: 0.152349
DTW расстояние: 0.0947517
Расстояние Фреше: 0.247787
Линии похожи с учётом строгих порогов
```
