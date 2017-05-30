#-------------------------------------------------
#
# Project created by QtCreator 2014-03-19T08:56:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WakfuMapReader
TEMPLATE = app

macx: {
    LIBS += "/usr/local/lib/libquazip.dylib"
    INCLUDEPATH += "/usr/local/include/quazip"
}

win32 {
    LIBS += -L"C:/Quazip/lib" -lquazip
    INCLUDEPATH += C:/Quazip/include
}

SOURCES += main.cpp\
        mainwindow.cpp \
    DialogTopology.cpp

HEADERS  += mainwindow.h \
    BinaryReader.h \
    Topology/TopologyMap.h \
    Topology/TopologyMapA.h \
    Topology/TopologyMapB.h \
    Topology/TopologyMapBi.h \
    Topology/TopologyMapBlockedCells.h \
    Topology/TopologyMapC.h \
    Topology/TopologyMapCi.h \
    Topology/TopologyMapDi.h \
    TopologyReader.h \
    DialogTopology.h

FORMS    += mainwindow.ui \
    DialogTopology.ui
