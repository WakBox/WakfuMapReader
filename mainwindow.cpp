#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    BinaryReader reader(_file);
    qint8 type = reader.readByte();

    qDebug() << "Map type:" << type;

    switch (type)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        qDebug() << "[Error] Map type unkown!";
        break;
    }

    ui->statusBar->showMessage("Reading header file...");
}
