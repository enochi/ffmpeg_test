#ifndef OPERATOR_VSYN_LAYOUT_H
#define OPERATOR_VSYN_LAYOUT_H

#include <cstring>
#include <cstdio>

#include <unordered_map>
#include <iostream>
using namespace std;


#include "vsyn_layout.h"

namespace std
{
    template <>
    struct less<Vsyn_Layout>
    {
        bool operator () ( Vsyn_Layout const& a, Vsyn_Layout const& b ) const
        {
//            if ( a.video_stream_desc.stream_type <
//                 b.video_stream_desc.stream_type )
//            {
//                return true;
//            }
//           if(a.video_stream_desc.stream_type ==
//                  b.video_stream_desc.stream_type)
//            {
                int a_size = (int)(a.right - a.left) * (int)(a.bottom - a.top);
                int b_size = (int)(b.right - b.left) * (int)(b.bottom - b.top);
                if ( a_size < b_size )
                    return true;
            //}

            return false;
        }

    };

    template <>
    struct equal_to<Vsyn_Layout>
    {
        bool operator () ( Vsyn_Layout const& a, Vsyn_Layout const& b ) const
        {
//            if ( a.video_stream_desc.stream_type !=
//                 b.video_stream_desc.stream_type )
//            {
//                return false;
//            }

            int a_size = (int)(a.right - a.left) * (int)(a.bottom - a.top);
            int b_size = (int)(b.right - b.left) * (int)(b.bottom - b.top);

            if ( a_size != b_size )
            {
                return false;
            }

            return true;
        }
    };


        template <>
        struct hash<Vsyn_Layout>
        {
            size_t operator() ( Vsyn_Layout const &key ) const
            {
                hash<string> hr;
                char sz[128]; char *ptr = sz;
                int w = (int)(key.right - key.left);
                int h = (int)(key.bottom - key.top);
                sprintf(ptr, "%d%d",w, h);
                string s = sz;

                return hr(s);
            }
        };

}

inline ostream& operator << ( ostream& s, Vsyn_Layout const& a )
{
    s << " Vsyn_Layout ( " << a.left << ", " << a.top << ", " << a.right << ", " << a.bottom << ") ";

    s << " ) ";
    return s;
}

#endif // OPERATOR_VSYN_LAYOUT_H
