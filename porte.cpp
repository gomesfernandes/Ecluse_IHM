/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

#include "porte.h"

Porte::Porte(QObject * parent):
    QThread(parent),
    etat(ETAT_FERME),
    transition(new QTimer(this)),
    intervalles(new QTimer(this))
{
    transition->setSingleShot(true);
    connect(transition,SIGNAL(timeout()),this,SLOT(finTransition()));
    connect(intervalles,SIGNAL(timeout()),this,SLOT(updateTransition()));
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
}

/**
 * @brief Lancement du thread.
*/
void Porte::run() {}

/**
 * @brief Slot appelé à chaque timeout du timer "intervalles" pour signaler
 * l'état courant et la position à la console.
 */
void Porte::updateTransition(){
    //emit position();
    emit etatCourant(etat);
}

/**
 * @brief Slot appelé après le timeout du timer "transition". La porte est
 * complètement ouverte ou fermée. Un signal est émis avec cet état.
 */
void Porte::finTransition() {
    intervalles->stop();
    if (etat == ETAT_EN_FERMETURE)
        etat = ETAT_FERME;
    else
        etat = ETAT_OUVERT;
    emit etatCourant(etat);
}

/**
 * @brief Slot appelé lorsqu'on souhaite ouvrir la porte. La transition dure 10s,
 * pendant laquelle la porte est en état "en ouverture" qui est émis chaque
 * seconde. Ne fait rien si on est en état d'alarme.
*/
void Porte::ouverture(){
    if ((etat != ETAT_ALARME)&& (etat != ETAT_OUVERT))  {
        etat = ETAT_EN_OUVERTURE;
        transition->start(10000);
        intervalles->start(1000);
        qDebug() << "ouverture de la porte" << endl;
    } else if (etat == ETAT_OUVERT)
        emit etatCourant(etat);
}

/**
 * @brief Idem que ouverture() pour la fermeture de la porte.
*/
void Porte::fermeture(){
    if ((etat != ETAT_ALARME)&& (etat != ETAT_FERME)) {
        etat = ETAT_EN_FERMETURE;
        transition->start(10000);
        intervalles->start(1000);
        qDebug() << " fermeture de la porte" << endl;
    } else if (etat == ETAT_FERME)
        emit etatCourant(etat);
}

/**
 * @brief Arrête la porte, et interrompt toute transition en arrêtant les timers.
 * L'état passe à "arrêt", et on signale cet état.
*/
void Porte::arret(){
    transition->stop();
    intervalles->stop();
    if (etat != ETAT_ALARME) {
        etat = ETAT_ARRETE;
    }
    emit etatCourant(etat);
}

/**
 * @brief Interromp toute transition, l'état passe à "alarme".
*/
void Porte::urgence() {
    transition->stop();
    intervalles->stop();
    etat = ETAT_ALARME;
}

/**
 * @brief Interromp toute transition, l'état passe à "alarme". On signale la
 * panne.
*/
void Porte::panne(){
    transition->stop();
    intervalles->stop();
    etat = ETAT_ALARME;
    emit alarme();
}

/**
 * @brief En fin d'alarme, la porte sera en état d'arrêt.
*/
void Porte::finAlarme(){
    etat = ETAT_ARRETE;
}
