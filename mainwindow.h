#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ecluse.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ConnexionBtn_clicked();

private:
    Ui::MainWindow *ui;
    Ecluse e;
};

#endif // MAINWINDOW_H
