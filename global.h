#ifndef GLOBAL_H
#define GLOBAL_H
#include <QRandomGenerator>
#include <QDateTime>
#include <QPoint>
#include <QDebug>
#include <QImage>
#include <QVector>
#include "math.h"

enum SEARCH_SIDE {RIGHT, LEFT};
enum SIDE {ENEMY, RUSSIA};

struct Edge {
    int a, b;       // номера индексов рёбер
    double cost;    // расстояние между рёбрами
};
struct UT {
    double x;
    double y;
    bool check;
};

struct LengthPathIndex {
    int i;
    double length;
};

struct Airplane {
    QString model;
    int speed;
    int radiusTurn;
};

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
    QVector<ZoneRLS> zoneRLSRussia;
    QVector<ZoneRLS> zoneRLSEnemy;
    QVector<QPoint> pointsFlightRussia;
    QVector<QPoint> pointsFlightEnemy;
    QVector<QPoint> lastPointsFlight;
    QVector<Airplane> airplanes;
    Airplane lastEnemyAir;
    Airplane lastRussiaAir;
    Airplane enemyAir;
    Airplane russiaAir;
//    QVector <QVector<UT> > *mat;
    QVector <UT> *arr_points;               // Массив координат по индексам (номера точек)
    QVector <UT> *arr_points_check;         // Массив координат по для проверки
    QVector <QPoint> *meshPoints;
    QVector <int> *indexPath;
    QVector <int> *indexPathSmooth;
    QVector<UT> *path;
    QVector<UT> *pathSmoothing;
    QVector <Edge> *edge_arr;
    QVector <Edge> *checkEdge;
    QMap<int, QPoint> indexes;
    QMap<int, QPoint> indexes_smooth;
    QVector <QVector<UT> > *vec_points;
    QVector <QVector<Edge> > *vec_edge;
    QVector <QVector<Edge> > *vec_edge_check;
    QVector <QVector<UT> > *vec_pathSmooth;
    QVector <QVector<UT> > *vec_path;
    QVector <QVector<QPoint> > *vec_meshPoints;
    QVector <double> lengthPath;
};
// возвращает true если внутри элипса
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
