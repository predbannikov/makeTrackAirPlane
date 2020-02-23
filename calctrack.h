#ifndef CALCTRACK_H
#define CALCTRACK_H
#include <QVector>
#include <QDebug>
#include <QObject>
#include "global.h"



class CalcTrack : public QObject
{
    Q_OBJECT
public:
    explicit CalcTrack(Data *a_data, QObject *parent = nullptr);
    ~CalcTrack();
    QVector<QPoint> *getPoints();
    void calcTrack(SIDE a_side, SEARCH_SIDE a_search_side);
private:
    int number_need_vert;
    double Ls;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    double sin_lastPath;
    int card_point = 0;
    void edgeAppend(Edge ed);
    void calcMinMax();
    int getMinimumLength();
    void setEdge();
    void setEdgeOnPath();
    bool checkEdge(Edge ed);
    QVector<double> calcPath(int number_vert);
    QVector<double> smoothingPath();

    double lengthPath = 0;

    QVector <UT> mat_line;
    QVector <QVector<UT> > mat;
    UT ut_p1_last;
    UT ut_p2_last;
    Data *data;
    SIDE side;
    SEARCH_SIDE search_side;
    double getAngleLastEdge(QPointF a, QPointF b);
//    double xn, yn, xk, yk;      // '-[Xn, Yn, Xk, Yk -координаты начала и конца маршрута
//    bool prErr;                 // '-[Признак наличия ошибок в составе входных данных
//                                // '-[для сообщения внешним программам
//    double Rr;                    // '-[Радиус разворота ЛА
//    double HLA;                 // '-[Высота полёта ЛА
//    double Lp, Sp;              // '-[Длина и ширина полосы
//    double Ls;                  // '-[Размер сетки (2 радиуса разворота)
//    double dXj, dYj, dXi, dYi;  // '-[dXj,dYj,dXi - переменные для использования в формулах преобразования координат
//    int jMax, jMin, iMax, iMin; // '-[jMax,jMin,iMax,iMin - предельные значения размеров полосы в относительных координатах
//    const int jiMax = 10000;    // '-[Константа <- предельная длина пути в заданную точку
//    QVector<int> Mut;           // '-[Массив значений длины маршрутов от ИПМ до каждой УТ
//    QVector <int> nPM;          // '-[Количество ПМ
//    QVector <int> jPM;          // '-[массив относительных координат ПМ
//    QVector<int> iPM;           // '-[массив относительных координат ПМ
//    QVector<double> xPM;        // '-[Массив топоцентрических координат ПМ
//    QVector<double> yPM;        // '-[Массив топоцентрических координат ПМ
//    QVector<double> Moz1;       // '-[Массив координат и параметров опасных зон типа oz1
//    int oz1Max;                 // '-[Количество опасных зон типа oz1
//    QVector<double> Moz2;       // '-[Массив координат и параметров опасных зон типа oz2
//    int oz2Max;                 // '-[Количество опасных зон типа oz2
//    QVector<double> Moz3;       // '-[Массив координат и параметров опасных зон типа oz3
//    int oz3Max;                 // '-[Количество опасных зон типа oz3

//    int wrapIntBs(double a_arg);

//    double geoCoordX(int n, int k);
//    double geoCoordY(int n, int k);
signals:
    void drawLine(UT p1, UT p2, int width, QColor col, QString str = "");
    void drawPoint(UT p1, int width, QColor col);
};

//class Dim(int 3);

#endif // CALCTRACK_H
