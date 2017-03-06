#include "ecluse.h"
#include "ui_ecluse.h"

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

    // gérer le changement des feux
    connect(ui->signalEntreeAmont, SIGNAL(buttonClicked(QAbstractButton*)),
                signalEntreeAmont,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalEntreeAval, SIGNAL(buttonClicked(QAbstractButton*)),
                signalEntreeAval,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalSortieAmont, SIGNAL(buttonClicked(QAbstractButton*)),
                signalSortieAmont,SLOT(changerFeu(QAbstractButton*)));
    connect(ui->signalSortieAval, SIGNAL(buttonClicked(QAbstractButton*)),
                signalSortieAval,SLOT(changerFeu(QAbstractButton*)));

    // gérer les changements d'états des portes
    connect(this,SIGNAL(ouvrirPorteAval()),porteAval,SLOT(ouverture()));
    connect(this,SIGNAL(ouvrirPorteAmont()),porteAmont,SLOT(ouverture()));
    connect(porteAval,SIGNAL(etatCourant(int)),this,SLOT(changementEtatPorteAval(int)));
    connect(porteAmont,SIGNAL(etatCourant(int)),this,SLOT(changementEtatPorteAmont(int)));

    // lancement des threads
    porteAval->run();
    porteAmont->run();
    vanneAmont->run();
    vanneAval->run();
}

Ecluse::~Ecluse() {
    delete ui;
}

/**
 * @brief Choix du mode d'utilisation et ouverture de la fenêtre principale
 * @param mode Le mode d'ouverture
 */
void Ecluse::ouvertureFenetreEcluse(int mode) {
    this->show();
    l->hide();

    if(mode == MODE_AUTO)
    {
        qDebug() << "Ouverture en mode automatique"<< endl;
        for(int i=0 ; i < (ui->operationsPorteAval->count()) ; i++)
        {
            QWidget* widget1 = ui->operationsPorteAval->itemAt(i)->widget();
            QWidget* widget2= ui->operationsPorteAmont->itemAt(i)->widget();
            if(widget1 != NULL)
                widget1->setVisible(false);
            if(widget2 != NULL)
                widget2->setVisible(false);
        }
        for(int i=0 ; i < (ui->operationsVanneAval->count()) ; i++)
        {
            QWidget* widget3 = ui->operationsVanneAval->itemAt(i)->widget();
            QWidget* widget4= ui->operationsVanneAmont->itemAt(i)->widget();
            if(widget3 != NULL)
                widget3->setVisible(false);
            if(widget4 != NULL)
                widget4->setVisible(false);
        }

    }
    else if (mode == MODE_MANUEL)
    {
        qDebug() << "Ouverture en mode manuel"<< endl;
    }
}

/**
 * @brief Vérifie le sens et envoie un signal à la porte aval pour s'ouvrir.
 */
void Ecluse::on_btnEntrerAval_clicked() {
    sens = (ui->sensAmont->isChecked()) ? SENS_AMONT : SENS_AVAL;
    qDebug() << "entrée en sens amont -> " << endl;
    if (sas_occupe) {
        qDebug() << "sas occupé..." << endl;

    } else {
        qDebug() << "sas libre, ouverture de la porte..." << endl;
        ui->statusBar->showMessage("Etat actuel: Ouverture de la porte aval");
        emit ouvrirPorteAval();
    }
}

/**
 * @brief Slot appelé lors du signal émis par la porte aval, qui précise son état
 * actuel. Si la porte est ouverte, on change le message du status
 * bar et la couleur des signaux lumineux.
 * @param etat L'état actuel de la porte aval.
 */
void Ecluse::changementEtatPorteAval(int etat) {
    qDebug() << "etat de la porte " << etat << endl;
    if (etat == ETAT_OUVERT) {
        sas_occupe = (sas_occupe) ? false : true;
        ui->statusBar->showMessage("Etat actuel: Passage par porte aval libre");
        ui->vertEntrer_Aval->setChecked(true);
        emit ui->signalEntreeAval->buttonClicked(ui->vertEntrer_Aval);
    }
}

/**
 * @brief idem que changementEtatPorteAval(int etat) pour la porte Amont
 */
void Ecluse::changementEtatPorteAmont(int etat) {
    qDebug() << "etat de la porte " << etat << endl;
}
