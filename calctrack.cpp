#include "calctrack.h"
#include "math.h"

CalcTrack::CalcTrack(Data *a_data, SIDE a_side)
{
    data = a_data;
    side = a_side;
    calcMinMax();
    QPoint p1 = data->pointsFlightEnemy[0];
    QPoint p2 = data->pointsFlightEnemy[1];

    Ls = data->enemyAir.radiusTurn * 2. / 100.;  // '-[Ширина стороны квадрата сетки (два средних радиуса разворота)
    double Lp = sqrt(pow((p2.x() - p1.x()), 2) + pow((p2.y() - p1.y()), 2));   // '-[Lp - длина полосы
    double widthCell = ((p2.x() - p1.x()) / Lp) * Ls;
    double hightCell = ((p2.y() - p1.y()) / Lp) * Ls;
    qDebug() << hightCell << widthCell << Ls << Lp;
    if(Lp / Ls > 3) {

        points = new QVector<QPoint>;
        for(int i = 0; i < Lp / widthCell; i++) {
            QPoint point;
            QVector <UT> line_ut;
            for(int j = 0; j < Lp / widthCell; j++) {
                double x = p1.x() + widthCell * j + -hightCell * i;
                double y = p1.y() + widthCell * i + hightCell * j;
                if(!isPointInsideElypseRLS(data->zoneRLSEnemy, x, y)) {
                    qDebug() << "this point not inside elypse" ;
                    point.setX(x);
                    point.setY(y);
                    points->append(point);
                    line_ut.append({x, y, true});
                } else {
                    line_ut.append({x, y, false});
                }
            }
            mat.append(line_ut);
        }
        setEdge();
    }
    data->mat = &mat;
}


void CalcTrack::setEdge() {

    arr_points = new QVector <UT>;
    edge_arr = new QVector <Edge>;

//    int index = 0;
    int width = mat.first().size();
    for(int j = 0; j < mat.size(); j++) {
        for(int i = 0; i < mat.first().size(); i++) {
            arr_points->append(mat[j][i]);
            Edge ed;
            ed.cost = Ls;
            if(i - 1 >= 0 && mat[j][i].check) {            // Лево
                ed.a = i - 1 + j * width;
                ed.b = j * width + i;
                if(mat[j][i - 1].check)
                    if(!checkEdge(ed))
                        edge_arr->append(ed);
            }

            if(j - 1 >= 0 && mat[j][i].check) {            // Верх
                ed.a = (j - 1) * width + i;
                ed.b = j * width + i;
                if(mat[j - 1][i].check)
                    if(!checkEdge(ed))
                        edge_arr->append(ed);
            }
            if(i + 1 < mat.first().size() && mat[j][i].check) {         // Право
                ed.a = (width * j) + i;
                ed.b = (width * j) + i + 1;
                if(mat[j][i + 1].check)
                    if(!checkEdge(ed))
                        edge_arr->append(ed);
            }
            if(j + 1 < mat.size() && mat[j][i].check) {                // Низ
                ed.a = width * j + i;
                ed.b = (width * (j + 1)) + i;
                if(mat[j + 1][i].check)
                    if(!checkEdge(ed))
                        edge_arr->append(ed);
            }
//            index++;
        }
    }
    data->arr_points = arr_points;
    data->edge_arr = edge_arr;
    qDebug() << "stop";
}

bool CalcTrack::checkEdge(Edge ed)
{
    if(ed.b >= 576)
        qDebug() << "stop";
    if(ed.a >= 576)
        qDebug() << "stop";
    for(int i = 0; i < edge_arr->size(); i++)
        if(ed.a == edge_arr->at(i).a && ed.b == edge_arr->at(i).b)
            return true;
    return false;
}

void CalcTrack::polosa()
{
    int j, i;
}

QVector<QPoint> *CalcTrack::getPoints()
{
    return points;
}

void CalcTrack::calcMinMax()
{
    if(side == ENEMY) {
//        if(data->pointBorderEnemy.isEmpty())
//            return;
        min_x = data->pointBorderEnemy.at(0).x();
        min_y = data->pointBorderEnemy.at(0).y();
        max_x = data->pointBorderEnemy.at(0).x();
        max_y = data->pointBorderEnemy.at(0).y();
        for(int i = 0; i < data->pointBorderEnemy.size(); i++) {
            if(min_x > data->pointBorderEnemy.at(i).x())
                min_x = data->pointBorderEnemy.at(i).x();
            if(min_y > data->pointBorderEnemy.at(i).y())
                min_y = data->pointBorderEnemy.at(i).y();
            if(max_x < data->pointBorderEnemy.at(i).x())
                max_x = data->pointBorderEnemy.at(i).x();
            if(max_y < data->pointBorderEnemy.at(i).y())
                max_y = data->pointBorderEnemy.at(i).y();
        }
    } else {
        min_x = data->pointBorderEnemy.at(0).x();
        min_y = data->pointBorderEnemy.at(0).y();
        max_x = data->pointBorderEnemy.at(0).x();
        max_y = data->pointBorderEnemy.at(0).y();
        for(int i = 0; i < data->pointBorderRussia.size(); i++) {
            if(min_x > data->pointBorderRussia.at(i).x())
                min_x = data->pointBorderRussia.at(i).x();
            if(min_y > data->pointBorderRussia.at(i).y())
                min_y = data->pointBorderRussia.at(i).y();
            if(max_x < data->pointBorderRussia.at(i).x())
                max_x = data->pointBorderRussia.at(i).x();
            if(max_y < data->pointBorderRussia.at(i).y())
                max_y = data->pointBorderRussia.at(i).y();
        }
    }
}

//int CalcTrack::wrapIntBs(double a_arg)
//{
//    if(a_arg < 0)
//        return (int)a_arg + 1;
//    else
//        return (int)a_arg;
//}

//double CalcTrack::geoCoordX(int jm, int im)
//{
//    return -dXj * jm + dXi * im + xn;
//}

//double CalcTrack::geoCoordY(int jm, int im)
//{
//    return dYj * jm + dYi * im + yn;
//}
