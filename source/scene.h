#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <QDebug>
class myGraphicsTextItem : public QGraphicsTextItem
{
    public:
        myGraphicsTextItem(const QString &text) :
            QGraphicsTextItem(text) { }
        void setColor(QColor _color) { color = _color; }
        ~myGraphicsTextItem() {}

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *o, QWidget *w) {
            painter->setBrush(color);
            painter->drawRect(boundingRect());
            QGraphicsTextItem::paint(painter, o, w);
        }
        QColor color;
};


struct Object{
  QGraphicsRectItem* rect;
  myGraphicsTextItem* text;
  QString name;
  QString precision;
} ;

class Scene : public QGraphicsScene
{
  Q_OBJECT
public:
  explicit Scene(QObject *parent = nullptr);
  ~Scene();
  void enableAddZone();
  //  void editZone();
  void deleteObject(int index);
  void clearAll();
  void createZonesFromFile(const QString text);
  void createPoint(QPointF point);
  void completeZone();
  QString text, msg;
  void setBackgroundImage(QString _filename);
  QVector<Object> objects;
public slots:
  void createZone(const int _index, const QRectF &rect, const QString &_name, const QString &_precision);
  void editZone(const int _index, const QRectF &rect, const QString &_name, const QString &_precision);
signals:
  void zoneCreatedFromFile(const QString &_name);
  void zoneAdded(Object &_object);
  void mousePosChanged();
  void openTextDialog(const QRectF &_rect);
  void editFinished(const int &_index, const QString &_name);
  void leftMousePressed();
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void drawBackground(QPainter *painter, const QRectF &rect) override;
private:
  QVector<QGraphicsEllipseItem *> ellipses;
  QVector<QGraphicsPolygonItem *> polygons;
  QVector<QGraphicsLineItem *> lines;
  QVector<QPointF> points;
  QVector<QVector<QPointF> > Zones;


  QGraphicsLineItem *lineItem;
  QGraphicsLineItem *lineItemPrev,
  *lineItemNext;
  QGraphicsRectItem *rectItem;
  qreal scale = 10;
  bool mouseEnable   = false,
  zoneCompleted = false,
  pointsCreated = false,
  creatingLine  = false;
//  int indexToChangeZone,
//  indexForDragEllipse;
  QPointF newPoint;
  QString fileName;
  QColor zoneColor = Qt::darkGreen, highlightColor = Qt::darkYellow;
};

#endif // SCENE_H
