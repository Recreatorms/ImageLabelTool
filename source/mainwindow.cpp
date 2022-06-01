#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

#include <QPixmap>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  scene = new Scene(this);

  zoom = new GraphicsViewZoom(ui->myView);
  ui->myView->setScene(scene);
  this->resize(1100,600);
  this->update();
  textDialog = new Dialog();

  ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
  connect(scene, &Scene::mousePosChanged, this, &MainWindow::statusBarShowPos);

  connect(scene, &Scene::openTextDialog, this, &MainWindow::openTextDialog);
  connect(textDialog, &Dialog::close, this, &MainWindow::closeTextDialog);
  connect(textDialog,&Dialog::textOK, scene, &Scene::createZone);

  connect(scene, &Scene::zoneAdded, ui->myListWidget, &MyListWidget::addObject);
  connect(scene, &Scene::editFinished, this, &MainWindow::switchConnects);
  connect(scene, &Scene::editFinished, ui->myListWidget, &MyListWidget::editObject);
  connect(this, &MainWindow::removeItemFromDialog, textDialog, &Dialog::removeItemFromList);
  connect(this, &MainWindow::clearDialogList, textDialog, &Dialog::clearList);
  connect(scene, &Scene::leftMousePressed, this, &MainWindow::on_pushButtonUnselect_clicked);
  connect(scene, &Scene::zoneCreatedFromFile, textDialog, &Dialog::addName);
}

MainWindow::~MainWindow()
{
  delete textDialog;
  delete ui;
}

void MainWindow::statusBarShowPos() {
  ui->statusBar->showMessage(scene->text);
}


void MainWindow::openTextDialog(const QRectF &_rect)
{
  textDialog->show();
  this->setEnabled(false);
  textDialog->rect = _rect;
}

void MainWindow::closeTextDialog()
{
  textDialog->hide();
  setEnabled(true);

}

void MainWindow::switchConnects()
{
    disconnect(textDialog,&Dialog::textOK, scene, &Scene::editZone);
    connect(textDialog,&Dialog::textOK, scene, &Scene::createZone);
}


void MainWindow::on_actionOpenImage_triggered()
{
  QString imagePath = QFileDialog::getOpenFileName(this, "Загрузить изображения",
                                                   "D:/Images/", "Изображения (*.png; *.jpg; *.jpeg; *.bmp;);;Все файлы(*)");
  if(imagePath != "") {
    scene->setBackgroundImage(imagePath);
    on_actionDelete_triggered();
    }
}

void MainWindow::on_actionAddObjects_triggered()
{
  QString filePath = QFileDialog::getOpenFileName(this, "Загрузить объекты",
                                                  "D:/", "Текстовый файл (*.txt;);;Все файлы(*)");
  if (filePath.isNull())
    return;
  QFile txtFile(filePath);
  QTextStream fstream(&txtFile);
  txtFile.open(QIODevice::ReadOnly);
  QString text = fstream.readAll();
  txtFile.close();
  on_actionDelete_triggered();
  scene->createZonesFromFile(text);

}

void MainWindow::on_actionSave_triggered()
{
  QFileDialog fileDialog;
  fileDialog.setDefaultSuffix("jpg");
  QString filePath = fileDialog.getSaveFileName(this, "Сохранение изображений",
                                                   QDir::homePath(), "Изображения (*.png;*.jpg;*.jpeg;*.tiff;*.bmp;);;Все файлы()");
  if (filePath.isNull())
    return;

  scene->clearSelection();
  QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  QPainter painter(&image);
  scene->render(&painter);
  image.save(filePath);

  QString text;
  for(int i = 0; i < scene->objects.size(); i++) {
      text+=scene->objects[i].name + " ";
      text+=QString::number(scene->objects[i].rect->rect().center().rx()/scene->sceneRect().width()) + " ";
      text+=QString::number(scene->objects[i].rect->rect().center().ry()/scene->sceneRect().height()) + " ";
      text+=QString::number(scene->objects[i].rect->rect().width()/scene->sceneRect().width()) + " ";
      text+=QString::number(scene->objects[i].rect->rect().height()/scene->sceneRect().height()) + " ";
      text+=scene->objects[i].precision + "\n";
    }
  if (filePath.isNull())
    return;
//  fileDialog.setDefaultSuffix("txt");

  filePath = filePath.split(".",QString::SkipEmptyParts).at(0);
  QFile txtFile(filePath+".txt");
  QTextStream fstream(&txtFile);
  txtFile.open(QIODevice::WriteOnly);
  fstream << text;
  txtFile.flush();
  txtFile.close();
}

void MainWindow::on_actionDrawRect_triggered()
{
  scene->enableAddZone();
  on_pushButtonUnselect_clicked();
}


void MainWindow::on_actionDelete_triggered()
{
  scene->clearAll();
  ui->myListWidget->clear();
  emit clearDialogList();
}


void MainWindow::on_myListWidget_itemPressed(QListWidgetItem *item)
{
  if (scene->objects.empty())
    return;
  QColor color(Qt::darkGreen);
  scene->objects.at(lastIndex).rect->setPen(QPen(color));
  scene->objects[lastIndex].text->setColor(color);
  int index = ui->myListWidget->currentIndex().row();
  lastIndex = index;
  color = Qt::darkYellow;
  scene->objects.at(index).rect->setPen(QPen(color));
  scene->objects[index].text->setColor(color);
  scene->update(scene->sceneRect());

}

void MainWindow::on_pushButtonUnselect_clicked()
{
  if (!scene->objects.isEmpty()) {
    QColor color(Qt::darkGreen);
    scene->objects[lastIndex].rect->setPen(QPen(color));
    scene->objects[lastIndex].text->setColor(color);
    ui->myListWidget->clearSelection();
    scene->update(scene->sceneRect());
  }
}


void MainWindow::on_pushButtonDelete_clicked()
{
    if (ui->myListWidget->selectedItems().isEmpty())
      return;
    lastIndex = 0;
    emit removeItemFromDialog(ui->myListWidget->currentItem()->text());
    int index = ui->myListWidget->currentIndex().row();
    scene->deleteObject(index);
    ui->myListWidget->takeItem(index);
    ui->myListWidget->clearSelection();
}


void MainWindow::on_pushButtonEdit_clicked()
{
  if (ui->myListWidget->selectedItems().isEmpty())
    return;
  disconnect(textDialog,&Dialog::textOK, scene, &Scene::createZone);
  connect(textDialog,&Dialog::textOK, scene, &Scene::editZone);


  textDialog->index = ui->myListWidget->currentIndex().row();;
  QRectF _rect = scene->objects.at(textDialog->index).rect->boundingRect();
  openTextDialog(_rect);



}

void MainWindow::closeEvent(QCloseEvent *event)
{

  closeTextDialog();
}


void MainWindow::on_actionSettings_triggered()
{
    textDialog->addDefaultClasses();
}

