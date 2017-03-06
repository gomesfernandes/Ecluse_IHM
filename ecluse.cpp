/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

#include "ecluse.h"
#include "ui_ecluse.h"

Ecluse::Ecluse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ecluse),
    signalEntreeAmont(new signalLumineux(this)),
    signalSortieAmont(new signalLumineux(this)),
    signalEntreeAval(new signalLumineux(this)),
    signalSortieAval(new signalLumineux(this)),
    porteAval(new Porte(this)),
    porteAmont(new Porte(this)),
    vanneAval(new Vanne(this)),
    vanneAmont(new Vanne(this)),
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

    // gérer la fermeture/ouverture des vannes
    connect(this,SIGNAL(ouvrirVanneAmont()),vanneAmont,SLOT(ouverture()));
    connect(this,SIGNAL(ouvrirVanneAval()),vanneAval,SLOT(ouverture()));
    connect(this,SIGNAL(fermerVanneAmont()),vanneAmont,SLOT(fermeture()));
    connect(this,SIGNAL(fermerVanneAval()),vanneAval,SLOT(fermeture()));
    connect(vanneAval,SIGNAL(etatCourant(int)),this,SLOT(changementEtatVanneAval(int)));
    connect(vanneAmont,SIGNAL(etatCourant(int)),this,SLOT(changementEtatVanneAmont(int)));

    //urgence
    connect(this,SIGNAL(arretUrgence()),porteAval,SLOT(urgence()));
    connect(this,SIGNAL(arretUrgence()),porteAmont,SLOT(urgence()));
    connect(this,SIGNAL(arretUrgence()),vanneAmont,SLOT(urgence()));
    connect(this,SIGNAL(arretUrgence()),vanneAval,SLOT(urgence()));
    connect(this,SIGNAL(finAlarme()),porteAval,SLOT(finAlarme()));
    connect(this,SIGNAL(finAlarme()),porteAmont,SLOT(finAlarme()));
    connect(this,SIGNAL(finAlarme()),vanneAmont,SLOT(finAlarme()));
    connect(this,SIGNAL(finAlarme()),vanneAval,SLOT(finAlarme()));

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
            QWidget* widget2 = ui->operationsPorteAmont->itemAt(i)->widget();
            if(widget1 != NULL)
                widget1->setVisible(false);
            if(widget2 != NULL)
                widget2->setVisible(false);
        }

        for(int i=0 ; i < (ui->operationsVanneAval->count()) ; i++)
        {
            QPushButton* widget3 = qobject_cast<QPushButton*>
                                (ui->operationsVanneAval->itemAt(i)->widget());
            QPushButton* widget4= qobject_cast<QPushButton*>
                                (ui->operationsVanneAmont->itemAt(i)->widget());
            if(widget3 != NULL)
                widget3->setVisible(false);
            if(widget4 != NULL)
                widget4->setVisible(false);
        }

        ui->voyantAlarme->setEnabled(false);
        ui->vertEntrer_Aval->setEnabled(false);
        ui->rougeEntrer_Aval->setEnabled(false);
        ui->vertEntrer_Amont->setEnabled(false);
        ui->rougeEntrer_Amont->setEnabled(false);
        ui->vertSortir_Aval->setEnabled(false);
        ui->rougeSortir_Aval->setEnabled(false);
        ui->vertSortir_Amont->setEnabled(false);
        ui->rougeSortir_Amont->setEnabled(false);

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
    sens = (ui->sensAaval->isChecked()) ? SENS_AMONT : SENS_AVAL;
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

/**
 * @brief Met toute l'écluse en état d'urgence.
 */
void Ecluse::on_boutonArretUrgence_clicked() {
    ui->statusBar->showMessage("Etat actuel: En arrêt d'urgence");
    emit arretUrgence();
}

/**
 * @brief Annule l'alarme.
 */
void Ecluse::on_voyantAlarme_clicked() {
    emit finAlarme();
}

/**
 * @brief Ouvre resp. ferme la vanne concernée.
 */
void Ecluse::on_ouvrirVanneAval_clicked() {
    emit ouvrirVanneAval();
}
void Ecluse::on_ouvrirVanneAmont_clicked() {
    emit ouvrirVanneAmont();
}
void Ecluse::on_fermerVanneAval_clicked(){
    emit fermerVanneAval();
}
void Ecluse::on_fermerVanneAmont_clicked() {
    emit fermerVanneAmont();
}

/**
 * @brief Slots notifiés des changements des vannes.
 */
void Ecluse::changementEtatVanneAval(int etat) {
    if (etat == ETAT_FERME) {
        QPixmap pixmap = QPixmap (":/images/vannefermee.png");
        ui->imageVanneAval->setPixmap(pixmap);
    } else if (etat == ETAT_OUVERT) {
        QPixmap pixmap = QPixmap (":/images/vanneouverte.png");
        ui->imageVanneAval->setPixmap(pixmap);
    }
    qDebug() << "etat de la vanne aval " << etat << endl;
}
void Ecluse::changementEtatVanneAmont(int etat) {
    if (etat == ETAT_FERME) {
        QPixmap pixmap = QPixmap (":/images/vannefermee.png");
        ui->imageVanneAmont->setPixmap(pixmap);
    } else if (etat == ETAT_OUVERT) {
        QPixmap pixmap = QPixmap (":/images/vanneouverte.png");
        ui->imageVanneAmont->setPixmap(pixmap);
    }
    qDebug() << "etat de la vanne amont " << etat << endl;
}
