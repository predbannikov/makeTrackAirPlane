#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QFile>

bool isPointInsidePolygon (QVector<QPoint> p, int x, int y);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scribbleArea = new ScribbleArea();
    ui->groupBox_4->hide(); // убрать
    label = new QLabel;
    data = new Data;

    QScrollArea *scroll = new QScrollArea;
    ui->groupBox_3->layout()->addWidget(scroll);


    loadedImage.load("../../map.png");

    scribbleArea->setCoord(&coord);
    sizeImage = loadedImage.size();
    scribbleArea->openImage(loadedImage);


    scroll->setWidgetResizable(true);
    scroll->setWidget(label);


    QObject::connect(scribbleArea, &ScribbleArea::updateImg, this, &MainWindow::updatePix);

    loadDivision();
    this->setGeometry(0, 0, 1366, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePix(QImage *img)
{
    qDebug() << "update";
}

void MainWindow::loadDivision()
{
    data->pointBorderRussia = loadArrBorder("../../border_russia.pos");
    QImage imgRussia = scribbleArea->loadDivision(data->pointBorderRussia);
    scribbleArea->openImage(imgRussia);

    data->pointBorderEnemy = loadArrBorder("../../border_enemy.pos");
    QImage imgEnemy = scribbleArea->loadDivision(data->pointBorderEnemy);
    data->image = imgEnemy;
    label->setPixmap(QPixmap::fromImage(data->image));
}

QVector<QPoint> MainWindow::loadArrBorder(QString a_fileName)
{
    QFile fileDivision(a_fileName);
    if(!fileDivision.open(QFile::Text | QFile::ReadOnly)) {
        qDebug() << "file division.pos not found";
        return QVector<QPoint>();
    }
    QVector <QPoint> points;
    QString str = fileDivision.readAll();
    fileDivision.close();
    QStringList list = str.split('\n');
    for(int i = 0; i < list.count(); i++) {
        if(list[i].isEmpty())
            continue;
        QStringList listCoord = list[i].split(':');
        QPoint point(listCoord[0].toInt() - coord.x, listCoord[1].toInt() - coord.y);
        points.push_back(point);
    }
    return points;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Right)
    {
        coord.x += 500;
        qDebug() << "right";
        QImage im = loadedImage.copy(coord.x, coord.y, loadedImage.size().width(), loadedImage.size().height());
        scribbleArea->openImage(im);
    } else if(event->key() == Qt::Key_Left) {
        coord.x -= 500;
        qDebug() << "left";
        QImage im = loadedImage.copy(coord.x, coord.y, loadedImage.size().width(), loadedImage.size().height());
        scribbleArea->openImage(im);
    } else if(event->key() == Qt::Key_Down) {
        coord.y += 250;
        qDebug() << "down";
        QImage im = loadedImage.copy(coord.x, coord.y, loadedImage.size().width(), loadedImage.size().height());
        scribbleArea->openImage(im);
    } else if(event->key() == Qt::Key_Up) {
        coord.y -= 250;
        qDebug() << "down";
        QImage im = loadedImage.copy(coord.x, coord.y, loadedImage.size().width(), loadedImage.size().height());
        scribbleArea->openImage(im);
    } else if(event->key() == Qt::Key_Escape) {
        this->close();
    }
    loadDivision();
}

void MainWindow::setPoint(int x, int y)
{
    data->image = scribbleArea->setPoint(QString::number(x) + ":" + QString::number(y), x, y);
    scribbleArea->openImage(data->image);
    label->setPixmap(QPixmap::fromImage(data->image));

}

void MainWindow::generatePoint()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    int x, y;
    do {
        QThread::msleep(15);
        x = qrand() % sizeImage.width();
        y = qrand() % sizeImage.height();
    }
    while(!isPointInsidePolygon(data->pointBorderEnemy, x, y));
    qDebug() << x << y << "true";
    this->setPoint(x, y);

}



void MainWindow::on_pushButton_clicked()
{
    for(int i = 0; i < 30; i++)
        generatePoint();
}

void MainWindow::on_pbZoneRLS_clicked()
{
    DialogZoneRLS* dialogZoneRLS = new DialogZoneRLS(data, ENEMY);
    if (dialogZoneRLS->exec() == QDialog::Accepted)
    {
       qDebug() << "accept";
       data->zoneRLS = data->lastZoneRLS;
       data->pointsFlight = data->lastPointsFlight;
       label->setPixmap(QPixmap::fromImage(data->lastImage));
    } else {
        qDebug() << "reject";
    }
    delete dialogZoneRLS;
}

void MainWindow::on_pushButton_2_clicked()
{
    ChooseAirplane *chooseAir = new ChooseAirplane(data, ENEMY);
    if(chooseAir->exec() == QDialog::Accepted)
    {
        qDebug() << "accept";
    } else {
        qDebug() << "reject";
    }
    delete chooseAir;
}
