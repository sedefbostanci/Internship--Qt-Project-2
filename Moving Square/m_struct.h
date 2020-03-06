#ifndef M_STRUCT_H
#define M_STRUCT_H

#pragma pack(1)
#include <QDataStream>

struct message{
    quint8 header;
    quint16 command;
    quint8 checksum;
} ;


#endif // M_STRUCT_H
