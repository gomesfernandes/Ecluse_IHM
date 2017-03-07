/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *
 *                                                                         *
 ***************************************************************************/

#include "simulation.h"


Simulation::Simulation(QObject * parent):
    QThread(parent),
    etat(ETAT_INIT),
    intervalle(new QTimer(this))
{
    intervalle->setSingleShot(true);
    connect(intervalle,SIGNAL(timeout()),this,SLOT(finIntervalle()));
}

Simulation::~Simulation() {
    intervalle->stop();
    while (intervalle->isActive()) {
        sleep(1);
    }
}


/**
 * @brief Lancement du thread.
*/
void Simulation::run() {}

void Simulation::debutIntervalle(int etat) {
    if (etat != ETAT_ALARME)  {
        qDebug() << "Lancement timer 5 sec" << endl;
        intervalle->start(5000);
    }
    this->etat = etat;
}

void Simulation::finIntervalle() {
     intervalle->stop();
     qDebug() << "Fin du timer" << endl;
     if(etat == ETAT_INIT)
        emit launch_simu();
     else if(etat== ETAT_EN_SIMULATION)
         emit sortie_bateau_sas();
     else if(etat== ETAT_FIN_SIMULATION)
         emit bateau_out();
     else if (etat == OVER)
         emit final();
}
