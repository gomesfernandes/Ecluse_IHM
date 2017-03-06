#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "constantes.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

signals:
    void loginAction(int mode);

private slots:
    void on_NonAuthBtn_clicked();
    void on_ConnextionBtn_clicked();

private:
    Ui::LoginWindow *ui;
    QMap<QString,QString> m;
};

#endif // LOGINWINDOW_H
