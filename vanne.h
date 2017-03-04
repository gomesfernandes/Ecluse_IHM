#ifndef VANNE_H
#define VANNE_H

#include <QThread>
#include "etats.h"

class Vanne : public QThread
{
    Q_OBJECT
public:
    Vanne(QObject * parent = 0);
    void run();


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
