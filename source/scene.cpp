#include "scene.h"
#include <QGraphicsView>
Scene::Scene(QObject *parent) :
  QGraphicsScene()
{
  scale = 10;
  rectItem = nullptr;
  lineItem = nullptr;

  setBackgroundImage("../IMAGES/11_ship.jpg");
}

Scene::~Scene()
{

}

void Scene::enableAddZone()
{
  msg = "\tAdding Object";
  mouseEnable = true;

}

void Scene::deleteObject(int index)
{
  objects.at(index).rect->~QGraphicsRectItem();
  objects.at(index).text->~myGraphicsTextItem();
  removeItem(objects.at(index).rect);
  removeItem(objects.at(index).text);
  objects.removeAt(index);
}

void Scene::clearAll()
{
  if(rectItem !=nullptr)
    if (rectItem->scene() != nullptr) {
        removeItem(rectItem);
        rectItem->~QGraphicsRectItem();
      }
  if(lineItem != nullptr)
    if (lineItem->scene() != nullptr) {
        removeItem(lineItem);
        lineItem->~QGraphicsLineItem();
      }

  if (!points.isEmpty())
    points.clear();
  if (!ellipses.isEmpty())
    ellipses.clear();
  if (!lines.isEmpty())
    lines.clear();
  while (!objects.isEmpty()) {
      objects.first().rect->~QGraphicsRectItem();
      objects.first().text->~myGraphicsTextItem();
      objects.pop_front();
    }

  while(this->items().size() > 0) {
      removeItem(items().first());
      this->items().first()->~QGraphicsItem();
    }
}

void Scene::createZonesFromFile(const QString text)
{
  QPointF center;
  qreal _width, _height;
  int name;
  qreal precision;

  QString _name, _precision;
  int i = 0;
  while(i < text.size()) {
      QString buff = "";
      while(i < text.size()  && text[i] != ' ') { // class

          buff.append(text[i]);
          i++;
        }
      if(buff != "") {
          std::string str = buff.toStdString();
          if (std::all_of(str.begin(), str.end(), ::isdigit))
            name = buff.toInt();
          else
            _name = buff;

        }
      buff = "";
      i++;
      while(i < text.size() && text[i] != ' ') { // point1 X
          buff +=text[i];
          i++;
        }
      if (buff != "")
        center.rx() = buff.toDouble();
      buff = "";
      i++;
      while(i < text.size() && text[i] != ' ') { // point1 Y
          buff +=text[i];
          i++;
        }
      if (buff != "")
        center.ry() = buff.toDouble();
      buff = "";
      i++;

      while(i < text.size() && text[i] != ' ') { // point2 X
          buff +=text[i];
          i++;
        }
      if (buff != "")
        _width = buff.toDouble();
      buff = "";
      i++;
      while(i < text.size() && text[i] != ' ') { // point2 Y
          buff +=text[i];
          i++;
        }
      if (buff != "")
        _height = buff.toDouble();
      buff = "";
      i++;

      while(i < text.size() && text[i] != ' ' && text[i] != '\n' && text[i] != '\r') { // precision
          buff +=text[i];
          i++;
        }
      if (buff != "")
        precision = buff.toDouble();
      i++;
      if (i < text.size())
        if (text[i] == '\n')
          i++;

      QPointF point1, point2;

      point1.setX(center.rx()-_width/2);
      point2.setX(center.rx()+_width/2);

      point1.setY(center.ry()+_height/2);
      point2.setY(center.ry()-_height/2);

      point1.setX(point1.rx()*sceneRect().width());
      point1.setY(point1.ry()*sceneRect().height());
      point2.setX(point2.rx()*sceneRect().width());
      point2.setY(point2.ry()*sceneRect().height());

      QRectF rect(point1,point2);

      switch (name) {
        case 1:
          _name = "Plane_1";
          break;
        case 2:
          _name = "Plane_2";
          break;
        case 3:
          _name = "Tank";
          break;
        case 4:
          _name = "Cannon";
          break;
        case 5:
          _name = "Ship";
          break;
        default:
          break;
        }
      _precision = QString::number(precision);
      createZone(-1, rect, _name, _precision);

      emit zoneCreatedFromFile(_name);
    }
}

void Scene::createPoint(QPointF point)
{
  if (points.contains(point))
    return;
  points.push_back(point);
  qreal radius = scale/10;
  QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(QRectF(QPointF(point.x()-radius, point.y()-radius), QPointF(point.x()+radius, point.y()+radius)));
  ellipse->setPen(QPen(Qt::white, scale/2));
  ellipse->setBrush(QBrush(Qt::NoBrush));
  ellipses.push_back(ellipse);
  addItem(ellipse);
}

void Scene::completeZone()
{

  for(int i = 0; i < ellipses.size();i++)
    this->removeItem(ellipses[i]);
  ellipses.clear();
  points.clear();
  for(int i = 0; i < lines.size(); i++)
    this->removeItem(lines[i]);
  lines.clear();
  removeItem(lineItem);

  emit openTextDialog(rectItem->rect());
  removeItem(rectItem);
}

void Scene::createZone(const int _index, const QRectF &_rect, const QString &_name, const QString &_precision)
{
  struct Object object;
  QRectF rect;
  if (_rect.topLeft().ry() > _rect.bottomLeft().ry())
    if (_rect.topLeft().rx() < _rect.bottomRight().rx() )
      rect = QRectF(_rect.bottomLeft(), _rect.topRight());
    else
      rect = QRectF(_rect.bottomRight(), _rect.topLeft());

  else
    if (_rect.topLeft().rx() < _rect.bottomRight().rx())
      rect = QRectF(_rect);
    else
      rect = QRectF(_rect.topRight(),_rect.bottomLeft());

  object.rect = new QGraphicsRectItem(rect);
  QColor color = zoneColor;
  object.rect->setBrush(Qt::NoBrush);
  QPen pen(color);
  pen.setWidthF(scale/5);
  pen.setCapStyle(Qt::PenCapStyle::SquareCap);
  pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
  object.rect->setPen(pen);
  this->addItem(object.rect);

  object.name = _name;
  object.precision = _precision;
  QString text = _name + " " + _precision;
  object.text = new myGraphicsTextItem(text);
  object.text->setColor(color);
  object.text->setDefaultTextColor(Qt::white);
  object.text->setPos(object.rect->boundingRect().topLeft() - QPointF(0,4*scale));
  QFont font("Helvetica", scale*2, 99);

  object.text->setFont(font);

  addItem(object.text);
  objects.push_back(object);
  emit zoneAdded(object);

  zoneCompleted = true;
  pointsCreated = false;
}

void Scene::editZone(const int _index, const QRectF &rect, const QString &_name, const QString &_precision)
{


  objects[_index].name = _name;
  objects[_index].precision = _precision;
  objects[_index].text->~myGraphicsTextItem();

  QString text = _name + " " + _precision;
  objects[_index].text = new myGraphicsTextItem(text);
  objects[_index].text->setDefaultTextColor(Qt::white);
  objects[_index].text->setPos(rect.topLeft() - QPointF(0,scale));
  objects[_index].text->setColor(highlightColor);
  QFont font("Helvetica", scale*2, 99);

  objects[_index].text->setFont(font);
  addItem(objects[_index].text);



  emit editFinished(_index, _name);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

  if (!mouseEnable)
    return;
  if (event->button() == Qt::RightButton) {
      for(int i = 0; i < ellipses.size();i++)
        this->removeItem(ellipses[i]);
      ellipses.clear();
      points.clear();
      for(int i = 0; i < lines.size(); i++)
        this->removeItem(lines[i]);
      lines.clear();
      removeItem(lineItem);
      removeItem(rectItem);

      mouseEnable = false;
      creatingLine = false;
      pointsCreated = false;
      msg = "";
      return;
    }
  if (event->button() != Qt::LeftButton)
    return;
  emit leftMousePressed();
  if (zoneCompleted)
    zoneCompleted = false;
  QPointF point = event->scenePos();
  createPoint(point);
  if (creatingLine) {
      completeZone();
      creatingLine = false;
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  qreal X = event->scenePos().x()/sceneRect().width(),
      Y = event->scenePos().y()/sceneRect().height();

  text = "X = " + QString::number(X) + " Y = " + QString::number(Y) + msg;
  emit mousePosChanged();
  if(!mouseEnable)
    return;

  if(rectItem !=nullptr)
    if (rectItem->scene() != nullptr) {
        removeItem(rectItem);
        rectItem->~QGraphicsRectItem();
      }
  if(lineItem != nullptr)
    if (lineItem->scene() != nullptr) {
        removeItem(lineItem);
        lineItem->~QGraphicsLineItem();
      }

  if(zoneCompleted)
    return;
  if(points.size() == 0)
    return;

  QLineF line = QLineF(points.back(),event->scenePos());


  rectItem = new QGraphicsRectItem(QRectF(points.back(),event->scenePos()));
  rectItem->setBrush(Qt::NoBrush);
  rectItem->setPen(QPen(Qt::white, scale/10, Qt::DashLine, Qt::RoundCap,Qt::RoundJoin));
  lineItem = new QGraphicsLineItem(line);
  lineItem->setPen(QPen(Qt::white, scale/10, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

  this->addItem(rectItem);
  this->addItem(lineItem);
  creatingLine = true;
}




void Scene::setBackgroundImage(QString _filename)
{
  this->fileName = _filename;
}


void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
  Q_UNUSED(rect)
  QPixmap backgroundPixmap = QPixmap(fileName);
  setSceneRect(0,0, backgroundPixmap.width(), backgroundPixmap.height());
  painter->drawPixmap(sceneRect(),backgroundPixmap, backgroundPixmap.rect());

}
