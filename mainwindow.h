#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDropEvent>
#include <QMimeData>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

#include "BinaryReader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void openFile(QString filename);
    void readTopology();

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    
private:
    Ui::MainWindow *ui;

    QString _filename;
    QByteArray _file;
};

#endif // MAINWINDOW_H
