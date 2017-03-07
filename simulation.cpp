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

void Simulation::debutIntervalle() {
    if (etat != ETAT_ALARME)  {
        qDebug() << "Lancement timer 2 sec" << endl;
        intervalle->start(2000);
    }
}

void Simulation::finIntervalle() {
     intervalle->stop();
     qDebug() << "Fin du timer" << endl;
     emit launch_simu();
}
