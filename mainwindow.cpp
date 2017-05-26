#include <QBitmap>

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
    /*QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    _filename = QFileInfo(filename).fileName();
    _file = file.readAll();*/

    qDebug() << "Reading" << filename;
    QBitmap bitmap(864, 5184);

    QuaZip archive(filename);
    archive.open(QuaZip::mdUnzip);

    QuaZipFile file(&archive);

    for (bool f = archive.goToFirstFile(); f; f = archive.goToNextFile())
    {
        QString filePath = archive.getCurrentFileName();

        if (filePath.contains("META") || filePath.contains("coord"))
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
        //tplg->print();

        // Generate map
        for (int y = 0; y < 18; y++)
        {
            for (int x = 0; x < 18; x++)
            {
                int offsetx = (mapx * 18) + x;
                int offsety = (mapy * 18) + y;

                /*bgCCellPathData bgc = GIRCellDataBuilder.BuildDataForCell(offsetx, offsety, parser);

                if (bgc != null)
                {
                    try
                    {
                        if (map.isBlocking(offsetx, offsety))
                        {
                            bitmap.SetPixel(offsetx + 431, offsety + 2592, Color.DarkGreen);
                        }
                        else
                        {
                            bitmap.SetPixel(offsetx + 431, offsety + 2592, Color.LightGreen);
                        }
                        //Console.WriteLine("[{0},{1},{2},{3}]({4})", offsetx, offsety, data.z, (map.isBlocking(offsetx, offsety) ? 1 : 0), instanceID);

                    }
                    catch (Exception)
                    {
                        bitmap.SetPixel(offsetx + 431, offsety + 2592, Color.White);
                        //Console.WriteLine("EMPTY CELL AT: {0},{1},{2}", offsetx, offsety, 0);
                    }
                }
                else
                {
                    bitmap.SetPixel(offsetx + 431, offsety + 2592, Color.Red);
                    //Console.WriteLine("EMPTY CELL AT: {0},{1},{2}", offsetx, offsety, 0);
                }*/
            }
        }

        delete tplg;
    }

    archive.close();
    bitmap.save(qApp->applicationDirPath() + "/" + filename.split("/").last().remove(".jar") + ".png", "PNG");

    //readTopology();
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
