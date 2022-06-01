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
  ui->lineEdit_2->setInputMask("B.900000");

}

Dialog::~Dialog()
{
  delete ui;
}
void Dialog::addDefaultClasses()
{
  //  Можно так
//    QFile file(QFileDialog::getOpenFileName());
  // или так
  QFile file("./classes.txt");
  QTextStream fstream(&file);
  file.open(QIODevice::ReadOnly);
  QString text = fstream.readAll();
  file.close();
  int i = 0;
  while(i < text.size()) {
      QString buff = "";
      while(i < text.size()  && text[i] != ' ' && text[i] != '\n') { // class

          buff.append(text[i]);
          i++;
        }
      if(buff != "")
        if (ui->listWidget->findItems(buff, Qt::MatchFlag::MatchExactly).isEmpty())
          ui->listWidget->addItem(buff);
      i++;
    }
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
  if (ui->lineEdit_2->text().toDouble() < 0.0f || ui->lineEdit_2->text().toDouble() > 1.0)
    return;
  if (ui->lineEdit_2->text()[0] != '1') {
      if (ui->lineEdit_2->text()[0] != '0')
        return;
    }
  else
    ui->lineEdit_2->setText("1.000000");
  if (ui->lineEdit_2->text().toDouble() == 0)
    ui->lineEdit_2->setText("0.000000");
  std::string precision = ui->lineEdit_2->text().toStdString();
  if (name.isEmpty())
    return;
  if (precision.empty())
    return;

  emit textOK(index, rect, name, QString::fromStdString(precision));
  on_buttonCancel_clicked();
  if (ui->listWidget->findItems(name, Qt::MatchFlag::MatchExactly).isEmpty()) {
      ui->listWidget->addItem(name);
      QFile file("./classes.txt");
      QTextStream fstream(&file);
      if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
          fstream << name << '\n';
          file.flush();
          file.close();
        }
    }
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

