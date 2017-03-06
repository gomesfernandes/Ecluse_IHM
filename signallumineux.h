#ifndef SIGNALLUMINEUX_H
#define SIGNALLUMINEUX_H

#include "constantes.h"
#include <QObject>
#include <QAbstractButton>
#include <QRadioButton>
#include <QButtonGroup>

class signalLumineux: public QObject
{
    Q_OBJECT
public:
    signalLumineux(QObject * parent = 0);
    char getCouleur();

public slots:
    void changerFeu(QAbstractButton* f);

private:
    char couleur;
};

#endif // SIGNALLUMINEUX_H
