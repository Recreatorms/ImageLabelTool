#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);
  setWindowTitle("Добавить новую индикацию..");
  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
  ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  addDefaultClasses();

}

Dialog::~Dialog()
{
  delete ui;
}
void Dialog::addDefaultClasses()
{
  ui->listWidget->addItem("Plane_1");
  ui->listWidget->addItem("Plane_2");
  ui->listWidget->addItem("Tank");
  ui->listWidget->addItem("Cannon");
  ui->listWidget->addItem("Ship");
}
void Dialog::addName(const QString &_name)
{
  if (ui->listWidget->findItems(_name, Qt::MatchFlag::MatchExactly).isEmpty())
    ui->listWidget->addItem(_name);
}

void Dialog::on_buttonCancel_clicked()
{
  emit close();
  ui->lineEdit->clear();
  ui->lineEdit_2->clear();
}

void Dialog::on_buttonOK_clicked()
{
  QString name = ui->lineEdit->text();
  std::string precision = ui->lineEdit_2->text().toStdString();
  if (name.isEmpty() && precision.empty()) {
      // QDialog

      return;
    }
  if (!std::any_of(precision.begin(),precision.end(), ::isdigit)) {
      // QDiagog
      return;
    }
  emit textOK(index, rect, name, QString::fromStdString(precision));
  on_buttonCancel_clicked();
  if (ui->listWidget->findItems(name, Qt::MatchFlag::MatchExactly).isEmpty())
    ui->listWidget->addItem(name);

}


void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
  ui->lineEdit->setText(item->text());
}

void Dialog::removeItemFromList(const QString &_name)
{
  if (ui->listWidget->count() == 0)
    return;
  if (!ui->listWidget->findItems(_name, Qt::MatchFlag::MatchExactly).isEmpty() && _name != "Plane_1" && _name != "Plane_1" && _name != "Ship" && _name != "Tank" && _name != "Cannon" )
    ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->findItems(_name, Qt::MatchFlag::MatchExactly).back()));

}

void Dialog::clearList()
{
  ui->listWidget->clear();
  addDefaultClasses();
}

