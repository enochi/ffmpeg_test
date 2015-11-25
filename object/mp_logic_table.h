#ifndef MP_LOGIC_TABLE_H
#define MP_LOGIC_TABLE_H

#include "relation.h"

class Table
{
public:
    list<Relation> video_relation;
    list<Relation> audio_relation;
    int encode_bitrate;
    int conf_bitrate;

};

#endif // MP_LOGIC_TABLE_H
