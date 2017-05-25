#ifndef TOPOLOGYMAP_H
#define TOPOLOGYMAP_H

#include <QtCore>

#include "BinaryReader.h"

class TopologyMap
{
public:
    TopologyMap(BinaryReader* reader)
    {
        _reader = reader;
    }

    ~TopologyMap() { delete _reader; }

    virtual void read()
    {
        m_x = _reader->readShort() * 18;
        m_y = _reader->readShort() * 18;
        m_z = _reader->readShort();
    }

    virtual void print()
    {
        qDebug() << "=================";
        qDebug() << "= MAP <> TYPE <"<< _type <<">";
        qDebug() << "= X:"<<m_x<<"| Y:"<<m_y<<"| Z:"<<m_z;
        qDebug() << "=================";
        qDebug() << "";
        qDebug() << " = Map Topology Data:";
    }

    template <typename T>
    static QString printVector(QVector<T>& vector)
    {
        QString print;

        for (int i = 0; i < vector.size(); ++i)
            print += QString::number(vector.at(i));

        return print;
    }

protected:
    BinaryReader* _reader;
    qint8 _type;

    int m_x;
    int m_y;
    short m_z;

};

#endif // TOPOLOGYMAP_H
