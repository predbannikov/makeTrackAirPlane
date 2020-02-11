#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QFile>

//bool isPointInsidePolygon (QVector<QPoint> p, int x, int y);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    data = new Data;
    initAirplanes();

    scribbleArea = new ScribbleArea();
    ui->groupBox_4->hide(); // убрать
    label = new QLabel;

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

void MainWindow::initAirplanes()
{
    data->airplanes.append({"СУ-27", 2500, 450});
    data->airplanes.append({"МИГ-29", 2400, 550});
    data->airplanes.append({"Як-130", 1600, 700});
    data->airplanes.append({"ES-1", 1600, 700});
    data->airplanes.append({"ES-2", 2000, 500});
    data->airplanes.append({"ES-3", 1200, 300});
    data->russiaAir = data->airplanes.at(0);
    data->enemyAir = data->airplanes.at(3);
    updateLabelInfo();
}

void MainWindow::updateLabelInfo()
{
    ui->labelInfo->setAlignment(Qt::AlignCenter);
    ui->labelInfo->setText(QString("Россия: модель %1, скорость %2 км/ч, радиус поворота %3 м\n"
                                   "Враг: модель %4, скорость %5 км/ч, радиус поворота %6 м")
                           .arg(data->russiaAir.model).arg(data->russiaAir.speed).arg(data->russiaAir.radiusTurn)
                           .arg(data->enemyAir.model).arg(data->enemyAir.speed).arg(data->enemyAir.radiusTurn));
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

void MainWindow::on_pushButton_4_clicked()
{
    DialogZoneRLS* dialogZoneRLS = new DialogZoneRLS(data, ENEMY);
    if (dialogZoneRLS->exec() == QDialog::Accepted)
    {
       qDebug() << "accept";
       data->zoneRLSEnemy = data->lastZoneRLS;
       data->pointsFlightEnemy = data->lastPointsFlight;
       data->image = data->lastImage;
       label->setPixmap(QPixmap::fromImage(data->lastImage));
    } else {
        qDebug() << "reject";
    }
    delete dialogZoneRLS;
}

void MainWindow::on_pbZoneRLS_clicked()
{
    DialogZoneRLS* dialogZoneRLS = new DialogZoneRLS(data, RUSSIA);
    if (dialogZoneRLS->exec() == QDialog::Accepted)
    {
       qDebug() << "accept";
       data->zoneRLSRussia = data->lastZoneRLS;
       data->pointsFlightRussia = data->lastPointsFlight;
       data->image = data->lastImage;
       label->setPixmap(QPixmap::fromImage(data->lastImage));
    } else {
        qDebug() << "reject";
    }
    delete dialogZoneRLS;
}

void MainWindow::on_pushButton_2_clicked()
{
    ChooseAirplane *chooseAir = new ChooseAirplane(data, RUSSIA);
    if(chooseAir->exec() == QDialog::Accepted)
    {
        qDebug() << "accept";
        data->russiaAir = data->lastRussiaAir;
        updateLabelInfo();
    } else {
        qDebug() << "reject";
    }
    delete chooseAir;
}

void MainWindow::on_pushButton_3_clicked()
{
    ChooseAirplane *chooseAir = new ChooseAirplane(data, ENEMY);
    if(chooseAir->exec() == QDialog::Accepted)
    {
        qDebug() << "accept";
        data->enemyAir = data->lastEnemyAir;
        updateLabelInfo();
    } else {
        qDebug() << "reject";
    }
    delete chooseAir;
}


