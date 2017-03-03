#ifndef ECLUSE_H
#define ECLUSE_H

#include <QMainWindow>
#include "loginwindow.h"

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
    void stuff();

private:
    Ui::Ecluse *ui;
    LoginWindow * l;
};

#endif // ECLUSE_H
