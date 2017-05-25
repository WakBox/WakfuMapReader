#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Topology/TopologyMap.h"
#include "Topology/TopologyMapA.h"
#include "Topology/TopologyMapB.h"
#include "Topology/TopologyMapBi.h"
#include "Topology/TopologyMapC.h"
#include "Topology/TopologyMapCi.h"
#include "Topology/TopologyMapD.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAcceptDrops(true);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        QString filename = url.toLocalFile();

        //if (QFileInfo(filename).suffix() == "bin")
            openFile(filename);

        return;
    }
}

void MainWindow::openFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    _filename = QFileInfo(filename).fileName();
    _file = file.readAll();

    readTopology();
}

void MainWindow::readTopology()
{
    short x = _filename.split("_").at(0).toShort();
    short y = _filename.split("_").at(1).toShort();

    qDebug() << "Reading" << _filename;
    qDebug() << "X:" << x;
    qDebug() << "Y:" << y;

    BinaryReader* reader = new BinaryReader(_file);
    qint8 type = reader->readByte();

    qDebug() << "Map type:" << type;

    TopologyMap* tplg = nullptr;

    switch (type)
    {
    case 0: tplg = new TopologyMapA(reader); break;
    case 1: tplg = new TopologyMapB(reader); break;
    case 2: tplg = new TopologyMapBi(reader); break;
    case 3: tplg = new TopologyMapC(reader); break;
    case 4: tplg = new TopologyMapCi(reader); break;
    case 5: tplg = new TopologyMapD(reader); break;
    default:
        qDebug() << "[Error] Map type unkown!";
        return;
    }

    tplg->read();
    tplg->print();

    delete tplg;
}
