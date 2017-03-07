/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

#include "signallumineux.h"

signalLumineux::signalLumineux(QObject * parent) :
    QObject(parent),
    couleur(FEU_ROUGE)
{
}

char signalLumineux::getCouleur() { return couleur; }


void signalLumineux::changerFeu(QAbstractButton* f){
    QRadioButton * feu = qobject_cast<QRadioButton*>(f);
    couleur = (feu->objectName().left(1) == "r") ? FEU_ROUGE : FEU_VERT;
}
