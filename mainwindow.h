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
//    QImage sourceImg;
    QImage resultImg;
//    QVector<ZoneRLS> vecZoneRLS;
    Data *data;
public slots:
    void updatePix(QImage *img);


private slots:
    void on_pushButton_clicked();
    void on_pbZoneRLS_clicked();
};
#endif // MAINWINDOW_H
