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
    compteurPorteAval(9),
    compteurPorteAmont(9),
    anglePorteAval(0),
    anglePorteAmont(0),
    niveau(NIVEAU_MOYEN),
    niveau_timer(new QTimer(this)),
    mode(0)
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
    connect(this,SIGNAL(fermerPorteAval()),porteAval,SLOT(fermeture()));
    connect(this,SIGNAL(fermerPorteAmont()),porteAmont,SLOT(fermeture()));
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

    niveau_timer->setSingleShot(true);
    connect(niveau_timer,SIGNAL(timeout()),this,SLOT(niveauAtteint()));

    // lancement des threads
    porteAval->run();
    porteAmont->run();
    vanneAmont->run();
    vanneAval->run();
}

Ecluse::~Ecluse() {
    niveau_timer->stop();
    delete ui;
}

void Ecluse::on_actionAuthentifier_triggered() {
    l->show();
}

/**
 * @brief Choix du mode d'utilisation et ouverture de la fenêtre principale
 * @param mode Le mode d'ouverture
 */
void Ecluse::ouvertureFenetreEcluse(int mode) {
    this->show();
    l->hide();

    if (mode == MODE_AUTO) {

        // boutons portes non accessibles en mode auto
        for(int i=0 ; i < (ui->operationsPorteAval->count()) ; i++)
        {
            QWidget* widget1 = ui->operationsPorteAval->itemAt(i)->widget();
            QWidget* widget2 = ui->operationsPorteAmont->itemAt(i)->widget();
            if(widget1 != NULL)
                widget1->setVisible(false);
            if(widget2 != NULL)
                widget2->setVisible(false);
        }
        //boutons vannes non accessible en mode auto
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

        // feux non accessibles en mode automatique
        if(ui->rougeEntrer_Amont != NULL)
            ui->rougeEntrer_Amont->setEnabled(false);
        if(ui->vertEntrer_Amont != NULL)
            ui->vertEntrer_Amont->setEnabled(false);
        if(ui->rougeSortir_Amont != NULL)
            ui->rougeSortir_Amont->setEnabled(false);
        if(ui->vertSortir_Amont != NULL)
            ui->vertSortir_Amont->setEnabled(false);

        if(ui->rougeEntrer_Aval != NULL)
            ui->rougeEntrer_Aval->setEnabled(false);
        if(ui->vertEntrer_Aval != NULL)
            ui->vertEntrer_Aval->setEnabled(false);
        if(ui->rougeSortir_Aval != NULL)
            ui->rougeSortir_Aval->setEnabled(false);
        if(ui->vertSortir_Aval != NULL)
            ui->vertSortir_Aval->setEnabled(false);

        setSignauxVisibles(true);

    } else if (mode == MODE_MANUEL) {
        QList<QWidget *> liste = ui->centralWidget->findChildren<QWidget *>();
        foreach (QWidget * p, liste ){
            p->setVisible(true);
        }

        if(ui->btnEntrerAval != NULL)
            ui->btnEntrerAval->setEnabled(true);
        if(ui->btnEntrerAmont != NULL)
            ui->btnEntrerAmont->setEnabled(true);
        if(ui->btnSortirSas != NULL)
            ui->btnSortirSas->setEnabled(true);
        if(ui->sensAmont != NULL)
            ui->sensAmont->setEnabled(true);
        if(ui->sensAval != NULL)
            ui->sensAval->setEnabled(true);
        setSignauxVisibles(true);
    }
    this->mode = mode;
}

void Ecluse::setSignauxVisibles(bool visibilite) {
    ui->voyantAlarme->setEnabled(visibilite);
    ui->vertEntrer_Aval->setEnabled(visibilite);
    ui->rougeEntrer_Aval->setEnabled(visibilite);
    ui->vertEntrer_Amont->setEnabled(visibilite);
    ui->rougeEntrer_Amont->setEnabled(visibilite);
    ui->vertSortir_Aval->setEnabled(visibilite);
    ui->rougeSortir_Aval->setEnabled(visibilite);
    ui->vertSortir_Amont->setEnabled(visibilite);
    ui->rougeSortir_Amont->setEnabled(visibilite);
}

/**
 * @brief Vérifie le sens -> Amont et envoie un signal à la porte aval pour s'ouvrir.
 * 
 */
void Ecluse::on_btnEntrerAval_clicked() {
    sens = (ui->sensAmont->isChecked()) ? SENS_AMONT : SENS_AVAL;
    if (sens == SENS_AVAL) return;
    if (!sas_occupe) {
        if( niveau == NIVEAU_BAS ) {
            emit ouvrirPorteAval();
        } else if (niveau == NIVEAU_MOYEN) {
            //2 portes fermées et vannes ouvertes -> remplir le sas
            emit fermerVanneAmont();
            emit ouvrirVanneAval();
        } else {
            //autre porte ouverte
            emit fermerPorteAmont();
        }
    }
}

/**
 * @brief Même chose que pour l'aval mais en partant de l'amont
 */
void Ecluse::on_btnEntrerAmont_clicked() {
    sens = (ui->sensAmont->isChecked()) ? SENS_AMONT : SENS_AVAL;
    if (sens == SENS_AMONT) return;
    if (!sas_occupe) {
        if( niveau == NIVEAU_HAUT) {
            emit ouvrirPorteAmont();
        } else if (niveau == NIVEAU_MOYEN) {
            //2 portes fermées et vannes ouvertes -> remplir le sas
            emit fermerVanneAval();
            emit ouvrirVanneAmont();
        } else {
            //autre porte ouverte
            emit fermerPorteAval();
        }
    }
}

/**
 * @brief Slot appelé lors du signal émis par la porte aval, qui précise son état
 * actuel. Si la porte est ouverte, on change le message du status
 * bar et la couleur des signaux lumineux.
 * @param etat L'état actuel de la porte aval.
 */
void Ecluse::changementEtatPorteAval(int etat) {
    QPixmap porte_haut = QPixmap (":/images/porte2.png");
    QPixmap porte_bas = QPixmap (":/images/porte1.png");
    QPixmap porteouverte = QPixmap (":/images/porteouverte.png");
    switch (etat) {
    case ETAT_EN_OUVERTURE :
        ui->statusBar->showMessage("Ouverture de la porte aval. Temps d'attente "
                                   "estimé :" +QString::number(compteurPorteAval));
        compteurPorteAval--;
        break;
    case ETAT_FERME:
        ui->porteAval_Haut->setPixmap(porte_haut);
        ui->porteAval_Bas->setPixmap(porte_bas);
        compteurPorteAval = 9;
        if (sas_occupe) {
            emit ouvrirVanneAmont();
        }
        break;
    case ETAT_EN_FERMETURE:
        ui->statusBar->showMessage("Fermeture de la porte aval. Temps d'attente "
                                   "estimé :" +QString::number(compteurPorteAval));
        compteurPorteAval--;
        break;
    case ETAT_OUVERT:
        ui->porteAval_Haut->setPixmap(porteouverte);
        ui->porteAval_Bas->setPixmap(porteouverte);
        if (sas_occupe) {
            ui->rougeEntrer_Aval->setChecked(true);
            ui->vertSortir_Aval->setChecked(true);
            emit ui->signalEntreeAval->buttonClicked(ui->rougeEntrer_Aval);
            emit ui->signalSortieAval->buttonClicked(ui->vertSortir_Aval);
            sas_occupe = false;
        } else {
            ui->vertEntrer_Aval->setChecked(true);
            ui->rougeSortir_Aval->setChecked(true);
            emit ui->signalEntreeAval->buttonClicked(ui->vertEntrer_Aval);
            emit ui->signalSortieAval->buttonClicked(ui->rougeSortir_Aval);
            sas_occupe = true;
        }
        compteurPorteAval = 9;
        ui->statusBar->showMessage("Etat actuel: Passage par porte aval libre");
        break;
    }

}

/**
 * @brief idem que changementEtatPorteAval(int etat) pour la porte Amont
 */
void Ecluse::changementEtatPorteAmont(int etat) {
    QPixmap porte_haut = QPixmap (":/images/porte2.png");
    QPixmap porte_bas = QPixmap (":/images/porte1.png");
    QPixmap porteouverte = QPixmap (":/images/porteouverte.png");
    switch (etat) {
    case ETAT_EN_OUVERTURE :
        ui->statusBar->showMessage("Ouverture de la porte amont. Temps d'attente "
                        "estimé : "+QString::number(compteurPorteAmont));
        compteurPorteAmont--;
        break;
    case ETAT_FERME:
        ui->porteAmont_Haut->setPixmap(porte_haut);
        ui->porteAmont_Bas->setPixmap(porte_bas);
        if (sas_occupe) {
            emit ouvrirVanneAval();
        }
        compteurPorteAmont=9;
        break;
    case ETAT_EN_FERMETURE:
        ui->statusBar->showMessage("Fermeture de la porte amont. Temps d'attente "
                        "estimé : "+QString::number(compteurPorteAmont));
        compteurPorteAmont--;
        break;
    case ETAT_OUVERT:
        ui->porteAmont_Haut->setPixmap(porteouverte);
        ui->porteAmont_Bas->setPixmap(porteouverte);
        if (sas_occupe) {
            ui->rougeEntrer_Amont->setChecked(true);
            ui->vertSortir_Amont->setChecked(true);
            emit ui->signalEntreeAmont->buttonClicked(ui->rougeEntrer_Amont);
            emit ui->signalSortieAmont->buttonClicked(ui->vertSortir_Amont);
        } else {
            ui->vertEntrer_Amont->setChecked(true);
            ui->rougeSortir_Amont->setChecked(true);
            emit ui->signalEntreeAmont->buttonClicked(ui->vertEntrer_Amont);
            emit ui->signalSortieAmont->buttonClicked(ui->rougeSortir_Amont);
        }
        compteurPorteAmont=9;
        sas_occupe = (sas_occupe) ? false : true;
        ui->statusBar->showMessage("Etat actuel: Passage par porte amont libre");
        break;
    }
}

/**
 * @brief Bouton sortir :
 *
 * Si SENS == AMONT
 * // si la porte amont est pas ouverte, => on la ferme
            // si porte côté aval ouverte => on la ferme
            // si vanne côté aval ouverte => on la ferme
            // on équilibre les niveau d'eau entre coté AMONT et écluse
            // le niveau doit monter à HAUT
            // puis on ouvre la porte AMONT
            // et signal feu vert

   Si sens == SENS_AVAL)
 * // si la porte aval est pas ouverte, => on la ferme
            // si porte côté amont ouverte => on la ferme
            // si vanne côté amont ouverte => on la ferme
            // on équilibre les niveau d'eau entre coté AVAL et écluse
            // le niveau doit monter à BAS
            // puis on ouvre la porte AVAL
            // et on met signal feu vert
 */

void Ecluse::on_btnSortirSas_clicked()
{
    if(sas_occupe == true) {
        if (sens == SENS_AMONT)
        {
            ui->rougeEntrer_Aval->setChecked(true);
            emit ui->signalEntreeAval->buttonClicked(ui->rougeEntrer_Aval);
            ui->statusBar->showMessage("Sortie vers l'amont, fermeture des portes..");
            // FERMETURE DE LA PORTE
            emit fermerPorteAval();
        }
        else if (sens == SENS_AVAL)
        {
            ui->rougeEntrer_Amont->setChecked(true);
            emit ui->signalEntreeAmont->buttonClicked(ui->rougeEntrer_Amont);
            ui->statusBar->showMessage("Sortie vers l'aval, fermeture des portes..");
            // FERMETURE DE LA PORTE
            emit fermerPorteAmont();
        }
    }
}

/**
 * @brief Met toute l'écluse en état d'urgence.
 */
void Ecluse::on_boutonArretUrgence_clicked() {
    ui->statusBar->showMessage("Etat actuel: En arrêt d'urgence.");
    if(this->mode==MODE_AUTO)
    {
        if(ui->btnEntrerAval != NULL)
            ui->btnEntrerAval->setEnabled(false);
        if(ui->btnEntrerAmont != NULL)
            ui->btnEntrerAmont->setEnabled(false);
        if(ui->btnSortirSas != NULL)
            ui->btnSortirSas->setEnabled(false);
        if(ui->sensAmont != NULL)
            ui->sensAmont->setEnabled(false);
        if(ui->sensAval != NULL)
            ui->sensAval->setEnabled(false);
    }
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
    int attente = 0;
    if (etat == ETAT_FERME) {
        QPixmap pixmap = QPixmap (":/images/vannefermee.png");
        ui->imageVanneAval->setPixmap(pixmap);
    } else if (etat == ETAT_OUVERT) {
        QPixmap pixmap = QPixmap (":/images/vanneouverte.png");
        ui->imageVanneAval->setPixmap(pixmap);
        if (sas_occupe && sens == SENS_AVAL) {
            if (niveau == NIVEAU_MOYEN) {
                attente = 5000;
            } else if (niveau == NIVEAU_HAUT){
                attente = 10000;
            }
            niveau = NIVEAU_BAS; //niveau de l'eau à la fin
            ui->statusBar->showMessage("Etat actuel : diminution du niveau d'eau du sas");
            niveau_timer->start(attente);
        } else if (!sas_occupe && sens == SENS_AMONT) {
            if (niveau == NIVEAU_MOYEN) {
                attente = 5000;
            } else if (niveau == NIVEAU_HAUT) {
                attente = 10000;
            }
            niveau = NIVEAU_BAS;
            ui->statusBar->showMessage("Etat actuel : diminution du niveau d'eau du sas");
            niveau_timer->start(attente);
        }
    }
}
void Ecluse::changementEtatVanneAmont(int etat) {
    int attente = 0;
    if (etat == ETAT_FERME) {
        QPixmap pixmap = QPixmap (":/images/vannefermee.png");
        ui->imageVanneAmont->setPixmap(pixmap);
    } else if (etat == ETAT_OUVERT) {
        QPixmap pixmap = QPixmap (":/images/vanneouverte.png");
        ui->imageVanneAmont->setPixmap(pixmap);
        if (sas_occupe && sens == SENS_AMONT) {
            if (niveau == NIVEAU_MOYEN) {
                attente = 5000;
            } else if (niveau == NIVEAU_BAS) {
                attente = 10000;
            }
            niveau = NIVEAU_HAUT; //niveau de l'eau à la fin
            ui->statusBar->showMessage("Etat actuel : élevation du niveau d'eau du sas");
            niveau_timer->start(attente);
        } else if (!sas_occupe && sens == SENS_AVAL) {
            if (niveau == NIVEAU_MOYEN) {
                attente = 5000;
            } else if (niveau == NIVEAU_BAS) {
                attente = 10000;
            }
            niveau = NIVEAU_HAUT;
            ui->statusBar->showMessage("Etat actuel : élevation du niveau d'eau du sas");
            niveau_timer->start(attente);
        }
    }
}

void Ecluse::on_ouvrirPorteAval_clicked() {
    if (niveau != NIVEAU_BAS) return;
    emit ouvrirPorteAval();
}

void Ecluse::on_ouvrirPorteAmont_clicked() {
    if (niveau != NIVEAU_HAUT) return;
    emit ouvrirPorteAmont();
}

void Ecluse::on_fermerPorteAval_clicked() {
    ui->rougeEntrer_Aval->setChecked(true);
    ui->rougeSortir_Aval->setChecked(true);
    emit ui->signalEntreeAval->buttonClicked(ui->rougeEntrer_Aval);
    emit ui->signalSortieAval->buttonClicked(ui->rougeSortir_Aval);
    emit fermerPorteAval();
}
void Ecluse::on_fermerPorteAmont_clicked() {
    ui->rougeEntrer_Amont->setChecked(true);
    ui->rougeSortir_Amont->setChecked(true);
    emit ui->signalEntreeAmont->buttonClicked(ui->rougeEntrer_Amont);
    emit ui->signalSortieAmont->buttonClicked(ui->rougeSortir_Amont);
    emit fermerPorteAval();
}
void Ecluse::on_arreterPorteAval_clicked() {
    emit arreterPorteAval();
}
void Ecluse::on_arreterPorteAmont_clicked() {
    emit arreterPorteAval();
}

void Ecluse::niveauAtteint() {
    if (sas_occupe) {
        if (sens == SENS_AMONT && niveau == NIVEAU_HAUT) {
            emit ouvrirPorteAmont();
        } else if (sens == SENS_AVAL && niveau == NIVEAU_BAS) {
            emit ouvrirPorteAval();
        }
    } else {
        if (sens == SENS_AMONT && niveau == NIVEAU_BAS) {
            emit ouvrirPorteAval();
        } else if (sens == SENS_AVAL && niveau == NIVEAU_HAUT) {
            emit ouvrirPorteAmont();
        }
    }
}
