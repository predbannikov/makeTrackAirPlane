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
#include <QVector>
#include <QComboBox>
#include <QSpinBox>
#include <QPainter>
#include <QImage>
#include <QGroupBox>
#include "global.h"

class ChooseAirplane : public QDialog
{
    Q_OBJECT
public:
    ChooseAirplane(Data *a_data, SIDE a_side);
private:
private:
    SIDE m_side;
    Data *data;
};

#endif // CHOOSEAIRPLANE_H
