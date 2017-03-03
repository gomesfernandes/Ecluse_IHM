#ifndef ECLUSE_H
#define ECLUSE_H

#include <QMainWindow>

namespace Ui {
class Ecluse;
}

class Ecluse : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ecluse(QWidget *parent = 0);
    ~Ecluse();

private:
    Ui::Ecluse *ui;
};

#endif // ECLUSE_H
