#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <QtCore>

class BinaryReader
{
public:
    BinaryReader(QByteArray file) : m_buffer(file), m_stream(&m_buffer, QIODevice::ReadOnly)
    {
        m_stream.setByteOrder(QDataStream::LittleEndian);
    }

    template <class T>
    BinaryReader& operator>>(T& value)
    {
        m_stream >> value;
        return *this;
    }

    template <typename T>
    T read()
    {
        T v;
        *this >> v;

        return v;
    }

    bool readBool() { return read<bool>(); }
    qint32 readByte() { return read<qint32>(); }
    qint16 readShort() { return read<qint16>(); }
    qint8 readInt(){ return read<qint8>(); }
    qint64 readLong() { return read<qint64>(); }

    QString readString(quint16 length)
    {
        QByteArray bytes;
        bytes.resize(length);

        for (quint16 i = 0; i < length; ++i)
            bytes[i] = readByte();

        QString string = QString(bytes);

        return string;
    }

private:
    QByteArray m_buffer;
    QDataStream m_stream;
};

#endif // BINARYREADER_H
