#include "chooseairplane.h"
#include <QApplication>
#include <QDesktopWidget>

ChooseAirplane::ChooseAirplane(Data *a_data, SIDE a_side)
{
    m_side = a_side;
    data = a_data;
    data->lastZoneRLS = data->zoneRLS;
    data->lastPointsFlight = data->pointsFlight;
    data->lastImage = data->image;
    QVBoxLayout *vbltopDialog = new QVBoxLayout;
    this->setLayout(vbltopDialog);

    QHBoxLayout *hblLabelHeader = new QHBoxLayout;
    QLabel *headerLabel = new QLabel("Выбрать модель самолёта");
    headerLabel->setAlignment(Qt::AlignCenter);
    hblLabelHeader->addWidget(headerLabel);

    QHBoxLayout *hblHeader = new QHBoxLayout;
    QPushButton *btChoosFromReadyAir = new QPushButton("Выбрать из готовых вариантов самолётов");
    hblHeader->addWidget(btChoosFromReadyAir);
    QPushButton *btInputManualDataAir = new QPushButton("Ввести данные вручную");
    hblHeader->addWidget(btInputManualDataAir);
    QPushButton *btSetRandomDataAir = new QPushButton("Задать случайные данные");
    hblHeader->addWidget(btSetRandomDataAir);


    // --------------------------------------- Choose model airplane group
    QGroupBox *groupBox = new QGroupBox;
    groupBox->setVisible(true);
    QHBoxLayout *phboxAirplane = new QHBoxLayout;
    groupBox->setLayout(phboxAirplane);

    QPushButton *pbSu_27 = new QPushButton("Су-27");
    phboxAirplane->addWidget(pbSu_27);
    QPushButton *pbMig_29 = new QPushButton("Миг-29");
    phboxAirplane->addWidget(pbMig_29);
    QPushButton *pbYak_130 = new QPushButton("Як-130");
    phboxAirplane->addWidget(pbYak_130);
    QLineEdit *customModel = new QLineEdit;
    phboxAirplane->addWidget(customModel);
    customModel->setMaximumWidth(150);

    QHBoxLayout *hblSetSpeed = new QHBoxLayout;
    QLabel *labelSetSpeed = new QLabel("Скорость самолёта");
    labelSetSpeed->setAlignment(Qt::AlignCenter);
    hblSetSpeed->addWidget(labelSetSpeed);

    QHBoxLayout *hblSetSpeed_2 = new QHBoxLayout;
    QLabel *labelSpeedInfo = new QLabel("Скорость самолёта составляет:");
    labelSpeedInfo->setAlignment(Qt::AlignCenter);
    QSpinBox *speedAir = new QSpinBox;
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
    QSpinBox *radiusAir = new QSpinBox;
    radiusAir->setMaximumWidth(70);
    radiusAir->setMaximum(10000);
    radiusAir->setMinimum(10);
    radiusAir->setValue(100);
    QLabel *labelRadiusSuf = new QLabel("(м)");
    hblLabelRadius_2->addWidget(labelRadius_2);
    hblLabelRadius_2->addWidget(radiusAir);
    hblLabelRadius_2->addWidget(labelRadiusSuf);


    vbltopDialog->addLayout(hblLabelHeader);
    vbltopDialog->addLayout(hblHeader);
    vbltopDialog->addWidget(groupBox);
    vbltopDialog->addLayout(hblSetSpeed);
    vbltopDialog->addLayout(hblSetSpeed_2);
    vbltopDialog->addLayout(hblLabelRadius);
    vbltopDialog->addLayout(hblLabelRadius_2);


    this->setMaximumWidth(600);
    this->setMaximumHeight(800);
    QDesktopWidget *wgt = QApplication::desktop();
    this->move(wgt->screen()->rect().center() - this->rect().center());
}
