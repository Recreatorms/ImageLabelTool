#include "mylistwidget.h"

MyListWidget::MyListWidget(QWidget *parent) :
  QListWidget(parent)
{
  setSelectionMode(QAbstractItemView::SingleSelection);
}

MyListWidget::~MyListWidget()
{

}

void MyListWidget::addObject(Object &_object)
{
  addItem(_object.name);
}

void MyListWidget::editObject(const int &_index, const QString &_name )
{
  this->item(_index)->setText(_name);
}
