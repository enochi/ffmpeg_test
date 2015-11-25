#ifndef OPERATOR_AUDIO_STREAM_DESCRIPTION_H
#define OPERATOR_AUDIO_STREAM_DESCRIPTION_H

#include<iostream>
#include <unordered_map>
using namespace std;

#include "audio_stream_description.h"
namespace std
{

        template <>
        struct hash<Audio_Stream_Description>
        {
            size_t operator() ( Audio_Stream_Description const &desc ) const
            {
                return desc.audio_type;
            }
        };

}


inline ostream& operator << ( ostream& s, Audio_Stream_Description const& a )
{
    s << "Audio_Stream_type (";
    s << a.audio_type;
    s << ") ";

    return s;
}



#endif // OPERATOR_AUDIO_STREAM_DESCRIPTION_H
