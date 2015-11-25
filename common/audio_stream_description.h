#ifndef AUDIO_STREAM_DESCRIPTION_H
#define AUDIO_STREAM_DESCRIPTION_H

#include "audio_stream_type.h"

class Audio_Stream_Description
{
public:
    Audio_Encode_Type audio_type;

    Audio_Stream_Description& operator = (Audio_Stream_Description const&desc)
    {
        audio_type = desc.audio_type;
        return *this;
    }

    bool operator == (Audio_Stream_Description const&desc) const
    {
        return audio_type == desc.audio_type;
    }

    bool operator < (Audio_Stream_Description const&desc) const
    {
        return audio_type < desc.audio_type;
    }

    bool operator > (Audio_Stream_Description const&desc) const
    {
        return audio_type > desc.audio_type;
    }
};

#endif // AUDIO_STREAM_DESCRIPTION_H
