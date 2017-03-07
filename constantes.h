/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <QDebug>

const int MODE_AUTO = 0;
const int MODE_MANUEL = 1;

const int ETAT_OUVERT = 0;
const int ETAT_FERME = 1;
const int ETAT_ARRETE = 2;
const int ETAT_EN_OUVERTURE = 3;
const int ETAT_EN_FERMETURE = 4;
const int ETAT_ALARME = 5;
const int ETAT_NORMAL = 6;

const int SENS_AMONT = 0;
const int SENS_AVAL = 1;

const int NIVEAU_HAUT = 1;
const int NIVEAU_MOYEN = 0;
const int NIVEAU_BAS = -1;

const char FEU_ROUGE = 'r';
const char FEU_VERT = 'v';

//simulation
const int ETAT_INIT = 7 ;
const int ETAT_EN_SIMULATION = 8;
const int ETAT_FIN_SIMULATION = 9;


#endif // CONSTANTES_H
