#ifndef SIGNALLUMINEUX_H
#define SIGNALLUMINEUX_H

#include <QDebug>

class signalLumineux
{

public:
    signalLumineux();
    char getCouleur();

private:
    char couleur;
};

#endif // SIGNALLUMINEUX_H
