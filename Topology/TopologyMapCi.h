#ifndef TOPOLOGYMAPCI_H
#define TOPOLOGYMAPCI_H

#include <QtCore>

#include "TopologyMapBlockedCells.h"

class TopologyMapCi : public TopologyMapBlockedCells
{
public:
    TopologyMapCi(BinaryReader* reader) : TopologyMapBlockedCells(reader) { _type = 4; }

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

        int cellSize = _reader->readByte() & 0xFF;
        for (int i = 0; i < cellSize; ++i)
            m_cells.push_back(_reader->readLong());
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
    }

private:
    QVector<qint8> m_cost;
    QVector<qint8> m_murfin;
    QVector<qint8> m_property;
    QVector<short> m_zs;
    QVector<qint8> m_heights;
    QVector<qint8> m_movLos;
    QVector<long> m_cells;
};

#endif // TOPOLOGYMAPCI_H
