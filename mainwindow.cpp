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
            OpenFile(filename);

        return;
    }
}

void MainWindow::OpenFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    _file = file.readAll();

    ReadHeader();
}

void MainWindow::ReadHeader()
{
    ui->statusBar->showMessage("Reading header file...");   
}
