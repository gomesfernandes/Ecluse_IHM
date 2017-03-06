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
    sas_occupe(false),
    compteurPorteAval(10),
    compteurPorteAmont(10),
    anglePorteAval(0),
    anglePorteAmont(0)
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
 * @brief Vérifie le sens -> Amont et envoie un signal à la porte aval pour s'ouvrir.
 * 
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
 * @brief Même chose que pour l'aval mais en partant de l'amont
 */
void Ecluse::on_btnEntrerAmont_clicked() {
    sens = (ui->sensAval->isChecked()) ? SENS_AMONT : SENS_AVAL;
    qDebug() << "entrée en sens amont -> " << endl;
    if (sas_occupe) {
        qDebug() << "sas occupé..." << endl;

    } else {
        qDebug() << "sas libre, ouverture de la porte..." << endl;
        ui->statusBar->showMessage("Etat actuel: Ouverture de la porte aval");
        emit ouvrirPorteAmont();
    }
}

/**
 * @brief Slot appelé lors du signal émis par la porte aval, qui précise son état
 * actuel. Si la porte est ouverte, on change le message du status
 * bar et la couleur des signaux lumineux.
 * @param etat L'état actuel de la porte aval.
 */
void Ecluse::changementEtatPorteAval(int etat) {
    qDebug() << "etat de la porte " << etat;
    if (etat == ETAT_EN_OUVERTURE) {
        ui->statusBar->showMessage("Temps attente estimé 10 secondes. Temps restant : "+QString::number(compteurPorteAval));
        compteurPorteAval--;
    }
    if (etat == ETAT_OUVERT) {
        compteurPorteAval=10;
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
    if (etat == ETAT_EN_OUVERTURE) {
        ui->statusBar->showMessage("Temps attente estimé 10 secondes. Temps restant : "+QString::number(compteurPorteAval));
        compteurPorteAmont--;
    }
    if (etat == ETAT_OUVERT) {
        compteurPorteAmont=10;
        sas_occupe = (sas_occupe) ? false : true;
        ui->statusBar->showMessage("Etat actuel: Passage par porte amont libre");
        ui->vertEntrer_Amont->setChecked(true);
        emit ui->signalEntreeAmont->buttonClicked(ui->vertEntrer_Amont);
    }
}

void Ecluse::on_btnSortirSas_clicked()
{
    sas_occupe = (sas_occupe) ? false : true;
    if (sens == SENS_AMONT)
    {
        // si la porte amont est pas ouverte, => on la ferme
            // si porte côté aval ouverte => on la ferme
            // si vanne côté aval ouverte => on la ferme
            // on équilibre les niveau d'eau entre coté AMONT et écluse
            // puis on ouvre la porte AMONT
            // et signal feu vert
        if()

    }
    else if (sens == SENS_AVAL)
    {
        // si la porte amont est pas ouverte, => on la ferme
            // si porte côté amont ouverte => on la ferme
            // si vanne côté amont ouverte => on la ferme
            // on équilibre les niveau d'eau entre coté AVAL et écluse
            // puis on ouvre la porte AVAL
            // et on met signal feu vert

    }
}
