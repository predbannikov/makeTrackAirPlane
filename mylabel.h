#ifndef MYLABEL_H
#define MYLABEL_H

#include <QObject>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QPoint>
#include <QEvent>


class mylabel : public QLabel
{
    Q_OBJECT
public:
    explicit mylabel(QLabel *parent = nullptr);
    void mouseReleaseEvent(QMouseEvent *ev) override;


signals:

};

#endif // MYLABEL_H
