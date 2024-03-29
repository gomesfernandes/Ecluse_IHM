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
#include <QKeyEvent>
#include "loginwindow.h"
#include "signallumineux.h"
#include "constantes.h"
#include "porte.h"
#include "vanne.h"
#include "simulation.h"


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
    void niveauAtteint_ouvrirPortes();
    void niveauAtteint();

    void bateau_sas();
    void bateau_dehors();
    void reset_simulation();
    void over();

private slots:
    void on_btnEntrerAval_clicked();
    void on_btnEntrerAmont_clicked();
    void on_boutonArretUrgence_clicked();
    void on_voyantAlarme_clicked();
    void on_ouvrirVanneAval_clicked();
    void on_ouvrirVanneAmont_clicked();
    void on_fermerVanneAmont_clicked();
    void on_fermerVanneAval_clicked();
    void on_ouvrirPorteAval_clicked();
    void on_fermerPorteAval_clicked();
    void on_arreterPorteAval_clicked();
    void on_ouvrirPorteAmont_clicked();
    void on_fermerPorteAmont_clicked();
    void on_arreterPorteAmont_clicked();
    void on_actionAuthentifier_triggered();
    void on_btnSortirSas_clicked();
    void on_actionQuitter_l_application_triggered();
    void on_actionLancer_la_simulation_triggered();
    void keyPressEvent(QKeyEvent *event);


signals:
   void ouvrirPorteAval();
   void ouvrirPorteAmont();
   void fermerPorteAval();
   void fermerPorteAmont();
   void arreterPorteAval();
   void arreterPorteAmont();
   void ouvrirVanneAval();
   void ouvrirVanneAmont();
   void fermerVanneAval();
   void fermerVanneAmont();
   void arretUrgence();
   void finAlarme();
   void init_simulation();
   void lancer_simulation();
   void continuer_simu();
   void init_simulation(int);
   void finir_simu();

private:
    void setSignauxEnabled(bool visibilite);
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
    int compteurPorteAval;
    int compteurPorteAmont;
    int anglePorteAval;
    int anglePorteAmont;
    int niveau;
   // QTimer * niveau_timer;
    int mode;
    Simulation * simonlation;
    bool boolsimu;
    int compteur_rotation;
};

#endif // ECLUSE_H
