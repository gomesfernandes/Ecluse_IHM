#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMap>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Authentification");
    m.insert("caty","password");
    m.insert("simon","12345");
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_NonAuthBtn_clicked()
{
    emit loginAction(1);
}

void LoginWindow::on_ConnextionBtn_clicked()
{
    QString login = ui->loginUser->text();
    QString mdp = ui->MdpUser->text();
    if (m.contains(login))
        if (mdp == m.value(login))
                emit loginAction(0);
}
