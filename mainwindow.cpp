#include <QImage>

#include "JlCompress.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Topology/TopologyMap.h"
#include "Topology/TopologyMapA.h"
#include "Topology/TopologyMapB.h"
#include "Topology/TopologyMapBi.h"
#include "Topology/TopologyMapC.h"
#include "Topology/TopologyMapCi.h"
#include "Topology/TopologyMapDi.h"

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

        if (QFileInfo(filename).suffix() == "jar")
            openFile(filename);

        return;
    }
}

void MainWindow::openFile(QString filename)
{
    qDebug() << "Reading" << filename;
    QImage map(864, 1500, QImage::Format_RGB32);

    QuaZip archive(filename);
    archive.open(QuaZip::mdUnzip);

    QuaZipFile file(&archive);

    for (bool f = archive.goToFirstFile(); f; f = archive.goToNextFile())
    {
        QString filePath = archive.getCurrentFileName();

        if (filePath.contains("META") || filePath.contains("coord"))
            continue;

        // tmp
        if (filePath != "0_0")
            continue;

        file.open(QIODevice::ReadOnly);
        QByteArray ba = file.readAll();
        file.close();

        qDebug() << "==========";
        qDebug() << "File :" << filePath << " Size : " << ba.size();

        short mapx = filePath.split("_").at(0).toShort();
        short mapy = filePath.split("_").at(1).toShort();

        qDebug() << "X:" << mapx;
        qDebug() << "Y:" << mapy;

        BinaryReader* reader = new BinaryReader(ba);
        qint8 type = reader->readByte();

        qDebug() << "X:" << reader->readShort();
        qDebug() << "Y:" << reader->readShort();
        qDebug() << "Z:" << reader->readShort();

        qDebug() << "Map type:" << type;

        return;

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

        // Generate map
        for (int y = 0; y < 18; y++)
        {
            for (int x = 0; x < 18; x++)
            {
                int offsetx = (mapx * 18) + x;
                int offsety = (mapy * 18) + y;

                qint8 res = tplg->isCellBlocked(offsetx, offsety);

                if (res == 1)
                {
                    map.setPixelColor(offsetx + 431, offsety + 750, Qt::GlobalColor::darkGreen);
                    qDebug() << "[" << offsetx << "," << offsety << ",1]";
                }
                else if (res == 0)
                {
                    map.setPixelColor(offsetx + 431, offsety + 750, Qt::GlobalColor::green);
                    qDebug() << "[" << offsetx << "," << offsety << ",0]";
                }
                else if (res == -1)
                {
                    map.setPixelColor(offsetx + 431, offsety + 750, Qt::GlobalColor::gray);
                    qDebug() << "EMPTY CELL = ERROR";
                }

                return;
            }
        }

        delete tplg;
    }

    archive.close();

    QDir dir(QCoreApplication::applicationDirPath());

    #ifdef Q_OS_MAC
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    #endif

    map.save(dir.absolutePath() + "/" + filename.split("/").last().remove(".jar") + ".png", "PNG");
}
