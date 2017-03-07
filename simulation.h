/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *
 *                                                                         *
 ***************************************************************************/

#ifndef SIMULATION_H
#define SIMULATION_H

#include <QThread>
#include <QTimer>
#include "constantes.h"

class Simulation: public QThread
{
    Q_OBJECT
public:
    Simulation(QObject * parent = 0);
    ~Simulation();
    void run();

public slots:
    void debutIntervalle(int etat);
    void finIntervalle();

signals:
    void launch_simu();
    void sortie_bateau_sas();
    void bateau_out();
    void final();
private:
    int etat;
    QTimer * intervalle;
};

#endif // SIMULATION_H
