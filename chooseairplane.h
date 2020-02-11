#ifndef CHOOSEAIRPLANE_H
#define CHOOSEAIRPLANE_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QDateTime>
#include <QThread>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QJsonObject>
#include <QVector>
#include <QComboBox>
#include <QSpinBox>
#include <QPainter>
#include <QImage>
#include <QMessageBox>
#include <QGroupBox>
#include "global.h"

class ChooseAirplane : public QDialog
{
    Q_OBJECT
public:
    ChooseAirplane(Data *a_data, SIDE a_side);
    void init_airplane();
    QLabel *labelInfo;
    QPushButton *btChoosFromReadyAir;
    QPushButton *btInputManualDataAir;
    QPushButton *btSetRandomDataAir;
    QPushButton *pbAir_1;
    QPushButton *pbAir_2;
    QPushButton *pbAir_3;
    QSpinBox *speedAir;
    QSpinBox *radiusAir;
    QLineEdit *customModel;
    QLabel *labelModel;
    QList<QChar> listGlasChar;
    QList<QChar> listSoglChar;
    Airplane genAirplane;
private:
    void updateLabelInfo(int i);
    void setSide();
    SIDE m_side;
    Data *data;
    QChar getGenGlasChar();
    QChar getGenSoglasChar();
private slots:
    void acceptButton();
signals:
    void chooseAir(int i);
};

#endif // CHOOSEAIRPLANE_H
