#include "ecluse.h"
#include "ui_ecluse.h"

#include <QDebug>

int Ecluse::modeAuto = 1;
int Ecluse::modeManuel = 0;

Ecluse::Ecluse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ecluse)
{
    ui->setupUi(this);
    l = new LoginWindow(this); // ouvrir fenêtre authentification
    l->show();
    connect(l,SIGNAL(loginAction(int)),this,SLOT(ouvertureFenetreEcluse(int)));
}

Ecluse::~Ecluse()
{
    delete ui;
}


void Ecluse::ouvertureFenetreEcluse(int mode) {
    this->show();
    l->hide();
    qDebug() << "Ouverture en mode " << mode << endl;
}
