#include "ecluse.h"
#include "ui_ecluse.h"

#include <QDebug>

int Ecluse::modeAuto = 1;
int Ecluse::modeManuel = 0;

Ecluse::Ecluse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ecluse),
    signalEntreeAmont(new signalLumineux()),
    signalSortieAmont(new signalLumineux()),
    signalEntreeAval(new signalLumineux()),
    signalSortieAval(new signalLumineux()),
    porteAval(new Porte()),
    porteAmont(new Porte()),
    vanneAval(new Vanne()),
    vanneAmont(new Vanne())
{
    ui->setupUi(this);

    // ouvrir fenêtre authentification
    l = new LoginWindow(this);
    l->show();
    connect(l,SIGNAL(loginAction(int)),this,SLOT(ouvertureFenetreEcluse(int)));

    connect(ui->signalEntreeAmont, SIGNAL(buttonClicked(QAbstractButton*)),
                this,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalEntreeAval, SIGNAL(buttonClicked(QAbstractButton*)),
                this,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalSortieAmont, SIGNAL(buttonClicked(QAbstractButton*)),
                this,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalSortieAval, SIGNAL(buttonClicked(QAbstractButton*)),
                this,SLOT(changerFeu(QAbstractButton*)));
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


void Ecluse::changerFeu(QAbstractButton* f){
    QButtonGroup * sender = qobject_cast<QButtonGroup*>(this->sender());
    QRadioButton * feu = qobject_cast<QRadioButton*>(f);
    qDebug() << "signal envoyé par" << sender->objectName() << endl;
    qDebug() << "feu active :" << feu->objectName() << endl;

    if (sender->objectName() == "signalEntreeAmont") {

    } else if (sender->objectName() == "signalSortieAmont") {

    } else if (sender->objectName() == "signalEntreeAval") {

    } else if (sender->objectName() == "signalSortieAval") {

    }
}

