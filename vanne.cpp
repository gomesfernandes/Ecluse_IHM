/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

#include "vanne.h"

Vanne::Vanne(QObject * parent):
    QThread(parent),
    etat(ETAT_FERME)
{
}

/**
 * @brief Lancement du thread.
*/
void Vanne::run() {
}

/**
 * @brief Slot appelé lorsqu'on souhaite ouvrir la vanne. L'état passe en "ouvert"
 * et sera signalé.
*/
void Vanne::ouverture(){
    if (etat != ETAT_ALARME) {
        etat = ETAT_OUVERT;
    }
    emit etatCourant(etat);
}

/**
 * @brief Slot appelé lorsqu'on souhaite fermer la vanne. L'état passe en "fermé"
 * et sera signalé.
*/
void Vanne::fermeture(){
    if (etat != ETAT_ALARME) {
        etat = ETAT_FERME;
    }
    emit etatCourant(etat);
}

/**
 * @brief Slot appelé lors d'une urgence. L'état passe en "alarme".
*/
void Vanne::urgence() {
    etat = ETAT_ALARME;
}

/**
 * @brief Lors d'une panne, l'état passe en "alarme" et on signale la panne.
*/
void Vanne::panne(){
    etat = ETAT_ALARME;
    emit alarme();
}

/**
 * @brief L'alarme, qui avait déclencé la fermeture de la vanne, est terminé.
 * La vanne passe en état "fermé".
*/
void Vanne::finAlarme(){
    etat = ETAT_FERME;
}

int Vanne::getEtat() { return etat;}
