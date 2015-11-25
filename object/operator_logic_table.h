#ifndef OPERATOR_LOGIC_TABLE_H
#define OPERATOR_LOGIC_TABLE_H

#include "mp_logic_table.h"
#include "operator_relation.h"

#include<iostream>
using namespace std;

inline ostream& operator << ( ostream& s, Table const& a )
{
	s << "Logic_Table { ";
    for(list<Relation>::const_iterator iter = a.audio_relation.begin();iter!=a.audio_relation.end();iter++)
    {
    s << *iter;
    }
    s << ", ";
    for(list<Relation>::const_iterator iter = a.video_relation.begin();iter!=a.video_relation.end();iter++)
    {
    s << *iter;
    }
	s << " } ";
	return s;
}
#endif // OPERATOR_LOGIC_TABLE_H
