#include "calctrack.h"
#include <algorithm>
#include "math.h"

CalcTrack::CalcTrack(Data *a_data)
{
    data = a_data;
    calcMinMax();
    path = new QVector<int>;
    data->path = new QVector<UT>;
    data->pathSmoothing = new QVector<UT>;
    points = new QVector<QPoint>;
    arr_points = new QVector <UT>;
    edge_arr = new QVector <Edge>;
    pathEdge = new QVector<Edge>;
    data->arr_points_check = new QVector<UT>;
    SEARCH_SIDE search_side = RIGHT;

    QPoint p1 = data->pointsFlightEnemy[0];
    QPoint p2 = data->pointsFlightEnemy[1];

    Ls = data->enemyAir.radiusTurn * 2. / 100.;  // '-[Ширина стороны квадрата сетки (два средних радиуса разворота)
    double Lp = sqrt(pow((p2.x() - p1.x()), 2) + pow((p2.y() - p1.y()), 2));   // '-[Lp - длина полосы
    double widthCell = ((p2.x() - p1.x()) / Lp) * Ls;
    double hightCell = ((p2.y() - p1.y()) / Lp) * Ls;
    double angleCos = (p2.x() - p1.x()) / Lp;      // Косинус угла
    double angleSin = (p2.y() - p1.y()) / Lp;      // Синус угла
    qDebug() << hightCell << widthCell << Ls << Lp;
    if(Lp / Ls > 3) {
        int right_border = Lp / Ls;

        double appendix_g = Lp - (right_border * Ls);
        double appendix_width = appendix_g * angleCos;
        double appendix_hight = appendix_g * angleSin;
        for(int i = 0; i < right_border; i++) {
            QPoint point;
            QVector <UT> line_ut;
            for(int j = 0; j <= right_border; j++) {

                double x = p1.x() + widthCell * j + hightCell * i;
                double y = p1.y() + -widthCell * i + hightCell * j;
//                double x = p1.x() + widthCell * j + -hightCell * i;
//                double y = p1.y() + widthCell * i + hightCell * j;
//                x *= -1.;
//                y *= -1.;
                if(!isPointInsideElypseRLS(data->zoneRLSEnemy, x, y)) {
                    qDebug() << "this point not inside elypse" ;
                    point.setX(x);
                    point.setY(y);
                    points->append(point);
                    line_ut.append({x, y, true});
                } else {
                    line_ut.append({x, y, false});
                }
                qDebug() << Lp / widthCell << "i=" << i << " j=" << j;
                if(j == (right_border) && search_side == RIGHT)           // *******************  перед переходом на следующую строку
                {
                    if(i == 0) {
                        points->append(p2);
                        line_ut.append({(double)p2.x(), (double)p2.y(), true});
                    } else {
                        double x_appendix = x + appendix_width * (j + 1) + -appendix_hight * i;
                        double y_appendix = y + p1.y() + appendix_width * i + appendix_hight * (j + 1);
                        points->append(QPoint(x_appendix, y_appendix));
                        line_ut.append({x_appendix, y_appendix, false});
                    }
                }
            }
            if(i == (right_border - 1) )           // *******************  перед переходом на следующую строку
            {
//                if(i == 0) {
//                    points->append(p2);
//                    line_ut.append({(double)p2.x(), (double)p2.y(), true});
//                } else {
//                    points->append(p2);
                    line_ut.append({(double)p2.x(), (double)p2.y(), false});
//                }
            }
            mat.append(line_ut);
        }
        setEdge();
    }
    number_need_vert = mat.first().size();
    data->mat = &mat;
    data->arr_points = arr_points;
    data->edge_arr = edge_arr;
    calcPath(number_need_vert);
    setEdgeOnPath();        // <--
    smoothingPath();
}
QVector<double> CalcTrack::calcPath(int number_vert) {
    const int INF = 1000000000;
    int n, m, v;
    n = arr_points->size();
    m = edge_arr->size();
    QVector<double> d (n, INF);

    v = 0;
    d[v] = 0;
    QVector<int> *p = new QVector<int>(n, -1);

    for (;;) {
        bool any = false;
        for (int j=0; j<m; ++j)
            if (d[edge_arr->at(j).a] < INF)
                if (d[edge_arr->at(j).b] > d[edge_arr->at(j).a] + edge_arr->at(j).cost &&
                        !qFuzzyCompare(d[data->edge_arr->at(j).b], d[data->edge_arr->at(j).a] + data->edge_arr->at(j).cost)) {
                    d[edge_arr->at(j).b] = d[edge_arr->at(j).a] + edge_arr->at(j).cost;
                    p->replace(edge_arr->at(j).b, edge_arr->at(j).a);
                    any = true;
                }
        if(!any)
            break;
    }
    int t = number_need_vert - 1;
    if (d[t] == INF)
        qDebug() << "No path from " << v << " to " << t << ".";
    else {
        for (int cur=t; cur!=-1; cur=p->at(cur))
            path->push_back (cur);
        std::reverse (path->begin(), path->end());

        qDebug() << "Path from " << v << " to " << t << ": ";
        for (int i=0; i<path->size(); ++i)
            qDebug() << path->at(i) << ' ';
        for(int i = 0; i < path->size(); i++) {
            data->path->append(arr_points->at(path->at(i)));
        }
    }
    return d;
}

QVector<double> CalcTrack::smoothingPath()
{
    const int INF = 1000000000;
    int n, m, v = 0;
    n = data->path->size();
    m = data->checkEdge->size();
    QVector<double> d (n, INF);
    d[v] = 0;
    QVector<int> *p = new QVector<int>(n, -1);
    for (;;) {
        bool any = false;
        for (int j=0; j<m; ++j) {
            if (d[data->checkEdge->at(j).a] < INF) {
                if (d[data->checkEdge->at(j).b] > d[data->checkEdge->at(j).a] + data->checkEdge->at(j).cost &&
                        !qFuzzyCompare(d[data->checkEdge->at(j).b], d[data->checkEdge->at(j).a] + data->checkEdge->at(j).cost)) {
                    d[data->checkEdge->at(j).b] = d[data->checkEdge->at(j).a] + data->checkEdge->at(j).cost;
                    p->replace(data->checkEdge->at(j).b, data->checkEdge->at(j).a);
                    any = true;
                }
            }
        }
        if(!any)
            break;
    }
    int t = n - 1;

    if (d[t] == INF)
        qDebug() << "No path from " << v << " to " << t << ".";
    else {
        QVector<int> pathSmuthInt ;
        for (int cur=t; cur!=-1; cur=p->at(cur))
            pathSmuthInt.push_back (cur);
        std::reverse (pathSmuthInt.begin(), pathSmuthInt.end());

        qDebug() << "pathSmuthInt from " << v << " to " << t << ": ";
        for (int i=0; i<pathSmuthInt.size(); ++i)
            qDebug() << pathSmuthInt.at(i) << ' ';
        for(int i = 0; i < pathSmuthInt.size(); i++) {
            data->pathSmoothing->append(data->path->at(pathSmuthInt.at(i)));
        }
    }
    qDebug() << "smoothingPath::stop";
    return d;
}

void CalcTrack::setEdge() {


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

    int siz_graph = edge_arr->size();
    for(int i = 0; i < siz_graph; i++) {
        Edge ed = {edge_arr->at(i).b, edge_arr->at(i).a, edge_arr->at(i).cost};
        edge_arr->append(ed);
    }
//    number_need_vert = width;
//    data->arr_points = arr_points;
//    data->edge_arr = edge_arr;
//    calcPath(number_need_vert);
//    setEdgeOnPath();        // <--
//    smoothingPath();
    qDebug() << "setEdge::stop";
}

void CalcTrack::setEdgeOnPath()
{
    pathEdge->clear();
    int count_tick = 0;
    for(int i = 0; i < path->size() - 1; i++) {
        for(int j = i + 1; j < path->size(); j++) {

            QPointF p1 = {arr_points->at(path->at(i)).x, arr_points->at(path->at(i)).y};
            QPointF p2 = {arr_points->at(path->at(j)).x, arr_points->at(path->at(j)).y};

            Edge ed{i, j, sqrt(pow(p2.x() - p1.x(), 2.) + pow(p2.y() - p1.y(), 2.))};
            double widthCell = ((p2.x() - p1.x()) / ed.cost) * Ls;
            double hightCell = ((p2.y() - p1.y()) / ed.cost) * Ls;
            bool check = false;
            for(int k = 0; k < ed.cost / widthCell; k++) {      // Строим точки по пути от А до Б и проверяем в элипсе
                double x = p1.x() + widthCell * k;
                double y = p1.y() + hightCell * k;
                if(isPointInsideElypseRLS(data->zoneRLSEnemy, x, y)) {
                    check = true;
                    break;
//                    qDebug() << "inside" << i << j;
                }
                count_tick++;
            }
            if(check)
                break;
            else {
//                qDebug() << "ed =" << ed.a << ed.b << ed.cost;
                pathEdge->append(ed);
            }
        }
    }
//    qDebug() << count_tick << pathEdge->size();
//    qDebug() << "stop";
    data->checkEdge = pathEdge;
}

bool CalcTrack::checkEdge(Edge ed)
{
//    if(ed.b >= 576)
//        qDebug() << "stop";
//    if(ed.a >= 576)
//        qDebug() << "stop";
    for(int i = 0; i < edge_arr->size(); i++)
        if(ed.a == edge_arr->at(i).a && ed.b == edge_arr->at(i).b)
            return true;
    return false;
}

QVector<QPoint> *CalcTrack::getPoints()
{
    return points;
}

void CalcTrack::calcTrack()
{
    SIDE a_side;
}

void CalcTrack::calcMinMax()
{
    if(side == ENEMY) {
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


