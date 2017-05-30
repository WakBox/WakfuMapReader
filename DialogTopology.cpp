#include "DialogTopology.h"
#include "ui_DialogTopology.h"

DialogTopology::DialogTopology(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTopology)
{
    ui->setupUi(this);
}

DialogTopology::~DialogTopology()
{
    delete ui;
}

void DialogTopology::updateTopology(QImage topology)
{
    ui->image->setPixmap(QPixmap::fromImage(topology));
}
