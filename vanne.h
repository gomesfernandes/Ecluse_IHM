/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

#ifndef VANNE_H
#define VANNE_H

#include <QThread>
#include "constantes.h"

class Vanne : public QThread
{
    Q_OBJECT
public:
    Vanne(QObject * parent = 0);
    void run();
    int getEtat();
    void setEtat(int e);

public slots:
    void urgence();
    void panne();
    void ouverture();
    void fermeture();
    void finAlarme();

signals:
    void etatCourant(int etat);
    void alarme();

private:
    int etat;
};

#endif // VANNE_H
