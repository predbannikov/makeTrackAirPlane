#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QString>
#include <QLabel>
#include <QKeyEvent>
#include "global.h"
#include "scribblearea.h"
#include "dialogzonerls.h"
#include "chooseairplane.h"
#include "calctrack.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ScribbleArea;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void routeAtoB();
private:

    void loadDivision();
    void initAirplanes();
    void updateLabelInfo();
    int getMinimumLength();
    ScribbleArea *scribbleArea;
    Ui::MainWindow *ui;
    QLabel *label;
    Coord coord;
    QImage loadedImage;
    QSize sizeImage;
    QVector<QPoint> loadArrBorder(QString a_fileName);
//    bool isPointInsidePolygon (QVector<QPoint> p, int x, int y);
    void keyReleaseEvent(QKeyEvent *event) override;
    void setPoint(int x, int y);
    void generatePoint();
    void setPoints(QVector <QPoint> *points);
    void setLines(QVector<Edge> *edge, QVector <UT> *a_points, int size_widge, QColor a_color);
    void setLinesOnPoint(QVector<UT> *ut, int size_widge, QColor a_color);
    void setLinesOnPointSmooth(QVector<UT> *ut, int size_widge, QColor a_color);
//    QImage sourceImg;
    QImage resultImg;
//    QVector<ZoneRLS> vecZoneRLS;
    Data *data;
    CalcTrack *track;
public slots:
    void updatePix(QImage *img);
    void drawLine(UT p1, UT p2, int width, QColor col = QColor(50, 50, 50), QString str = "");
    void drawPoint(UT p1, int width, QColor col = QColor(50, 50, 50));


private slots:
    void on_pushButton_clicked();
    void on_pbZoneRLS_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
};
#endif // MAINWINDOW_H
