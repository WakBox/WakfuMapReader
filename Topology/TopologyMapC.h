#ifndef TOPOLOGYMAPC_H
#define TOPOLOGYMAPC_H

#include <QtCore>

#include "TopologyMap.h"

class TopologyMapC : public TopologyMap
{
public:
    TopologyMapC(BinaryReader* reader) : TopologyMap(reader) { _type = 3; }

    virtual void read()
    {
        TopologyMap::read();

        for (int i = 0; i < 324; ++i)
        {
            m_cost.push_back(_reader->readByte());
            m_murfin.push_back(_reader->readByte());
            m_property.push_back(_reader->readByte());
            m_zs.push_back(_reader->readShort());
            m_heights.push_back(_reader->readByte());
            m_movLos.push_back(_reader->readByte());
        }
    }

    virtual void print()
    {
        TopologyMap::print();

        qDebug() << "m_cost[]:" << m_cost;
        qDebug() << "m_murfin[]:" << m_murfin;
        qDebug() << "m_property[]:" << m_property;
        qDebug() << "m_zs[]:" << m_zs;
        qDebug() << "m_heights[]:" << m_heights;
        qDebug() << "m_movLos[]:" << m_movLos;
    }

private:
    QVector<qint8> m_cost;
    QVector<qint8> m_murfin;
    QVector<qint8> m_property;
    QVector<short> m_zs;
    QVector<qint8> m_heights;
    QVector<qint8> m_movLos;
};

#endif // TOPOLOGYMAPC_H
