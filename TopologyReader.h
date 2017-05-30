#ifndef TOPOLOGYREADER_H
#define TOPOLOGYREADER_H

#include <QtCore>
#include <QThread>
#include <QImage>

#include "JlCompress.h"
#include "Topology/TopologyMap.h"
#include "Topology/TopologyMapA.h"
#include "Topology/TopologyMapB.h"
#include "Topology/TopologyMapBi.h"
#include "Topology/TopologyMapC.h"
#include "Topology/TopologyMapCi.h"
#include "Topology/TopologyMapDi.h"

#include "DialogTopology.h"

class TopologyReader : public QThread
{
    Q_OBJECT

public:
    TopologyReader(QObject* parent = 0) : QThread(parent) {}
    void setPath(QString path) { _path = path; }

protected:
    virtual void run()
    {
        int width = 2000;
        int height = 2000;

        QImage topology(width, height, QImage::Format_RGB32);

        QuaZip archive(_path);
        archive.open(QuaZip::mdUnzip);
        int entries = archive.getEntriesCount();

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
                        topology.setPixelColor(offsetx + (width/2), offsety + (height/2), Qt::GlobalColor::darkGreen);
                        qDebug() << "[" << offsetx << "," << offsety << ",1]";
                    }
                    else if (res == 0)
                    {
                        topology.setPixelColor(offsetx + (width/2), offsety + (height/2), Qt::GlobalColor::green);
                        qDebug() << "[" << offsetx << "," << offsety << ",0]";
                    }
                    else if (res == -1)
                    {
                        topology.setPixelColor(offsetx + (width/2), offsety + (height/2), Qt::GlobalColor::gray);
                        qDebug() << "EMPTY CELL";
                    }
                }

                updateTopology(topology);
            }

            delete tplg;

            updateProgressBar(entries - 1, 1);
        }

        archive.close();

        QDir dir(QCoreApplication::applicationDirPath());

        #ifdef Q_OS_MAC
            dir.cdUp();
            dir.cdUp();
            dir.cdUp();
        #endif

        topology.save(dir.absolutePath() + "/" + _path.split("/").last().remove(".jar") + ".png", "PNG");

        emit done();
    }

signals:
    void updateProgressBar(int max, int value);
    void updateTopology(QImage topology);
    void done();

private:
    QString _path;
};

#endif // TOPOLOGYREADER_H
