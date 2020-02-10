#include "dialogzonerls.h"


DialogZoneRLS::DialogZoneRLS(Data *a_data, SIDE a_side)
{
    m_side = a_side;
    data = a_data;
    data->lastZoneRLS = data->zoneRLS;
    data->lastPointsFlight = data->pointsFlight;
    data->lastImage = data->image;

    QGridLayout* ptopLayout = new QGridLayout;

    // --------------------------------------- MORE RLS
    QGroupBox *groupBox = new QGroupBox;
    groupBox->setVisible(false);
    QHBoxLayout *phboxLayout_3 = new QHBoxLayout;
    groupBox->setLayout(phboxLayout_3);
    ptopLayout->addWidget(groupBox, 1, 2, 1, 3);
    // . . . . .
    // . . * * *
    // . . . . .
    // . . . . .
    // . . . . .
    QPushButton *pbCheckPoint = new QPushButton("Проверить точку");
    connect(pbCheckPoint, &QPushButton::clicked, [this, pbCheckPoint] () mutable {
        ZoneRLS rls;
        rls.x = this->sbXCoordRLS->value();
        rls.y = this->sbYCoordRLS->value();
        rls.radius = this->sbRCoordRLS->value();
        pbCheckPoint->setAutoFillBackground(true);
        if(checkPointRLS(rls)) {
            QPalette pal;
            pal.setColor(QPalette::Button, QColor(0, 255, 0));
            pbCheckPoint->setPalette(pal);
        } else {
            QPalette pal;
            pal.setColor(QPalette::Button, QColor(255, 0, 0));
            pbCheckPoint->setPalette(pal);
        }
        pbCheckPoint->update();
    });
    QPushButton *pbEnterPoint = new QPushButton("Внести точку");
    connect(pbEnterPoint, &QPushButton::clicked, [this] () mutable {
        ZoneRLS rls;
        rls.x = this->sbXCoordRLS->value();
        rls.y = this->sbYCoordRLS->value();
        rls.radius = this->sbRCoordRLS->value();
        setPointRLS(rls);
    });
    QPushButton *pbCanclePoint = new QPushButton("Отмена");
    connect(pbCanclePoint, &QPushButton::clicked, [groupBox]() mutable {
        if(groupBox->isVisible()) {
            groupBox->setVisible(false);
        }
    });
//    QPushButton *pbOkPoint = new QPushButton("Сохранить");
//    connect(pbOkPoint, &QPushButton::clicked, [groupBox]() mutable {
//        if(groupBox->isVisible()) {
//            groupBox->setVisible(false);
//        }
//    });
    sbXCoordRLS = new QSpinBox;
    sbXCoordRLS->setMaximum(data->image.size().width());
    sbXCoordRLS->setMinimum(1);
    sbYCoordRLS = new QSpinBox;
    sbYCoordRLS->setMaximum(data->image.size().height());
    sbYCoordRLS->setMinimum(1);
    sbRCoordRLS = new QSpinBox;
    sbRCoordRLS->setMaximum(70);
    sbRCoordRLS->setMinimum(1);
    sbRCoordRLS->setValue(15);
    cmbChoiceRLS = new QComboBox;
    connect(cmbChoiceRLS, qOverload<int>(&QComboBox::currentIndexChanged), [this] (int value) mutable {
        if(value >= this->data->lastZoneRLS.size() )
            cmbChoiceRLS->setCurrentIndex(this->data->lastZoneRLS.size());
    });
    phboxLayout_3->addWidget(cmbChoiceRLS);
    QHBoxLayout *hbRLS_X = new QHBoxLayout;
    hbRLS_X->addWidget(new QLabel("X"));
    hbRLS_X->addWidget(sbXCoordRLS);
    phboxLayout_3->addLayout(hbRLS_X);
    QHBoxLayout *hbRLS_Y = new QHBoxLayout;
    hbRLS_Y->addWidget(new QLabel("Y"));
    hbRLS_Y->addWidget(sbYCoordRLS);
    phboxLayout_3->addLayout(hbRLS_Y);
    QHBoxLayout *hbRLS_R = new QHBoxLayout;
    hbRLS_R->addWidget(new QLabel("R"));
    hbRLS_R->addWidget(sbRCoordRLS);
    phboxLayout_3->addLayout(hbRLS_R);
    phboxLayout_3->addWidget(pbCheckPoint);
    phboxLayout_3->addWidget(pbEnterPoint);
    phboxLayout_3->addWidget(pbCanclePoint);

    // --------------------------------------- MORE FLIGHT
    QGroupBox *groupBox_2 = new QGroupBox;
    groupBox_2->setVisible(false);
    QHBoxLayout *phboxLayout_4 = new QHBoxLayout;
    groupBox_2->setLayout(phboxLayout_4);
    ptopLayout->addWidget(groupBox_2, 3, 2, 1, 3);
    // . . . . .
    // . . . . .
    // . . . . .
    // . . * * *
    // . . . . .
    QPushButton *pbCheckPointFlight = new QPushButton("Проверить точку");
    connect(pbCheckPointFlight, &QPushButton::clicked, [this, pbCheckPointFlight] () mutable {
        QPoint point;
        point.setX(this->sbXCoordFlight->value());
        point.setY(this->sbYCoordFlight->value());
        pbCheckPointFlight->setAutoFillBackground(true);
        if(checkPointFlight(point)) {
            QPalette pal;
            pal.setColor(QPalette::Button, QColor(0, 255, 0));
            pbCheckPointFlight->setPalette(pal);
        } else {
            QPalette pal;
            pal.setColor(QPalette::Button, QColor(255, 0, 0));
            pbCheckPointFlight->setPalette(pal);
        }
        pbCheckPointFlight->update();
    });
    QPushButton *pbEnterPointFlight = new QPushButton("Внести точку");
    connect(pbEnterPointFlight, &QPushButton::clicked, [this] () mutable {
        QPoint point;
        point.setX(this->sbXCoordFlight->value());
        point.setY(this->sbYCoordFlight->value());
        setPointFlight(point);
    });    QPushButton *pbCanclePointFlight = new QPushButton("Отмена");
    connect(pbCanclePointFlight, &QPushButton::clicked, [groupBox_2]() mutable {
        if(groupBox_2->isVisible()) {
            groupBox_2->setVisible(false);
        }
    });
    sbXCoordFlight = new QSpinBox;
    sbXCoordFlight->setMaximum(data->image.size().width());
    sbXCoordFlight->setMinimum(1);
    sbYCoordFlight = new QSpinBox;
    sbYCoordFlight->setMaximum(data->image.size().height());
    sbYCoordFlight->setMinimum(1);

    cmbChoiceFlight = new QComboBox;
    connect(cmbChoiceFlight, qOverload<int>(&QComboBox::currentIndexChanged), [this] (int value) mutable {
        if(value >= this->data->lastPointsFlight.size() )
            cmbChoiceFlight->setCurrentIndex(this->data->lastPointsFlight.size());
    });
    phboxLayout_4->addWidget(cmbChoiceFlight);
    QHBoxLayout *hbFlight_X = new QHBoxLayout;
    hbFlight_X->addWidget(new QLabel("X"));
    hbFlight_X->addWidget(sbXCoordFlight);
    phboxLayout_4->addLayout(hbFlight_X);
    QHBoxLayout *hbFlight_Y = new QHBoxLayout;
    hbFlight_Y->addWidget(new QLabel("X"));
    hbFlight_Y->addWidget(sbYCoordFlight);
    phboxLayout_4->addLayout(hbFlight_Y);
    phboxLayout_4->addWidget(pbCheckPointFlight);
    phboxLayout_4->addWidget(pbEnterPointFlight);
    phboxLayout_4->addWidget(pbCanclePointFlight);

    // ---------------------------------------


    QPushButton* pcmdOk     = new QPushButton("&Ok");
    QPushButton* pcmdCancel = new QPushButton("&Cancel");

    // --------------------------------------- RLS
    // * * . . .
    // . . . . .
    // . . . . .
    // . . . . .
    // . . . . .
    QHBoxLayout *phboxLayout = new QHBoxLayout;
    ptopLayout->addLayout(phboxLayout, 0, 0, 1, 2);
    QLabel *lbCountRLS = new QLabel("Кол-во зон РЛС");
    QLabel *lbCountRLSsuf = new QLabel("(шт)");
    sbCountRLS = new QSpinBox;
    sbCountRLS->setMaximum(30);
    sbCountRLS->setMinimum(1);
    sbCountRLS->setValue(4);
    connect(sbCountRLS, qOverload<int>(&QSpinBox::valueChanged), [this](int value) {
        qDebug() << "value = " << value << "size = " << this->data->lastZoneRLS.size();
        if(value < this->data->lastZoneRLS.size())
            this->data->lastZoneRLS.remove(value, this->data->lastZoneRLS.size() - value);
        setCmbRLS();
    });
    phboxLayout->addWidget(lbCountRLS);
    phboxLayout->addWidget(sbCountRLS);
    phboxLayout->addWidget(lbCountRLSsuf);

    // --------------------------------------- FLIGHT
    QHBoxLayout *phboxLayout_2 = new QHBoxLayout;
    ptopLayout->addLayout(phboxLayout_2, 2, 0, 1, 2);
    // . . . . .
    // . . . . .
    // * * . . .
    // . . . . .
    // . . . . .
    QLabel *lbCountFlight = new QLabel("Кол-во точек облёта");
    QLabel *lbCountFlightsuf = new QLabel("(шт)");
    sbCountPointsFlight = new QSpinBox;
    sbCountPointsFlight->setMaximum(30);
    sbCountPointsFlight->setMinimum(1);
    sbCountPointsFlight->setValue(5);
    connect(sbCountPointsFlight, qOverload<int>(&QSpinBox::valueChanged), [this](int value) {
        qDebug() << "value = " << value << "size = " << this->data->lastPointsFlight.size();
        if(value < this->data->lastPointsFlight.size())
            this->data->lastPointsFlight.remove(value, this->data->lastPointsFlight.size() - value);
        setCmbFlight();
    });
    phboxLayout_2->addWidget(lbCountFlight);
    phboxLayout_2->addWidget(sbCountPointsFlight);
    phboxLayout_2->addWidget(lbCountFlightsuf);

    // ---------------------------------------
    QPushButton *pbInputDataRLS = new QPushButton("Ввести данные о точках RLS");
    connect(pbInputDataRLS, &QPushButton::clicked, this, &DialogZoneRLS::setDataPointsRLS);
    ptopLayout->addWidget(pbInputDataRLS, 0, 2);
    // . . * . .
    // . . . . .
    // . . . . .
    // . . . . .
    // . . . . .
    QPushButton* plblLastName     = new QPushButton("Задать автоматически данные о зонах РЛС");
    connect(plblLastName, &QPushButton::clicked, this, &DialogZoneRLS::autoGenerateRLS);
    ptopLayout->addWidget(plblLastName, 0, 3);
    // . . . * .
    // . . . . .
    // . . . . .
    // . . . . .
    // . . . . .
    QPushButton* pbInputDataZoneRLS = new QPushButton("Ввести вручную данные о зонах РЛС");
    connect(pbInputDataZoneRLS, &QPushButton::clicked, [this, groupBox]() mutable {
        if(!groupBox->isVisible()) {
            groupBox->setVisible(true);
//            this->setCmbRLS();
        }
    });
    ptopLayout->addWidget(pbInputDataZoneRLS, 0, 4);
    // . . . . *
    // . . . . .
    // . . . . .
    // . . . . .
    // . . . . .


    // ---------------------------------------
    QPushButton *pbInputDataFlight = new QPushButton("Ввести данные о точках облёта");
    connect(pbInputDataFlight, &QPushButton::clicked, this, &DialogZoneRLS::setDataPointsFlight);
    ptopLayout->addWidget(pbInputDataFlight, 2, 2);
    // . . . . .
    // . . . . .
    // . . * . .
    // . . . . .
    // . . . . .
    QPushButton* pbGenPointsFlight = new QPushButton("Задать автоматически точки облёта");
    connect(pbGenPointsFlight, &QPushButton::clicked, this, &DialogZoneRLS::autoGenerateFlight);
    ptopLayout->addWidget(pbGenPointsFlight, 2, 3);
    // . . . . .
    // . . . . .
    // . . . * .
    // . . . . .
    // . . . . .
    QPushButton* pbInputCustomDataFlight = new QPushButton("Ввести вручную данные точек облёта");
    connect(pbInputCustomDataFlight, &QPushButton::clicked, [groupBox_2]() mutable {
        if(!groupBox_2->isVisible()) {
            groupBox_2->setVisible(true);
        }
    });
    ptopLayout->addWidget(pbInputCustomDataFlight, 2, 4);
    // . . . . .
    // . . . . .
    // . . . . *
    // . . . . .
    // . . . . .



    // ---------------------------------------

    QScrollArea *scroll = new QScrollArea;
    imgLabel = new QLabel;
    imgLabel->setPixmap(QPixmap::fromImage(data->lastImage));
    scroll->setWidget(imgLabel);
    scroll->setWidgetResizable(true);

    ptopLayout->addWidget(scroll, 4, 0, 1, 5);
    // . . . . .
    // . . . . .
    // . . . . .
    // . . . . .
    // * * * * *



    connect(pcmdOk, SIGNAL(clicked()), SLOT(acceptButton()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

    ptopLayout->addWidget(pcmdOk, 7,0);
    ptopLayout->addWidget(pcmdCancel, 7, 1);
    setLayout(ptopLayout);


    if(!data->lastZoneRLS.isEmpty())
        setCountRLS();
    this->setGeometry(0, 0, 1366, 600);
    setCmbRLS();
    setCmbFlight();

}

QImage DialogZoneRLS::setPointRLS(QString a_str, int x, int y, int r, int a_widthPen, QColor a_color)
{
    QPainter painter(&data->lastImage);
    QPoint point = QPoint(x, y);
    painter.setPen(QPen(a_color, a_widthPen, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawPoint(point);
    painter.drawText(point, a_str);
    painter.setPen(QPen(a_color, 2, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QRect rect(x - r , y - r , r * 2, r * 2);
    painter.drawEllipse(rect);
    update();
    return data->lastImage;
}

QImage DialogZoneRLS::setPointsFlight(QString a_str, int x, int y, int a_widthPen, QColor a_color)
{
    QPainter painter(&data->lastImage);
    QPoint point = QPoint(x, y);
    painter.setPen(QPen(a_color, a_widthPen, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawPoint(point);
    painter.drawText(point, a_str);
    painter.setPen(QPen(a_color, 2, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    update();
    return data->lastImage;
}

void DialogZoneRLS::setCmbRLS()
{
    cmbChoiceRLS->clear();
    for(int i = 0; i < sbCountRLS->value(); i++) {
        cmbChoiceRLS->addItem(QString::number(i+1));
    }
    for(int i = 0; i < data->lastZoneRLS.size(); i++) {
        ZoneRLS rls = data->lastZoneRLS.at(i);
        QString str = QString("%1:%2:%3").arg(rls.x).arg(rls.y).arg(rls.radius);
        cmbChoiceRLS->setItemText(i, str);
        cmbChoiceRLS->setItemData(i, QColor(Qt::black), Qt::TextColorRole);
        cmbChoiceRLS->setItemData(i, QColor(Qt::green), Qt::BackgroundRole);
    }

}

void DialogZoneRLS::setCmbFlight()
{
    cmbChoiceFlight->clear();
    for(int i = 0; i < sbCountPointsFlight->value(); i++) {
        cmbChoiceFlight->addItem(QString::number(i+1));
    }
    for(int i = 0; i < data->lastPointsFlight.size(); i++) {
        QPoint point = data->lastPointsFlight.at(i);
        QString str = QString("%1:%2").arg(point.x()).arg(point.y());
        cmbChoiceFlight->setItemText(i, str);
        cmbChoiceFlight->setItemData(i, QColor(Qt::black), Qt::TextColorRole);
        cmbChoiceFlight->setItemData(i, QColor(Qt::green), Qt::BackgroundRole);
    }
}

bool DialogZoneRLS::checkPointRLS(ZoneRLS a_rls)
{
    QVector<ZoneRLS> checkRLS;
    checkRLS.append(a_rls);
    QVector<QPoint> pointBorder;
    if(m_side == ENEMY)
        pointBorder = data->pointBorderEnemy;
    else
        pointBorder = data->pointBorderRussia;

    if(this->data->lastPointsFlight.isEmpty()) {
        if(isPointInsidePolygon(pointBorder, a_rls.x, a_rls.y))
            return true;
        else
            return false;
    } else {
        if(isPointInsidePolygon(pointBorder, a_rls.x, a_rls.y)) {
            qDebug() << "this point within given territory ";
        } else {
            qDebug() << "this point not within given territory ";
            return false;
        }
        bool check = true;
        for(int i = 0; i < this->data->lastPointsFlight.size(); i++) {
            if(isPointInsideElypseRLS(checkRLS,
                                       this->data->lastPointsFlight.at(i).x(),
                                       this->data->lastPointsFlight.at(i).y()))
                check = false;
        }
        if(!check)
            qDebug() << "this point  within given light points";
        else
            qDebug() << "this point not within given light points";

        return check;
    }
}

bool DialogZoneRLS::checkPointFlight(QPoint a_point)
{
    QVector<QPoint> pointBorder;
    if(m_side == ENEMY)
        pointBorder = data->pointBorderEnemy;
    else
        pointBorder = data->pointBorderRussia;

    if(this->data->lastZoneRLS.isEmpty()) {
        if(isPointInsidePolygon(pointBorder, a_point.x(), a_point.y()))
            return true;
        else
            return false;
    } else {
        if(isPointInsidePolygon(pointBorder, a_point.x(), a_point.y())) {
            qDebug() << "this point within given territory ";
        } else {
            qDebug() << "this point not within given territory ";
            return false;
        }
        bool check = true;
        if(isPointInsideElypseRLS(data->lastZoneRLS, a_point.x(), a_point.y()))
            check = false;
        if(!check)
            qDebug() << "this point  within given light points";
        else
            qDebug() << "this point not within given light points";

        return check;
    }
}

void DialogZoneRLS::acceptButton()
{
    setDataPointsRLS();
    setDataPointsFlight();
    accept();
}

void DialogZoneRLS::autoGenerateRLS()
{
    data->lastZoneRLS.clear();
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QVector<QPoint> pointBorder;
    if(m_side == ENEMY)
        pointBorder = data->pointBorderEnemy;
    else
        pointBorder = data->pointBorderRussia;

    int x, y, r;
    for(int i = 0; i < sbCountRLS->value(); i++) {
        ZoneRLS rls;
        do {
            QThread::msleep(1);
            x = qrand() % data->image.width();
            y = qrand() % data->image.height();
            r = qrand() % 75 + 10;
            rls = {x, y, r};
        } while(!checkPointRLS(rls));
        data->lastZoneRLS.append(rls);
    }
    qDebug() << x << y << r << "true RLS" << (m_side == ENEMY ? "enemy" : "russia") ;
    setCmbRLS();
}

void DialogZoneRLS::autoGenerateFlight()
{
    data->lastPointsFlight.clear();
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QVector<QPoint> pointBorder;
    if(m_side == ENEMY)
        pointBorder = data->pointBorderEnemy;
    else
        pointBorder = data->pointBorderRussia;

    for(int i = 0; i < sbCountPointsFlight->value(); i++) {
        int x, y;
        do {
            do {
                QThread::msleep(11);
                x = qrand() % data->image.width();
                y = qrand() % data->image.height();

            } while(isPointInsideElypseRLS(data->lastZoneRLS, x, y));

        }
        while(!isPointInsidePolygon(pointBorder, x, y));
        QPoint point;
        point.setX(x);
        point.setY(y);
        data->lastPointsFlight.append(point);
        qDebug() << point.x() << point.y() << "true flight" << (m_side == ENEMY ? "enemy" : "russia");
    }
    setCmbFlight();
}

void DialogZoneRLS::setCountRLS()
{
    sbCountRLS->setValue(data->lastZoneRLS.size());
}

void DialogZoneRLS::setPointRLS(ZoneRLS a_rls)
{
    QString str = QString("%1:%2:%3").arg(a_rls.x).arg(a_rls.y).arg(a_rls.radius);
    cmbChoiceRLS->setItemText(cmbChoiceRLS->currentIndex(), str);
    cmbChoiceRLS->setItemData(cmbChoiceRLS->currentIndex(), QColor(Qt::black), Qt::TextColorRole);
    cmbChoiceRLS->setItemData(cmbChoiceRLS->currentIndex(), QColor(Qt::green), Qt::BackgroundRole);
    if(cmbChoiceRLS->currentIndex() >= data->lastZoneRLS.size())
        data->lastZoneRLS.append(a_rls);
    else
        data->lastZoneRLS.replace(cmbChoiceRLS->currentIndex(), a_rls);
}

void DialogZoneRLS::setPointFlight(QPoint a_point)
{
    QString str = QString("%1:%2").arg(a_point.x()).arg(a_point.y());
    cmbChoiceRLS->setItemText(cmbChoiceFlight->currentIndex(), str);
    cmbChoiceRLS->setItemData(cmbChoiceFlight->currentIndex(), QColor(Qt::black), Qt::TextColorRole);
    cmbChoiceRLS->setItemData(cmbChoiceFlight->currentIndex(), QColor(Qt::green), Qt::BackgroundRole);
    if(cmbChoiceFlight->currentIndex() >= data->lastPointsFlight.size())
        data->lastPointsFlight.append(a_point);
    else
        data->lastPointsFlight.replace(cmbChoiceRLS->currentIndex(), a_point);
}

void DialogZoneRLS::setDataPointsRLS()
{
    qDebug() << "setDataPointsRLS";
    for(int i = 0; i < data->lastZoneRLS.size(); i++) {
        data->lastImage = setPointRLS(QString::number(data->lastZoneRLS.at(i).x)
                                      + ":" + QString::number(data->lastZoneRLS.at(i).y)
                                      + ":" + QString::number(data->lastZoneRLS.at(i).radius),
                                 data->lastZoneRLS.at(i).x,
                                 data->lastZoneRLS.at(i).y,
                                 data->lastZoneRLS.at(i).radius,
                                 7,
                                 QColor(255, 0, 0));
        imgLabel->setPixmap(QPixmap::fromImage(data->lastImage));
    }
}

void DialogZoneRLS::setDataPointsFlight()
{
    qDebug() << "setDataPointsFlight";
    for(int i = 0; i < data->lastPointsFlight.size(); i++) {
        data->lastImage = setPointsFlight(QString::number(data->lastPointsFlight.at(i).x())
                                          + ":" + QString::number(data->lastPointsFlight.at(i).y()),
                                 data->lastPointsFlight.at(i).x(),
                                 data->lastPointsFlight.at(i).y(),
                                 7,
                                 QColor(0, 0, 255));
        imgLabel->setPixmap(QPixmap::fromImage(data->lastImage));
    }
}
