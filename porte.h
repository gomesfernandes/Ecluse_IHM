/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

#ifndef PORTE_H
#define PORTE_H

#include <QThread>
#include <QTimer>
#include "constantes.h"

class Porte : public QThread
{
    Q_OBJECT
public:
    Porte(QObject * parent = 0);
    ~Porte();
    void run();
    int getEtat();
    void setEtat(int e);

public slots:
    void ouverture();
    void fermeture();
    void arret();
    void urgence();
    void panne();
    void finAlarme();
    void finTransition();
    void updateTransition();

signals:
    void etatCourant(int etat);
    void position(int position);
    void alarme();

private:
    int etat;
    int pos;
    QTimer * transition;
    QTimer * intervalles;
};

#endif // PORTE_H
