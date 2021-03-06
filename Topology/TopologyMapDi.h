#ifndef TOPOLOGYMAPD_H
#define TOPOLOGYMAPD_H

#include <QtCore>

#include "TopologyMapBlockedCells.h"

class TopologyMapD : public TopologyMapBlockedCells
{
public:
    TopologyMapD(BinaryReader* reader) : TopologyMapBlockedCells(reader) { _type = 5; }

    virtual void read()
    {
        TopologyMapBlockedCells::read();

        int indexSize = _reader->readByte() & 0xFF;

        for (int i = 0; i < indexSize; ++i)
        {
            m_cost.push_back(_reader->readByte());
            m_murfin.push_back(_reader->readByte());
            m_property.push_back(_reader->readByte());
            m_zs.push_back(_reader->readShort());
            m_heights.push_back(_reader->readByte());
            m_movLos.push_back(_reader->readByte());
        }

        int cellCount = _reader->readByte() & 0xFF;
        for (int i = 0; i < cellCount; ++i)
            m_cells.push_back(_reader->readLong());

        int remainsCount = _reader->readShort() & 0xFFFF;
        for (int i = 0; i < remainsCount; ++i)
            m_cellsWithMultiZ.push_back(_reader->readInt());
    }

    virtual void print()
    {
        TopologyMapBlockedCells::print();

        qDebug() << "m_cost[]:" << m_cost;
        qDebug() << "m_murfin[]:" << m_murfin;
        qDebug() << "m_property[]:" << m_property;
        qDebug() << "m_zs[]:" << m_zs;
        qDebug() << "m_heights[]:" << m_heights;
        qDebug() << "m_movLos[]:" << m_movLos;
        qDebug() << "m_cells[]:" << m_cells;
        qDebug() << "m_cellsWithMultiZ[]:" << m_cells;
    }

private:
    QVector<qint8> m_cost;
    QVector<qint8> m_murfin;
    QVector<qint8> m_property;
    QVector<short> m_zs;
    QVector<qint8> m_heights;
    QVector<qint8> m_movLos;
    QVector<long> m_cells;
    QVector<int> m_cellsWithMultiZ;
};

#endif // TOPOLOGYMAPD_H
