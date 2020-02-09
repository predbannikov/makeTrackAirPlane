#ifndef GLOBAL_H
#define GLOBAL_H
#include <QRandomGenerator>
#include <QDateTime>
#include <QPoint>
#include <QDebug>
#include <QImage>
#include <QVector>
#include "math.h"


enum SIDE {ENEMY, RUSSIA};

struct Coord {
    int x = 0;
    int y = 0;
};

struct ZoneRLS {
    int x = 0;
    int y = 0;
    int radius = 1;
};

struct Data {
    QVector<QPoint> pointBorderRussia;
    QVector<QPoint> pointBorderEnemy;
    QImage image;
    QImage lastImage;
    QVector<ZoneRLS> lastZoneRLS;
    QVector<ZoneRLS> zoneRLS;
    QVector<QPoint> pointsFlight;
    QVector<QPoint> lastPointsFlight;
};

inline bool isPointInsideElypseRLS(QVector<ZoneRLS> a_vecRLS, int px, int py) {
    for(int i = 0; i < a_vecRLS.size(); i++) {
        if(pow(px - a_vecRLS.at(i).x, 2) +
                pow(py - a_vecRLS.at(i).y, 2) <=
                pow(a_vecRLS.at(i).radius, 2))
            return true;
        else
            continue;
    }
    return false;
}

inline bool isPointInsidePolygon (QVector<QPoint> p, int x, int y)
{
    int i1, i2, n, N, S, S1, S2, S3, flag;
    N = p.size();
    for (n=0; n<N; n++)
    {
        flag = 0;
        i1 = n < N-1 ? n + 1 : 0;
        while (flag == 0)
        {
            i2 = i1 + 1;
            if (i2 >= N)
                i2 = 0;
            if (i2 == (n < N-1 ? n + 1 : 0))
                break;
            S = abs (p[i1].x() * (p[i2].y() - p[n ].y()) +
                     p[i2].x() * (p[n ].y() - p[i1].y()) +
                     p[n].x()  * (p[i1].y() - p[i2].y()));
            S1 = abs (p[i1].x() * (p[i2].y() - y) +
                      p[i2].x() * (y       - p[i1].y()) +
                      x       * (p[i1].y() - p[i2].y()));
            S2 = abs (p[n ].x() * (p[i2].y() - y) +
                      p[i2].x() * (y        - p[n ].y()) +
                      x       * (p[n ].y() - p[i2].y()));
            S3 = abs (p[i1].x() * (p[n ].y() - y) +
                      p[n ].x() * (y       - p[i1].y()) +
                      x       * (p[i1].y() - p[n ].y()));
            if (S == S1 + S2 + S3)
            {
                flag = 1;
                break;
            }
            i1 = i1 + 1;
            if (i1 >= N)
                i1 = 0;
        }
        if (flag == 0)
            break;
    }
    return flag;
}

#endif // GLOBAL_H
