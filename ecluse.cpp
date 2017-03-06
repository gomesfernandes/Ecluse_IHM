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
    vanneAmont(new Vanne()),
    sens(SENS_AMONT),
    sas_occupe(false)
{
    ui->setupUi(this);

    // ouvrir fenêtre authentification
    l = new LoginWindow(this);
    l->show();
    connect(l,SIGNAL(loginAction(int)),this,SLOT(ouvertureFenetreEcluse(int)));

    connect(ui->signalEntreeAmont, SIGNAL(buttonClicked(QAbstractButton*)),
                signalEntreeAmont,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalEntreeAval, SIGNAL(buttonClicked(QAbstractButton*)),
                signalEntreeAval,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalSortieAmont, SIGNAL(buttonClicked(QAbstractButton*)),
                signalSortieAmont,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalSortieAval, SIGNAL(buttonClicked(QAbstractButton*)),
                signalSortieAval,SLOT(changerFeu(QAbstractButton*)));
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

void Ecluse::on_btnEntrerAval_clicked()
{
    sens = (ui->sensAmont->isChecked()) ? SENS_AMONT : SENS_AVAL;
    qDebug() << "entrée en sens amont -> " << endl;
    if (sas_occupe) {
        qDebug() << "sas occuee..." << endl;
    } else {
        qDebug() << "sas libre, ouverture de la porte..." << endl;
        //emit ouvrirPorteAval();
    }
}
