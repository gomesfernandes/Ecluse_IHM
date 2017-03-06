/***************************************************************************
 *                                                                         *
 *   Gomes Fernandes Caty, Hamery Simon                                    *
 *   L3 Informatique, S6 Printemps                                         *                                              *
 *                                                                         *
 ***************************************************************************/

#ifndef ECLUSE_H
#define ECLUSE_H

#include <QMainWindow>
#include <QAbstractButton>
#include "loginwindow.h"
#include "signallumineux.h"
#include "constantes.h"
#include "porte.h"
#include "vanne.h"

namespace Ui {
class Ecluse;
}

class Ecluse : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ecluse(QWidget *parent = 0);
    ~Ecluse();

public slots:
    void ouvertureFenetreEcluse(int mode);
    void changementEtatPorteAval(int etat);
    void changementEtatPorteAmont(int etat);
    void changementEtatVanneAval(int etat);
    void changementEtatVanneAmont(int etat);

private slots:
    void on_btnEntrerAval_clicked();
    void on_boutonArretUrgence_clicked();
    void on_voyantAlarme_clicked();
    void on_ouvrirVanneAval_clicked();
    void on_ouvrirVanneAmont_clicked();
    void on_fermerVanneAmont_clicked();
    void on_fermerVanneAval_clicked();

signals:
   void ouvrirPorteAval();
   void ouvrirPorteAmont();
   void ouvrirVanneAval();
   void ouvrirVanneAmont();
   void fermerVanneAval();
   void fermerVanneAmont();
   void arretUrgence();
   void finAlarme();

private:
    Ui::Ecluse *ui;
    LoginWindow * l;
    static int modeManuel;
    static int modeAuto;
    signalLumineux * signalEntreeAmont;
    signalLumineux * signalSortieAmont;
    signalLumineux * signalEntreeAval;
    signalLumineux * signalSortieAval;
    Porte * porteAval;
    Porte * porteAmont;
    Vanne * vanneAval;
    Vanne * vanneAmont;
    int sens;
    bool sas_occupe;
};

#endif // ECLUSE_H
