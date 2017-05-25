#-------------------------------------------------
#
# Project created by QtCreator 2014-03-19T08:56:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WakfuMapReader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    BinaryReader.h \
    Topology/TopologyMap.h \
    Topology/TopologyMapA.h \
    Topology/TopologyMapB.h \
    Topology/TopologyMapBi.h \
    Topology/TopologyMapC.h \
    Topology/TopologyMapCi.h \
    Topology/TopologyMapD.h

FORMS    += mainwindow.ui
