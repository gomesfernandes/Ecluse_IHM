#include "porte.h"

Porte::Porte(QObject * parent):
    QThread(parent)
{

}

Porte::~Porte() {
    transition->stop();
    intervalles->stop();
    while (transition->isActive()) {
        sleep(1);
    }
    while (intervalles->isActive()) {
        sleep(1);
    }
    delete transition;
    delete intervalles;
}

void Porte::run() {
    transition->setSingleShot(true);
    connect(transition,SIGNAL(timeout()),this,SLOT(finTransition()));
    connect(intervalles,SIGNAL(timeout()),this,SLOT(updateTransition()));
}

void Porte::updateTransition(){
    //emit position();
    emit etatCourant(etat);
}

void Porte::finTransition() {
    intervalles->stop();
    if (etat == ETAT_EN_FERMETURE)
        etat = ETAT_FERME;
    else
        etat = ETAT_OUVERT;
    emit etatCourant(etat);
}


void Porte::ouverture(){
    if (etat != ETAT_ALARME) {
        etat = ETAT_EN_OUVERTURE;
        transition->start(10000);
        intervalles->start(1000);
    }
}

void Porte::fermeture(){
    if (etat != ETAT_ALARME) {
        etat = ETAT_EN_FERMETURE;
        transition->start(10000);
        intervalles->start(1000);
    }
}

void Porte::arret(){
    if (etat != ETAT_ALARME) {
        etat = ETAT_ARRETE;
    }
    emit etatCourant(etat);
}

void Porte::urgence() {
    etat = ETAT_ALARME;
}

void Porte::panne(){
    etat = ETAT_ALARME;
    emit alarme();
}
void Porte::finAlarme(){
    etat = ETAT_ARRETE;
}
