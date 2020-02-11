#include "scribblearea.h"

#include <QMouseEvent>
#include <QPainter>


ScribbleArea::ScribbleArea(QWidget *parent)
    : QLabel(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setPenWidth(4);
    setPenColor(QColor(255, 0, 0));

}

bool ScribbleArea::openImage(QImage &imageMap)
{
    image = imageMap;
    modified = false;
    update();
    return true;
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::setCoord(Coord *cd)
{
    coord = cd;
}

QImage ScribbleArea::setPoint(QString a_str, int x, int y, int a_widthPen, QColor a_color)
{
    QPainter painter(&image);
    QPoint point = QPoint(x, y);
    painter.setPen(QPen(a_color, a_widthPen, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawPoint(point);
    painter.drawText(point, a_str);
    update();
    return image;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

QImage ScribbleArea::loadDivision(QVector<QPoint> points)
{

    if(points.isEmpty())
        return QImage();
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    for(int i = 0; i < points.size() - 1; i++) {
        painter.drawLine(points[i], points[i + 1]);

    }
    update();
    return image;
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling) {
        moveCursor = true;
        drawLineTo(event->pos());
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling && moveCursor == true) {
        drawLineTo(event->pos());
        scribbling = false;
        moveCursor = false;
    } else {
        QPoint p = event->pos();
        p.setX(p.x() + coord->x);
        p.setY(p.y() + coord->y);

        qDebug() << "event pos = " << p;
        QFile division("division.pos");
        if (division.open(QFile::WriteOnly | QFile::Text | QFile::Append)) {
                QString str = QString::number(p.x()) + ":" + QString::number(p.y()) + "\n";
                division.write(str.toUtf8());
                division.close();

                QPainter painter(&image);
                painter.setPen(QPen(QColor(0, 0, 255), 5, Qt::SolidLine, Qt::RoundCap,
                                    Qt::RoundJoin));
                painter.drawPoint(event->pos());
                update();

        }
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}



