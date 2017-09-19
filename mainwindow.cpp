#include <QImage>

#include "JlCompress.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "TopologyReader.h"

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
        QString filename = url.toLocalFile();

        if (QFileInfo(filename).suffix() == "jar")
            openJarFile(filename);
        else
            openWakfuFolder(filename);

        return;
    }
}

void MainWindow::openJarFile(QString path)
{
    ui->progressBar->show();
    ui->progressBar->setValue(0);

    if (ui->comboBox->currentText() == "env")
    {
        QuaZip archive(path);
        archive.open(QuaZip::mdUnzip);

        QuaZipFile file(&archive);

        for (bool f = archive.goToFirstFile(); f; f = archive.goToNextFile())
        {
            QString filePath = archive.getCurrentFileName();

            if (filePath.contains("META"))
                continue;

            file.open(QIODevice::ReadOnly);
            QByteArray ba = file.readAll();
            file.close();

            if (filePath.contains("data"))
            {
                //coord = ba;
                continue;
            }

            short mapx = filePath.split("_").at(0).toShort();
            short mapy = filePath.split("_").at(1).toShort();

            qDebug() << "X:" << mapx;
            qDebug() << "Y:" << mapy;

            BinaryReader* r = new BinaryReader(ba);
            qint8 type = r->readByte();

            qDebug() << "Map type:" << type;

            qint16 m_x = r->readShort();
            qint16 m_y = r->readShort();

            // loadParticleData
            qint8 count = r->readByte();
            for (qint8 a = 0; a < count; ++a)
            {
                qint8 m_p_x = r->readByte();
                qint8 m_p_y = r->readByte();
                qint16 m_p_z = r->readShort();

                qint32 m_systemId = r->readInt();
                qint8 m_level = r->readByte();
                qint8 m_offsetX = r->readByte();
                qint8 m_offsetY = r->readByte();
                qint8 m_offsetZ = r->readByte();
                qint8 m_lod = r->readByte();
            }

            // loadSoundData
            count = r->readByte();
            for (qint8 a = 0; a < count; ++a)
            {
                qint8 m_s_x = r->readByte();
                qint8 m_s_y = r->readByte();
                qint16 m_s_z = r->readShort();

                qint32 m_soundId = r->readInt();
            }

            // loadAmbianceData
            count = r->readByte();
            QVector<qint32> ambiancesId;

            for (qint8 a = 0; a < count; ++a)
                ambiancesId.push_back(r->readInt());

            qint8 size = r->readByte();
            for (qint8 a = 0; a < size; ++a)
                r->readByte();

            // loadInteractiveElements
            count = r->readByte();
            qDebug() << "InteractiveElements count : " << count;
            for (qint8 a = 0; a < count; ++a)
            {
                qint64 m_id = r->readLong();
                qDebug() << "IE id : " << m_id;
                qint16 m_type = r->readShort();
                qDebug() << "IE type : " << m_type;

                QVector<qint32> m_views;
                qint8 numView = r->readByte();
                for (qint8 b = 0; b < numView; ++b)
                    m_views.push_back(r->readInt());

                qint16 dataSize = r->readShort();
                for (qint16 b = 0; b < dataSize; ++b)
                    r->readByte();

                bool m_clientOnly = r->readByte(); // probably wrong, see readBooleanBit
                qDebug() << "IE clientOnly : " << m_clientOnly;

                qint16 m_landMarkType = r->readShort();
                qDebug() << "IE landMarkType : " << m_landMarkType;
            }

            // loadDynamicElements
            count = r->readByte();
            qDebug() << "DynamicElements count : " << count;
            for (qint8 a = 0; a < count; ++a)
            {
                qint8 m_de_x = r->readByte();
                qint8 m_de_y = r->readByte();
                qint16 m_de_z = r->readShort();

                qint32 m_id = r->readInt();
                qint32 m_gfxId = r->readInt();
                qint16 m_type = r->readShort();
                qint8 m_direction = r->readByte();
            }
        }

    }
    else if (ui->comboBox->currentText() == "tplg")
    {
        DialogTopology* dialogTopology = new DialogTopology(this);

        TopologyReader* tplgThread = new TopologyReader(this);
        connect(tplgThread, SIGNAL(resetProgressBar(int)), this, SLOT(resetProgressBar(int)));
        connect(tplgThread, SIGNAL(updateProgressBar()), this, SLOT(updateProgressBar()));
        connect(tplgThread, SIGNAL(updateTopology(QImage)), dialogTopology, SLOT(updateTopology(QImage)));
        connect(tplgThread, SIGNAL(finished()), tplgThread, SLOT(deleteLater()));

        tplgThread->setPath(path);
        tplgThread->start();
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

    // Parse elements.lib

}

void MainWindow::onSelectMap(QString map)
{
    if (!map.contains(".jar"))
        return;

    ui->progressBar->show();
    ui->progressBar->setValue(0);

    DialogTopology* dialogTopology = new DialogTopology(this);

    TopologyReader* tplgThread = new TopologyReader(this);
    connect(tplgThread, SIGNAL(resetProgressBar(int)), this, SLOT(resetProgressBar(int)));
    connect(tplgThread, SIGNAL(updateProgressBar()), this, SLOT(updateProgressBar()));
    connect(tplgThread, SIGNAL(updateTopology(QImage)), dialogTopology, SLOT(updateTopology(QImage)));
    connect(tplgThread, SIGNAL(finished()), tplgThread, SLOT(deleteLater()));

    tplgThread->setPath(_wakfuPath + "tplg/" + map);
    tplgThread->start();
}

void MainWindow::resetProgressBar(int max)
{
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(0);
}

void MainWindow::updateProgressBar()
{
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}
