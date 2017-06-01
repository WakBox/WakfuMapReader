#include "DialogTopology.h"
#include "ui_DialogTopology.h"

DialogTopology::DialogTopology(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTopology)
{
    ui->setupUi(this);

    connect(ui->close, SIGNAL(clicked(bool)), this, SLOT(accept()));
}

DialogTopology::~DialogTopology()
{
    delete ui;
}

void DialogTopology::updateTopology(QImage topology)
{
    ui->topology->setPixmap(QPixmap::fromImage(topology));
    show();
}
