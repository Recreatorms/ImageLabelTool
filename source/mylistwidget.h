#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>
#include "scene.h"

class MyListWidget : public QListWidget
{
  Q_OBJECT
public:
  explicit MyListWidget(QWidget *parent = nullptr);
  ~MyListWidget() override;
public slots:
  void addObject(Object &_object);
  void editObject(const int &_index, const QString &_name);
};

#endif // MYLISTWIDGET_H
