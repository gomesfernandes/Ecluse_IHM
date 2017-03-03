#include "ecluse.h"
#include "ui_ecluse.h"

Ecluse::Ecluse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ecluse)
{
    ui->setupUi(this);
}

Ecluse::~Ecluse()
{
    delete ui;
}
