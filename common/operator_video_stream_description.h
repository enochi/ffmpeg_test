#ifndef OPERATOR_VIDEO_STREAM_DESCRIPTION_H
#define OPERATOR_VIDEO_STREAM_DESCRIPTION_H

#include<iostream>
#include <unordered_map>
using namespace std;

#include "video_stream_description.h"
namespace std
{

		template <>
		struct hash<Video_Stream_Description>
		{
			size_t operator() ( Video_Stream_Description const &desc ) const
			{
				return desc.stream_type;
			}
		};

}


inline ostream& operator << ( ostream& s, Video_Stream_Description const& a )
{
    s << "Video_Stream_type (";
    s << a.stream_type;
    s << ") ";

    return s;
}

#endif
