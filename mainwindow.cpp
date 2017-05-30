#include <QImage>

#include "JlCompress.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "TopologyReader.h"
#include "DialogTopology.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);

    ui->maps->hide();
    ui->progressBar->hide();

    connect(ui->maps, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSelectMap(QString)));
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
        openWakfuFolder(url.toLocalFile());
        return;
    }
}

void MainWindow::openWakfuFolder(QString path)
{
    _wakfuPath = path;

    #ifdef Q_OS_MAC
    _wakfuPath += "/Contents/Data/Wakfu.app/Contents/Resources/contents/maps/";
    #endif

    QFileInfo fi(_wakfuPath);

    if (!fi.isDir())
    {
        ui->statusBar->showMessage("Error while reading Wakfu folder.");
        return;
    }

    QDir tplg(_wakfuPath + "tplg");
    tplg.setFilter(QDir::Files);
    tplg.setSorting(QDir::NoSort);

    QStringList list = tplg.entryList();

    QCollator collator;
    collator.setNumericMode(true);

    std::sort(list.begin(), list.end(), [&collator](const QString &file1, const QString &file2)
    {
        return collator.compare(file1, file2) < 0;
    });

    list.prepend("Please select a map to analyze");
    ui->maps->addItems(list);

    ui->label->hide();
    ui->maps->show();
}

void MainWindow::onSelectMap(QString map)
{
    if (!map.contains(".jar"))
        return;

    ui->progressBar->show();
    ui->progressBar->setValue(0);

    DialogTopology* dialogTopology = new DialogTopology(this);
    dialogTopology->show();

    TopologyReader* tplgThread = new TopologyReader(this);
    connect(tplgThread, SIGNAL(updateProgressBar(int,int)), this, SLOT(updateProgressBar(int,int)));
    connect(tplgThread, SIGNAL(updateTopology(QImage)), dialogTopology, SLOT(updateTopology(QImage)));
    connect(tplgThread, SIGNAL(finished()), tplgThread, SLOT(deleteLater()));

    tplgThread->setPath(_wakfuPath + "tplg/" + map);
    tplgThread->start();
}

void MainWindow::updateProgressBar(int max, int value)
{
    if (ui->progressBar->maximum() != max)
        ui->progressBar->setMaximum(max);

    ui->progressBar->setValue(ui->progressBar->value() + value);
}
