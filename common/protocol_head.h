#ifndef PROTOCOL_HEAD_H
#define PROTOCOL_HEAD_H

#include <cstdint>
enum Protocol_Type
{
    Pro_Logic_Table = 0,
    Pro_Heart,
    Pro_Video_Forward,
    Pro_Audio_Forward,
    Pro_Vsyn_Video_Encode,
    Pro_Vsyn_Video_Decode,
    Pro_Amix_Audio,
    Pro_MP_Capacity
};

enum Protocol_Version_Major
{
    Major_0 = 0,
    Major_1,
    Major_2
};

enum Protocol_Version_Minor
{
    Minor_0 = 0,
    Minor_1,
    Minor_2
};

struct Protocol_Version
{
    Protocol_Version_Major version_major;
    Protocol_Version_Minor version_minor;
};

#pragma pack (1)
struct Protocol_Head
{
    uint32_t             pkg_len;
    Protocol_Version     version;
    Protocol_Type        protocol_type;
};


struct MP_Feature
{
    int max_decode_count;
    int max_encode_count;
    int max_instream_count;
    int max_outstream_count;

};


struct Protocol_Message
{
    Protocol_Head head;
    char          body[0];
#ifdef __cplusplus

    Protocol_Type protocol_type()
    {
        return head.protocol_type;
    }

    Protocol_Version_Major major_version()
    {
        return head.version.version_major;
    }

    Protocol_Version_Minor minor_version()
    {
        return head.version.version_minor;
    }

    uint32_t pkg_len()
    {
        return head.pkg_len;
    }

    char* msg_body()
    {
        return body;
    }

#endif
};

#ifndef __cplusplus

Protocol_Type protocol_type(Protocol_Message &msg)
{
    return msg.protocol_type;
}


Protocol_Version_Major major_version(Protocol_Message &msg)
{
    return msg.version.version_major;
}

Protocol_Version_Minor minor_version(Protocol_Message &msg)
{
    return msg.version.version_minor;
}

uint32_t pkg_len(Protocol_Message &msg)
{
    return msg.pkg_len;
}

char* msg_body(Protocol_Message &msg)
{
    return msg.body[0];
}
#endif

#pragma pack ()
#endif
