#ifndef CALCTRACK_H
#define CALCTRACK_H
#include <QVector>

class CalcTrack
{
public:
    CalcTrack();
    void polosa();
private:
    double m_xn, m_yn;      // координаты начала маршрута
    double m_xk, m_yk;      // координаты конца маршрута
    bool m_isError;           // Если в данных где-то есть ошибка, для сообщения внешним программам
    double m_rLA;            // Радиус развотота ЛА
    double m_HLA;            // Высота полёта ЛА     - на данном этапе не используем
    double m_Lp, m_Sp;          // Длина и ширина полосы
    double m_Ls;              // Размер сетки (2 радиуса разворота)
    double jMax, jMin, iMax, iMin;      // предельные значения размеров полосы в относительных координатах
    const int ijMax = 10000;            // Константа <- предельная длина пути в заданную точку
    QVector<double> Mut;    // Массив значений длины маршрутов от ИПМ до каждой УТ
    double nPM;             // Количество ПМ
    QVector<double> jPM;    // массив относительных координат ПМ
    QVector<double> iPM;    // массив относительных координат ПМ
    QVector<double> xPM;    // Массив топоцентрических координат ПМ
    QVector<double> yPM;    // Массив топоцентрических координат ПМ
    QVector<double> Moz1;   // Массив координат и параметров опасных зон типа oz1
    QVector<double> Moz2;   // Массив координат и параметров опасных зон типа oz2
    QVector<double> Moz3;   // Массив координат и параметров опасных зон типа oz3
    int oz1Max;             // Количество опасных зон типа oz1
    int oz2Max;             // Количество опасных зон типа oz2
    int oz3Max;             // Количество опасных зон типа oz3

};

#endif // CALCTRACK_H
