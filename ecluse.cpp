#include "ecluse.h"
#include "ui_ecluse.h"

Ecluse::Ecluse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ecluse)
{
    ui->setupUi(this);
    l = new LoginWindow(this);
    l->show();
    connect(l,SIGNAL(loginAction()),this,SLOT(stuff()));
}

Ecluse::~Ecluse()
{
    delete ui;
}


void Ecluse::stuff() {
    this->show();
    l->hide();
}
