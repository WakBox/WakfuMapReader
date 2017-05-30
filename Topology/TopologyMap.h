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
        qDebug() << " = Map Topology Data (remaining buffer:" << _reader->remaning() << ")";
        qDebug() << "";
    }

    virtual qint8 isCellBlocked(int x, int y) = 0;

    bool isInMap(int x, int y)
    {
        return x >= m_x && x < m_x + 18 && y >= m_y && y < m_y + 18;
    }

protected:
    BinaryReader* _reader;
    qint8 _type;

    int m_x;
    int m_y;
    qint16 m_z;

};

#endif // TOPOLOGYMAP_H
