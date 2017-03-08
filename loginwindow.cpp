/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

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
    m.insert("ihm","mdp");
    ui->MdpUser->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow() {
    delete ui;
}

void LoginWindow::on_NonAuthBtn_clicked() {
    emit loginAction(MODE_AUTO);
}

void LoginWindow::on_ConnextionBtn_clicked() {
    QString login = ui->loginUser->text();
    QString mdp = ui->MdpUser->text();
    if (m.contains(login))
        if (mdp == m.value(login))
                emit loginAction(MODE_MANUEL);
    ui->loginUser->setText("");
    ui->MdpUser->setText("");
}

void LoginWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    } else if(event->key() == Qt::Key_Control)
    {
        on_ConnextionBtn_clicked();
    } else if ( (event->key()==Qt::Key_Enter) || (event->key()==Qt::Key_Return) )
    {
        on_ConnextionBtn_clicked();
    }
}


