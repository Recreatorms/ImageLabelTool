#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
namespace Ui {
  class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = nullptr);
  ~Dialog();
  QRectF rect;
  int index;
signals:
  void close();
  void textOK(const int _index, const QRectF &_rect, const QString &_name, const QString &_precision);
public slots:
  void addName(const QString &_name);
  void removeItemFromList(const QString &_name);
  void clearList();
  void addDefaultClasses();
private slots:
  void on_buttonCancel_clicked();
  void on_buttonOK_clicked();
  void on_listWidget_itemClicked(QListWidgetItem *item);

private:
  Ui::Dialog *ui;
};

#endif // DIALOG_H
