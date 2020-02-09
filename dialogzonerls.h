#ifndef DIALOGZONERLS_H
#define DIALOGZONERLS_H

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

class DialogZoneRLS : public QDialog
{
    Q_OBJECT
public:
    DialogZoneRLS(Data *a_data, SIDE a_side);
    Data *data;
private:
    SIDE m_side;
    QSpinBox *sbCountRLS;
    QSpinBox *sbCountPointsFlight;
    QSpinBox *sbXCoordRLS;
    QSpinBox *sbYCoordRLS;
    QSpinBox *sbRCoordRLS;
    QSpinBox *sbXCoordFlight;
    QSpinBox *sbYCoordFlight;
    QComboBox *cmbChoiceRLS;
    QComboBox *cmbChoiceFlight;
    QLabel *imgLabel;
    QImage setPointRLS(QString a_str, int x, int y, int r, int a_widthPen, QColor a_color);
    QImage setPointsFlight(QString a_str, int x, int y, int a_widthPen, QColor a_color);
    void setCmbRLS();
    void setCmbFlight();
    bool checkPointRLS(ZoneRLS a_rls);
    bool checkPointFlight(QPoint a_point);
private slots:
    void acceptButton();
    void autoGenerateRLS();
    void autoGenerateFlight();
    void setCountRLS();
    void setPointRLS(ZoneRLS a_rls);
    void setPointFlight(QPoint a_point);
    void setDataPointsRLS();
    void setDataPointsFlight();
};

#endif // DIALOGZONERLS_H
