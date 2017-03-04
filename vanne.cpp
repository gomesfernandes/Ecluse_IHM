#include "vanne.h"

Vanne::Vanne(QObject * parent):
    QThread(parent),
    etat(ETAT_FERME)
{
}

void Vanne::run() {
}

void Vanne::ouverture(){
    if (etat != ETAT_FERME) {
        etat = ETAT_OUVERT;
    }
    emit etatCourant(etat);
}
void Vanne::fermeture(){
    if (etat != ETAT_FERME) {
        etat = ETAT_FERME;
    }
    emit etatCourant(etat);
}

void Vanne::urgence() {
    etat = ETAT_ALARME;
}

void Vanne::panne(){
    etat = ETAT_ALARME;
    emit alarme();
}

void Vanne::finAlarme(){
    etat = ETAT_FERME;
}
