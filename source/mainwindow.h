#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "scene.h"
#include "graphicsviewzoom.h"
#include "dialog.h"
#include "mylistwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void removeItemFromDialog(QString text);
    void clearDialogList();
private slots:

    void statusBarShowPos();
    void openTextDialog(const QRectF &_rect);
    void closeTextDialog();
    void switchConnects();
    void on_actionOpenImage_triggered();
    void on_actionAddObjects_triggered();
    void on_actionDrawRect_triggered();
    void on_actionDelete_triggered();


    void on_myListWidget_itemPressed(QListWidgetItem *item);

    void on_pushButtonUnselect_clicked();
    void on_pushButtonDelete_clicked();
    void on_pushButtonEdit_clicked();
    void on_actionSave_triggered();

    void on_actionSettings_triggered();

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    Scene *scene;
    GraphicsViewZoom *zoom;
    Dialog *textDialog;
    int lastIndex = 0;

};
#endif // MAINWINDOW_H
