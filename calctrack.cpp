#include "calctrack.h"
#include <algorithm>
#include "math.h"
//#include "qmath.h"
#define     COOF_RAT    50

double VectorLength( const QPoint p)
{
    return ( sqrt( p.x() * p.x() + p.y() * p.y() ) );
}

float GetSin(UT ut1, UT ut2, UT ut3, UT ut4)
{
    QPoint a(ut2.x - ut1.x, ut2.y - ut1.y);
    QPoint b(ut4.x - ut3.x, ut4.y - ut3.y);

    const double lenA = VectorLength( a );
    const double lenB = VectorLength( b );
//    const double dot = a.x() * b.x() + a.y() * b.y();
    const double dot = a.x() * b.y() + a.y() * b.x();
    const double cosAB = dot / ( lenA * lenB );

    return ( sqrt( 1 - cosAB * cosAB ) );
}

CalcTrack::CalcTrack(Data *a_data, QObject *parent) : QObject(parent)
{
    data = a_data;
//    calcMinMax();
    data->indexPath = new QVector<int>;
    data->indexPathSmooth = new QVector<int>;
    data->path = new QVector<UT>;
    data->pathSmoothing = new QVector<UT>;
    data->meshPoints = new QVector <QPoint>;
    data->arr_points = new QVector<UT>;
    data->edge_arr = new QVector <Edge>;
    data->checkEdge = new QVector <Edge>;
    data->arr_points_check = new QVector<UT>;


}

CalcTrack::~CalcTrack()
{

}
QVector<double> CalcTrack::calcPath(int number_vert) {
    const int INF = 1000000000;
    int n, m, v;
    n = data->arr_points->size();
    m = data->edge_arr->size();
    QVector<double> d (n, INF);

    v = 0;
    d[v] = 0;
    QVector<int> *p = new QVector<int>(n, -1);

    for (;;) {
        bool any = false;
        for (int j=0; j<m; ++j)
            if (d[data->edge_arr->at(j).a] < INF)
                if (d[data->edge_arr->at(j).b] > d[data->edge_arr->at(j).a] + data->edge_arr->at(j).cost &&
                        !qFuzzyCompare(d[data->edge_arr->at(j).b], d[data->edge_arr->at(j).a] + data->edge_arr->at(j).cost)) {
                    d[data->edge_arr->at(j).b] = d[data->edge_arr->at(j).a] + data->edge_arr->at(j).cost;
                    p->replace(data->edge_arr->at(j).b, data->edge_arr->at(j).a);
                    any = true;
                }
        if(!any)
            break;
    }
    int t = number_need_vert - 1;
    if (d[t] == INF)
        qDebug() << "No path from " << v << " to " << t << ".";
    else {
//        QVector<int> *path = new QVector<int>;
        data->indexPath->clear();

        for (int cur=t; cur!=-1; cur=p->at(cur))
            data->indexPath->push_back (cur);
        std::reverse (data->indexPath->begin(), data->indexPath->end());

//        qDebug() << "Path from " << v << " to " << t << ": " << data->indexPath->size();
//        for (int i=0; i<path->size(); ++i)
//            qDebug() << path->at(i) << ' ';
        for(int i = 0; i < data->indexPath->size(); i++) {
            data->path->append(data->arr_points->at(data->indexPath->at(i)));
            data->indexes_smooth.insert(i, data->indexes.value(data->indexPath->at(i)));
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
        data->indexPathSmooth->clear();
        for (int cur=t; cur!=-1; cur=p->at(cur))
            data->indexPathSmooth->push_back (cur);
        std::reverse (data->indexPathSmooth->begin(), data->indexPathSmooth->end());
//        for(int i = 0; i < data->indexPathSmooth->size(); i++)
        lengthPath += d[data->indexPathSmooth->last()];
        data->lengthPath.append(lengthPath);
//        qDebug() << "pathSmuthInt from " << v << " to " << t << ": " << " length =" << lengthPath;
        lengthPath = 0;
//        for (int i=0; i<data->indexPathSmooth->size(); ++i)
//            qDebug() << data->indexPathSmooth->at(i) << ' ';
        for(int i = 0; i < data->indexPathSmooth->size(); i++) {
            data->pathSmoothing->append(data->path->at(data->indexPathSmooth->at(i)));
        }
    }
//    qDebug() << "smoothingPath::stop";
    return d;
}

double CalcTrack::getAngleLastEdge(QPointF a, QPointF b)
{
//    QPoint a(ut_p2_last.x - ut_p1_last.x, ut_p2_last.y - ut_p1_last.y);
//    QPoint b(p2.x - ut_p2_last.x, p2.y - ut_p2_last.y);

    double atan_a = atan2(a.x() * b.y() - b.x() * a.y(), a.x() * b.x() + a.y() * b.y());
    double grad = atan_a * 180. / M_PI;
    return grad;
}

void CalcTrack::setEdge() {

    // Если вектор первой сетки точек пуст то не проверять вектор
    // направления иначе проверять и измерять угол и чтоб было больше 90 град
    bool first_point = false;
//    double sin_path;

    if(!data->path_short.isEmpty() ) {
//        QVector<UT> lastPath = data->vec_pathSmooth->last();
        QVector<UT> lastPath = data->path_short.last().path;
        int size_path = lastPath.size();
        ut_p1_last = lastPath.at(size_path - 2);
        ut_p2_last = lastPath.at(size_path - 1);
        emit drawLine(lastPath.at(size_path - 2), lastPath.at(size_path - 1), 3, QColor(0, 255, 0), "");
        emit drawPoint(lastPath.at(size_path - 2), 7, QColor(0, 255, 0));
//        double x1 = lastPath.at(size_path - 2).x;
//        double y1 = lastPath.at(size_path - 2).y;
//        double x2 = lastPath.at(size_path - 1).x;
//        double y2 = lastPath.at(size_path - 1).y;
//        double g = sqrt(pow(x2 - x1, 2.) + pow(y2 - y1, 2.));
//        sin_path = (y2 - y1) / g;


//        qDebug() << "sin1 =" << asin(sin_path) * (180./M_PI);
////        qDebug() << M_PI << sin_path;           // 3.14159 0.35749
//        switch (search_side) {          // Уголы для обеих сторон
//        case LEFT: {
////            qDebug() << "sin 90 =" << sin(20.9462 * (M_PI/180.));             sin 90 = 0.357491
////            qDebug() << "sin 90 =" << asin(sin_path )* (180./M_PI);           sin 90 = 20.9462
////            double diff_grad = asin(sin_lastPath) * (180./M_PI) - asin(sin_path) * (180./M_PI);
////            if(diff_grad < 90.) {
////                qDebug() << "little grad rotate! grad =" << diff_grad ;
////            }
//            break;
//        }
//        case RIGHT:
////            qDebug() << "sin 45 =" << qDegreesToRadians(45.);
//            break;
//        }
        first_point = true;
    }

    int width = mat.first().size();
    for(int j = 0; j < mat.size(); j++) {
        for(int i = 0; i < mat.first().size(); i++) {
            data->arr_points->append(mat[j][i]);
            Edge ed;
            ed.cost = Ls;
            if(i - 1 >= 0 && mat[j][i].check) {            // Лево
                ed.a = i - 1 + j * width;
                ed.b = j * width + i;                    
                if(mat[j][i - 1].check)
                    if(!checkEdge(ed))
                        if(ed.a == 0 && first_point) {
                            UT p1 = mat[j][i];
                            UT p2 = mat[j][i - 1];
//                            emit drawLine(p1, p2, 3, QColor(255, 0, 0));
//                            emit drawPoint(p1, 10, QColor(255, 0, 0));

                            QPoint a(ut_p2_last.x - ut_p1_last.x, ut_p2_last.y - ut_p1_last.y);
                            QPoint b(p2.x - ut_p2_last.x, p2.y - ut_p2_last.y);

                            double atan_a = atan2(a.x() * b.y() - b.x() * a.y(), a.x() * b.x() + a.y() * b.y());
                            double grad = atan_a * 180. / M_PI;
                            if(grad < 0) {
                                grad += 180.;
                            } else {
                                grad = 180. - grad;
                            }
                            if(grad < 90.) {
                                qDebug() << "LEFT: little grad rotate! grad =" << grad ;
                                emit drawLine(p1, p2, 3, QColor(255, 0, 0), QString::number(grad, 'f', 0));
                                emit drawPoint(p1, 5, QColor(255, 0, 0));

                            } else {
                                emit drawPoint(p1, 15, QColor(1, 1, 1));
                                emit drawLine(ut_p1_last, ut_p2_last, 5, QColor(1, 1, 1));
                                emit drawLine(p1, p2, 3, QColor(0, 0, 0), QString::number(grad, 'f', 0));
                                qDebug() << "LEFT: good grad rotate! grad =" << grad ;
                                edgeAppend(ed);
                            }

                        } else {
                            edgeAppend(ed);
                        }
//                        edgeAppend(ed);
//                        edge_arr->append(ed);

            }

            if(j - 1 >= 0 && mat[j][i].check) {            // Верх
                ed.a = (j - 1) * width + i;
                ed.b = j * width + i;
                if(mat[j - 1][i].check) {
                    if(!checkEdge(ed)) {
                        if(ed.a == 0 && first_point) {
                            UT p1 = mat[j][i];
                            UT p2 = mat[j - 1][i];
//                            emit drawLine(p1, p2, 3, QColor(255, 0, 0));
//                            emit drawPoint(p1, 10, QColor(255, 0, 0));

                            QPoint a(ut_p2_last.x - ut_p1_last.x, ut_p2_last.y - ut_p1_last.y);
                            QPoint b(p2.x - ut_p2_last.x, p2.y - ut_p2_last.y);

                            double atan_a = atan2(a.x() * b.y() - b.x() * a.y(), a.x() * b.x() + a.y() * b.y());
                            double grad = atan_a * 180. / M_PI;
                            if(grad < 0) {
                                grad += 180.;
                            } else {
                                grad = 180. - grad;
                            }
                            if(grad < 90.) {
                                qDebug() << "UP: little grad rotate! grad =" << grad ;
                                emit drawLine(p1, p2, 3, QColor(255, 0, 0), QString::number(grad, 'f', 0));
                                emit drawPoint(p1, 5, QColor(255, 0, 0));
                            } else {
                                qDebug() << "UP: good grad rotate! grad =" << grad ;
                                edgeAppend(ed);
                            }

                        } else {
                            edgeAppend(ed);
                        }
                    }
                }
//                        edgeAppend(ed);
//                        edge_arr->append(ed);
            }
            if(i + 1 < mat.first().size() && mat[j][i].check) {         // Право
                ed.a = (width * j) + i;
                ed.b = (width * j) + i + 1;
                if(mat[j][i + 1].check)
                    if(!checkEdge(ed)) {
                        if(ed.a == 0 && first_point) {
                            UT p1 = mat[j][i];
                            UT p2 = mat[j][i + 1];
//                            emit drawLine(p1, p2, 3, QColor(255, 0, 0));
//                            emit drawPoint(p1, 10, QColor(255, 0, 0));

                            QPoint a(ut_p2_last.x - ut_p1_last.x, ut_p2_last.y - ut_p1_last.y);
                            QPoint b(p2.x - ut_p2_last.x, p2.y - ut_p2_last.y);

                            double atan_a = atan2(a.x() * b.y() - b.x() * a.y(), a.x() * b.x() + a.y() * b.y());
                            double grad = atan_a * 180. / M_PI;
                            if(grad < 0) {
                                grad += 180.;
                            } else {
                                grad = 180. - grad;
                            }
                            if(grad < 90.) {
                                qDebug() << "RIGHT: little grad rotate! grad =" << grad ;
//                                emit drawLine(p1, p2, 3, QColor(255, 0, 0), QString::number(grad, 'f', 0));
                                emit drawPoint(p1, 5, QColor(255, 0, 0));
                            } else {
                                qDebug() << "RIGHT: good grad rotate! grad =" << grad ;
                                edgeAppend(ed);
                            }
                        } else {
                            edgeAppend(ed);
                        }
                        //                        edge_arr->append(ed);
                    }
            }
            if(j + 1 < mat.size() && mat[j][i].check) {                // Низ
                ed.a = width * j + i;
                ed.b = (width * (j + 1)) + i;
                if(mat[j + 1][i].check)
                    if(!checkEdge(ed)) {
                        if(ed.a == 0 && first_point) {
                            UT p1 = mat[j][i];
                            UT p2 = mat[j + 1][i];
//                            emit drawLine(p1, p2, 3, QColor(255, 0, 0));
//                            emit drawPoint(p1, 10, QColor(255, 0, 0));

                            QPoint a(ut_p2_last.x - ut_p1_last.x, ut_p2_last.y - ut_p1_last.y);
                            QPoint b(p2.x - ut_p2_last.x, p2.y - ut_p2_last.y);


                            double atan_a = atan2(a.x() * b.y() - b.x() * a.y(), a.x() * b.x() + a.y() * b.y());
                            double grad = atan_a * 180. / M_PI;
                            if(grad < 0) {
                                grad += 180.;
                            } else {
                                grad = 180. - grad;
                            }
                            if(grad < 90.) {
                                emit drawLine(p1, p2, 3, QColor(255, 0, 0), QString::number(grad, 'f', 0));
                                emit drawPoint(p1, 5, QColor(255, 0, 0));
                                qDebug() << "BOTTOME little grad rotate! grad =" << grad ;
                            } else {
                                qDebug() << "BOTTOME good grad rotate! grad =" << grad ;
                                edgeAppend(ed);
                            }

                        } else {
                            edgeAppend(ed);
                        }
                    }
//                        edge_arr->append(ed);
            }
//            index++;
//            first_point = false;
        }
    }

    int siz_graph = data->edge_arr->size();
    for(int i = 0; i < siz_graph; i++) {
        Edge ed = {data->edge_arr->at(i).b, data->edge_arr->at(i).a, data->edge_arr->at(i).cost};
        edgeAppend(ed);
//        edge_arr->append(ed);
    }

//    qDebug() << "setEdge::stop";
}

void CalcTrack::setEdgeOnPath()
{

//    /*
    QPointF a;
    if(!data->path_short.isEmpty()) {
        QVector<UT> lastPath = data->path_short.last().path;
        int size_path = lastPath.size();
        emit drawLine(lastPath.at(size_path - 2), lastPath.at(size_path - 1), 3, QColor(0, 100, 0), "");
        emit drawPoint(lastPath.at(size_path - 2), 15, QColor(255, 0, 0));
        a = QPoint(lastPath.at(size_path - 1).x - lastPath.at(size_path - 2).x,
                          lastPath.at(size_path - 1).y - lastPath.at(size_path - 2).y);
    }
    //    */

    data->checkEdge->clear();
    int count_tick = 0;
    QPointF p1;
    QPointF p2;
    for(int i = 0; i < data->indexPath->size() - 1; i++) {
        for(int j = i + 1; j < data->indexPath->size(); j++) {

//            if(data->indexes_smooth.value(path->at(i)).x() == data->indexes_smooth.value(path->at(j)).x() ||
//                    data->indexes_smooth.value(path->at(i)).y() == data->indexes_smooth.value(path->at(j)).y()) {
//                qDebug() << "skip " << data->indexes_smooth.value(path->at(i)).x()
//                         << data->indexes_smooth.value(path->at(j)).x()
//                         << data->indexes_smooth.value(path->at(i)).y()
//                         << data->indexes_smooth.value(path->at(j)).y();
//                continue;
//            }

            p1.setX(data->arr_points->at(data->indexPath->at(i)).x);
            p1.setY(data->arr_points->at(data->indexPath->at(i)).y);
            p2.setX(data->arr_points->at(data->indexPath->at(j)).x);
            p2.setY(data->arr_points->at(data->indexPath->at(j)).y);

            Edge ed{i, j, sqrt(pow(p2.x() - p1.x(), 2.) + pow(p2.y() - p1.y(), 2.))};
            if(ed.cost < 0)
                qDebug() << "stop";
            double widthCell = ((p2.x() - p1.x()) / ed.cost) * Ls;
            double hightCell = ((p2.y() - p1.y()) / ed.cost) * Ls;
            bool check = false;
            for(int k = 0; k < ed.cost / Ls; k++) {      // Строим точки по пути от А до Б и проверяем в элипсе
                double x = p1.x() + widthCell * k;
                double y = p1.y() + hightCell * k;
                if(isPointInsideElypseRLS(data->zoneRLSEnemy, x, y)) {
                    check = true;
                    break;
                }
                count_tick++;
            }

            if(check)
                break;
            else {
                data->checkEdge->append(ed);
            }
        }
        a = p1;
    }
//    int siz_graph = data->checkEdge->size();
//    for(int i = 0; i < siz_graph; i++) {
//        Edge ed = {data->checkEdge->at(i).b, data->checkEdge->at(i).a, data->checkEdge->at(i).cost};
//        data->checkEdge->append(ed);
//    }
}

bool CalcTrack::checkEdge(Edge ed)
{
    for(int i = 0; i < data->edge_arr->size(); i++)
        if(ed.a == data->edge_arr->at(i).a && ed.b == data->edge_arr->at(i).b)
            return true;
    return false;
}

QVector<QPoint> *CalcTrack::getPoints()
{
    return data->meshPoints;
}

void CalcTrack::calcTrack(SIDE a_side, SEARCH_SIDE a_search_side)
{
    side = a_side;
    search_side = RIGHT;
    if(data->pointsFlightEnemy.size() < 2)
    {
        qDebug() << "very small size point flight";
        return;
    }
    for(int p_index = 0; p_index < data->pointsFlightEnemy.size() - 1; p_index++) {

        QPoint p1 = data->pointsFlightEnemy[p_index];
        QPoint p2 = data->pointsFlightEnemy[p_index + 1];


        for(int c = 0; c < COUNT_IT; c++) {
            if(p1.x() < p2.x() && p1.y() < p2.y())
                card_point = 0;
            else if(p1.x() < p2.x() && p1.y() > p2.y())
                card_point = 1;
            else if(p1.x() > p2.x() && p1.y() > p2.y())
                card_point = 2;
            else if(p1.x() > p2.x() && p1.y() < p2.y())
                card_point = 3;
            search_side = (SEARCH_SIDE) c;
            mat.clear();
            data->arr_points->clear();
            data->edge_arr->clear();
            data->path->clear();
            data->checkEdge->clear();
            data->pathSmoothing->clear();

            Ls = data->enemyAir.radiusTurn * 2. / COOF_RAT;  // '-[Ширина стороны квадрата сетки (два средних радиуса разворота)
            double Lp = sqrt(pow((p2.x() - p1.x()), 2) + pow((p2.y() - p1.y()), 2));   // '-[Lp - длина полосы
            double widthCell = ((p2.x() - p1.x()) / Lp) * Ls;
            double hightCell = ((p2.y() - p1.y()) / Lp) * Ls;
            double angleCos = (p2.x() - p1.x()) / Lp;      // Косинус угла
            double angleSin = (p2.y() - p1.y()) / Lp;      // Синус угла
            sin_lastPath = angleSin;
            qDebug() << hightCell << widthCell << Ls << Lp;
            if(Lp / Ls > 3) {
                int right_border = Lp / Ls;

                double appendix_g = Lp - (right_border * Ls);
                double appendix_width = appendix_g * angleCos;
                double appendix_hight = appendix_g * angleSin;
                int index_number = -1;
                for(int i = 0; i < right_border; i++) {
                    QPoint point;
                    QVector <UT> line_ut;
                    int j = 0;
                    for(j = 0; j <= right_border; j++) {
                        index_number++;
                        double x;
                        double y;
                        if(c == 1) {
                            x = p1.x() + widthCell * j + hightCell * i;
                            y = p1.y() + -widthCell * i + hightCell * j;
                        } else {
                            x = p1.x() + widthCell * j + -hightCell * i;
                            y = p1.y() + widthCell * i + hightCell * j;
                        }
                        if(!isPointInsideElypseRLS(data->zoneRLSEnemy, x, y)) {
//                            qDebug() << "this point not inside elypse" ;
                            point.setX(x);
                            point.setY(y);
                            data->meshPoints->append(point);
                            line_ut.append({x, y, true});
                        } else {
                            line_ut.append({x, y, false});
                        }
//                        qDebug() << Lp / widthCell << "i=" << i << " j=" << j;
                        if(j == (right_border) && search_side == RIGHT)           // *******************  перед переходом на следующую строку
                        {
                            if(i == 0) {
                                data->meshPoints->append(p2);
                                line_ut.append({(double)p2.x(), (double)p2.y(), true});
                            } else {
                                double x_appendix = x + appendix_width ;
                                double y_appendix = y + appendix_hight ;
                                if(!isPointInsideElypseRLS(data->zoneRLSEnemy, x_appendix, y_appendix)) {
                                    data->meshPoints->append(QPoint(x_appendix, y_appendix));
                                    line_ut.append({x_appendix, y_appendix, true});
                                } else {
                                    line_ut.append({x_appendix, y_appendix, false});
                                }
                            }
                        }
                        data->indexes.insert(index_number, QPoint(j, i));
                    }
    //                if(i == (right_border - 1) )           // *******************  перед переходом на следующую строку
    //                {
    //                    index_number++;
    //    //                if(i == 0) {
    //    //                    points->append(p2);
    //    //                    line_ut.append({(double)p2.x(), (double)p2.y(), true});
    //    //                } else {
    //    //                    points->append(p2);
    //                        line_ut.append({(double)p2.x(), (double)p2.y(), false});
    //                        data->indexes.insert(index_number, QPoint(j+1, i));

    //    //                }
    //                }
                    mat.append(line_ut);
                }
                setEdge();
            }

            if(!mat.isEmpty()){
                if(!mat.first().isEmpty())
                    number_need_vert = mat.first().size();
                else
                    return;
            }
            else
                return;
            calcPath(number_need_vert);
            setEdgeOnPath();        // <--

            if(data->path->isEmpty()) {
                qDebug() << " path not faund";
            } else {
                smoothingPath();
                // array UT path smooth
                QVector <UT> tmp_path_smooth = *data->pathSmoothing;
                data->vec_pathSmooth->append(tmp_path_smooth);
                data->pathSmoothing->clear();
            }

            // mesh points (QPoint)
            QVector<QPoint> tmp_mesh = *data->meshPoints;
            data->vec_meshPoints->append(tmp_mesh);
            data->meshPoints->clear();

            // points UT
            QVector <UT> tmp_points = *data->arr_points;
            data->vec_points->append(tmp_points);
            data->arr_points->clear();

            // Edge array source UT
            QVector <Edge> tmp_edge = *data->edge_arr;
            data->vec_edge->append(tmp_edge);
            data->edge_arr->clear();


            // Edge array path check UT
            QVector <Edge> tmp_edgeCheck = *data->checkEdge;
            data->vec_edge_check->append(tmp_edgeCheck);
            data->checkEdge->clear();

            QVector <UT> tmp_path = *data->path;
            data->vec_path->append(tmp_path);
            data->path->clear();

        }
//        data->lengthPath
        if(getMinimumLength() == -1) {
            qDebug() << "not faund path";
            return;
        }
        PathTrack pathTrack;
        pathTrack.path = data->vec_pathSmooth->at(getMinimumLength());
        pathTrack.length = data->lengthPath.at(getMinimumLength());
        data->path_short.append(pathTrack);
        data->vec_pathSmooth->clear();
        data->lengthPath.clear();
    }

}

int CalcTrack::getMinimumLength()
{
    if(data->lengthPath.isEmpty())
        return -1;
    double minPath = data->lengthPath[0];
    int index = 0;
    for(int i = 0; i < data->lengthPath.size(); i++) {
        if(minPath > data->lengthPath.at(i))
            index = i;
    }
    return index;
}

void CalcTrack::edgeAppend(Edge ed)
{
    data->edge_arr->append(ed);
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


