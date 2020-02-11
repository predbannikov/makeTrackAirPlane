#include "chooseairplane.h"
#include <QApplication>
#include <QDesktopWidget>

ChooseAirplane::ChooseAirplane(Data *a_data, SIDE a_side)
{
    m_side = a_side;
    data = a_data;
    data->lastZoneRLS = data->zoneRLSRussia;
    data->lastPointsFlight = data->pointsFlightRussia;
    data->lastImage = data->image;

    connect(this, &ChooseAirplane::chooseAir, this, &ChooseAirplane::updateLabelInfo);


    QVBoxLayout *vbltopDialog = new QVBoxLayout;
    this->setLayout(vbltopDialog);

    QHBoxLayout *hblLabelHeader = new QHBoxLayout;
    QLabel *headerLabel = new QLabel("Выбрать модель самолёта");
    headerLabel->setAlignment(Qt::AlignCenter);
    hblLabelHeader->addWidget(headerLabel);

    QHBoxLayout *hblHeader = new QHBoxLayout;
    btChoosFromReadyAir = new QPushButton("Выбрать из готовых вариантов самолётов");
    connect(btChoosFromReadyAir, &QPushButton::clicked, [this] () mutable {
        srand(QDateTime::currentMSecsSinceEpoch());
        this->updateLabelInfo(rand() % 3);
    });
    hblHeader->addWidget(btChoosFromReadyAir);
    btInputManualDataAir = new QPushButton("Ввести данные вручную");
    connect(btInputManualDataAir, &QPushButton::clicked, [this] () mutable {
        if(customModel->text().isEmpty() || customModel->text()[0] == ' ') {
            QMessageBox msgBox;
            msgBox.setText("Поле имени не должно быть пустым или начинаться на пробел");
            msgBox.exec();
        } else {
            labelModel->setText(QString("Модель: %1").arg(customModel->text()));
            labelInfo->setText(QString("Модель %1, %2(км/ч), %3(м)").arg(customModel->text())
                               .arg(speedAir->value())
                               .arg(radiusAir->value()));
        }
        genAirplane.model = customModel->text();
        genAirplane.speed = speedAir->value();
        genAirplane.radiusTurn = radiusAir->value();

    });
    hblHeader->addWidget(btInputManualDataAir);
    btSetRandomDataAir = new QPushButton("Задать случайные данные");
    connect(btSetRandomDataAir, &QPushButton::clicked, [this]() mutable {
        srand(QDateTime::currentMSecsSinceEpoch());
        int max = rand() % 3 + 3;
        bool glasChar = false;
        bool doubleSogl = false;
        QString model_name;
        for(int i = 0; i < max; i++) {
            QThread::usleep(1);
            int  _type;
            if(model_name.isEmpty()) {
                _type = rand() % 2;
            } else {
                if(glasChar)
                    _type = 0;
                else if(doubleSogl)
                    _type = 1;
                else
                    _type = rand() % 2;
            }
            switch (_type) {
            case 0:
                if(!model_name.isEmpty())
                    if(listSoglChar.contains(model_name.back()))
                        doubleSogl = true;
                model_name.append(getGenSoglasChar());
                glasChar = false;
                break;
            case 1:
                model_name.append(getGenGlasChar());
                glasChar = true;
                break;
            }
        }
        model_name.append("-");
        model_name.append(QString::number(rand() % 10 + 1));
        qDebug() << model_name;
        customModel->setText(model_name);
        speedAir->setValue(rand() % (speedAir->maximum() - speedAir->minimum()) + speedAir->minimum());
        radiusAir->setValue(rand() % (radiusAir->maximum() - radiusAir->minimum()) + radiusAir->minimum());
    }); // также сделать ласт аиры для сохранения выбора
    hblHeader->addWidget(btSetRandomDataAir);


    // --------------------------------------- Choose model airplane group
//    QGroupBox *groupBox = new QGroupBox;
//    groupBox->setVisible(true);
    QHBoxLayout *phboxAirplane = new QHBoxLayout;
//    groupBox->setLayout(phboxAirplane);

    pbAir_1 = new QPushButton("");
    phboxAirplane->addWidget(pbAir_1);
    connect(pbAir_1, &QPushButton::clicked, [this](){
        emit chooseAir(0);
    });
    pbAir_2 = new QPushButton("");
    phboxAirplane->addWidget(pbAir_2);
    connect(pbAir_2, &QPushButton::clicked, [this](){
        emit chooseAir(1);
    });
    pbAir_3 = new QPushButton("");
    connect(pbAir_3, &QPushButton::clicked, [this](){
        emit chooseAir(2);
    });
    phboxAirplane->addWidget(pbAir_3);
    customModel = new QLineEdit;
    phboxAirplane->addWidget(customModel);
    customModel->setMaximumWidth(150);

    labelModel = new QLabel;
    labelModel->setAlignment(Qt::AlignCenter);


    QHBoxLayout *hblSetSpeed = new QHBoxLayout;
    QLabel *labelSetSpeed = new QLabel("Скорость самолёта");
    labelSetSpeed->setAlignment(Qt::AlignCenter);
    hblSetSpeed->addWidget(labelSetSpeed);

    QHBoxLayout *hblSetSpeed_2 = new QHBoxLayout;
    QLabel *labelSpeedInfo = new QLabel("Скорость самолёта составляет:");
    labelSpeedInfo->setAlignment(Qt::AlignCenter);
    speedAir = new QSpinBox;
    speedAir->setMaximumWidth(70);
    speedAir->setMaximum(30000);
    speedAir->setMinimum(100);
    speedAir->setValue(1000);
    QLabel *labelSpeedSuf = new QLabel("(км/ч)");
    hblSetSpeed_2->addWidget(labelSpeedInfo);
    hblSetSpeed_2->addWidget(speedAir);
    hblSetSpeed_2->addWidget(labelSpeedSuf);


    QHBoxLayout *hblLabelRadius = new QHBoxLayout;
    QLabel *labelRadius = new QLabel("Радиус разворота самолёта");
    hblLabelRadius->setAlignment(Qt::AlignCenter);
    hblLabelRadius->addWidget(labelRadius);

    QHBoxLayout *hblLabelRadius_2 = new QHBoxLayout;
    QLabel *labelRadius_2 = new QLabel("Радиус разворота самолёта состовляет:");
    labelRadius_2->setAlignment(Qt::AlignCenter);
    radiusAir = new QSpinBox;
    radiusAir->setMaximumWidth(70);
    radiusAir->setMaximum(10000);
    radiusAir->setMinimum(10);
    radiusAir->setValue(100);
    QLabel *labelRadiusSuf = new QLabel("(м)");
    hblLabelRadius_2->addWidget(labelRadius_2);
    hblLabelRadius_2->addWidget(radiusAir);
    hblLabelRadius_2->addWidget(labelRadiusSuf);

    labelInfo = new QLabel;
    labelInfo->setAlignment(Qt::AlignCenter);

    QHBoxLayout *hbBottomeButtons = new QHBoxLayout;
    QPushButton* pcmdOk     = new QPushButton("&Ok");
    QPushButton* pcmdCancel = new QPushButton("&Cancel");
    hbBottomeButtons->addWidget(pcmdOk);
    hbBottomeButtons->addWidget(pcmdCancel);
    connect(pcmdOk, SIGNAL(clicked()), SLOT(acceptButton()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

    vbltopDialog->addLayout(hblLabelHeader);
    vbltopDialog->addLayout(hblHeader);
    vbltopDialog->addLayout(phboxAirplane);
    vbltopDialog->addWidget(labelModel);
    vbltopDialog->addLayout(hblSetSpeed);
    vbltopDialog->addLayout(hblSetSpeed_2);
    vbltopDialog->addLayout(hblLabelRadius);
    vbltopDialog->addLayout(hblLabelRadius_2);
    vbltopDialog->addWidget(labelInfo);
    vbltopDialog->addLayout(hbBottomeButtons);


    init_airplane();



    this->setMaximumWidth(600);
    this->setMaximumHeight(600);
    QDesktopWidget *wgt = QApplication::desktop();
    this->move(wgt->screen()->rect().center() - this->rect().center());
    updateLabelInfo(0);
}

void ChooseAirplane::init_airplane()
{
    int index_side;
    if(m_side == ENEMY) {
        index_side = 3;
        listGlasChar = {'A', 'E', 'I', 'O', 'U'};
        listSoglChar = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N',
                              'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'};
    } else {
        index_side = 0;
        listGlasChar = {L'А', L'О', L'У', L'Е', L'И', L'Я', L'Ю', L'Э'};
        listSoglChar = {L'Б', L'П', L'Г', L'Д', L'Х', L'Р', L'Ж', L'К', L'С', L'Ч', L'М', L'В', L'З', L'Л', L'Ш', L'Н'};
    }


    pbAir_1->setText(data->airplanes[0 + index_side].model);
    pbAir_2->setText(data->airplanes[1 + index_side].model);
    pbAir_3->setText(data->airplanes[2 + index_side].model);

}

void ChooseAirplane::updateLabelInfo(int i)
{
    int index_side;
    if(m_side == ENEMY)
        index_side = 3;
    else
        index_side = 0;
    labelModel->setText(QString("Модель: %1").arg(data->airplanes[i + index_side].model));
    labelInfo->setText(QString("Модель %1, %2(км/ч), %3(м)").arg(data->airplanes.at(i + index_side).model)
                       .arg(data->airplanes.at(i + index_side).speed)
                       .arg(data->airplanes.at(i + index_side).radiusTurn));
    speedAir->setValue(data->airplanes.at(i + index_side).speed);
    radiusAir->setValue(data->airplanes.at(i + index_side).radiusTurn);
    genAirplane.model = data->airplanes[i + index_side].model;
    genAirplane.speed = data->airplanes[i + index_side].speed;
    genAirplane.radiusTurn = data->airplanes[i + index_side].radiusTurn;

}

void ChooseAirplane::acceptButton()
{
    if(m_side == ENEMY) {
        data->lastEnemyAir.model = genAirplane.model;
        data->lastEnemyAir.speed = genAirplane.speed;
        data->lastEnemyAir.radiusTurn = genAirplane.radiusTurn;
    } else {
        data->lastRussiaAir.model = genAirplane.model;
        data->lastRussiaAir.speed = genAirplane.speed;
        data->lastRussiaAir.radiusTurn = genAirplane.radiusTurn;
    }
    accept();
}

void ChooseAirplane::setSide()
{

}

QChar ChooseAirplane::getGenGlasChar()
{
    srand(QDateTime::currentMSecsSinceEpoch());
    return listGlasChar.at(rand() % listGlasChar.size());

}

QChar ChooseAirplane::getGenSoglasChar()
{
    srand(QDateTime::currentMSecsSinceEpoch());
    return listSoglChar.at(rand() % listSoglChar.size());
}


