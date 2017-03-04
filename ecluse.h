#ifndef ECLUSE_H
#define ECLUSE_H

#include <QMainWindow>
#include <QAbstractButton>
#include "loginwindow.h"
#include "signallumineux.h"
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

private slots:
    void changerFeu(QAbstractButton* f);

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
};

#endif // ECLUSE_H
