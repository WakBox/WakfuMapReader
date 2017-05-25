#ifndef TOPOLOGYMAPA_H
#define TOPOLOGYMAPA_H

#include <QtCore>

#include "TopologyMap.h"

class TopologyMapA : public TopologyMap
{
public:
    TopologyMapA(BinaryReader* reader) : TopologyMap(reader) { _type = 0; }

    virtual void read()
    {
        TopologyMap::read();

        m_cost = _reader->readByte();
        m_murfin = _reader->readByte();
        m_property = _reader->readByte();
    }

    virtual void print()
    {
        TopologyMap::print();

        qDebug() << "m_cost:" << m_cost;
        qDebug() << "m_murfin:" << m_murfin;
        qDebug() << "m_property:" << m_property;
    }

private:
    qint8 m_cost;
    qint8 m_murfin;
    qint8 m_property;
};

#endif // TOPOLOGYMAPA_H
