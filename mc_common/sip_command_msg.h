#ifndef SIP_COMMAND_MSG_H
#define SIP_COMMAND_MSG_H
/* create: 2015-08-31
 * author: chpan
 * modify: 2015-08-31
 * 该文件类名以及变量命名是根据sip指令的xml
 */
#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>
#include <list>
static const char* command_name_type_str[] = {
    "audiomixapply",
    "audiomixdrop",
    "videomixapply",
    "videomixdrop",
    "audiomixresponse",
    "videomixresponse",
    "audiomixset",
    "videomixset",
    "switchset",
    "switchsetdrop",
    "switchsetresponse",
    "optstatus",
    "chanstatus",
    "null_commandname"
};

enum Command_Name_Type{
    audiomixapply = 0, // 混音器申请
    audiomixdrop,   // 混音器释放
    videomixapply,  // 合成器申请
    videomixdrop,   // 合成器释放

    audiomixresponse,   // 混音器响应
    videomixresponse,   // 合成器响应

    audiomixset,    // 混音器设置
    videomixset,    // 合成器设置

    switchset,      // 转发器设置
    switchsetdrop,   // 转发器释放（自定义）
    switchsetresponse,
    optstatus,      //
    chanstatus,

    null_commandname    // 自定义初始化
};

struct Response_OptStatus{
    Command_Name_Type   type;
    int seq;
    std::string response;
};

struct Notify_Message{
    Command_Name_Type   type;
    int seq;
    std::string devname;
    std::string devid;
    std::string groupid;
    std::string channelstatus;
    std::string chanid;
};

enum Response_Type{
    RESPONSE_OK_TYPE,
    RESPONSE_ERR_TYPE,
    NULL_RESPONSE_TYPE
};

struct Posids{
    std::vector<std::string> posids;
};

struct Rtpids{
    std::vector<std::string> rtpids;
};

class Fwd_Rtpid_Rlt{
public:
    Fwd_Rtpid_Rlt(){}
    Fwd_Rtpid_Rlt(const Fwd_Rtpid_Rlt& r)
    {
        this->dst_rtpids = r.dst_rtpids;
        this->max_dst = r.max_dst;
        this->src_rtpid = r.src_rtpid;
    }

    Fwd_Rtpid_Rlt& operator = (const Fwd_Rtpid_Rlt& r)
    {
        this->dst_rtpids = r.dst_rtpids;
        this->max_dst = r.max_dst;
        this->src_rtpid = r.src_rtpid;
        return *this;
    }

    std::uint32_t   max_dst;
    Rtpids  src_rtpid;
    Rtpids  dst_rtpids;
};

struct SRC_DST_Synec{
    std::string rtpid;
    std::string posid;
    std::string codec;
    std::string format;
    std::int32_t bandwidth;
};

struct SRC_DST_Mixer{
    std::string rtpid;
    std::string posid;
    std::string codec;
    std::string format;
};

enum Stream_Type{
    VIDEO_STREAM_TYPE,
    AUDIO_STREAM_TYPE
};

class Stream_Base{
public:
    Stream_Base(){}
    virtual ~Stream_Base(){}
    Stream_Base(const Stream_Base& sb)
    {
        this->ip = sb.ip;
        this->codec = sb.codec;
        this->format = sb.format;
        this->stream_type = sb.stream_type;
        this->port = sb.port;
    }

    Stream_Base& operator = (const Stream_Base& sb)
    {
        this->ip = sb.ip;
        this->codec = sb.codec;
        this->format = sb.format;
        this->stream_type = sb.stream_type;
        this->port = sb.port;
    }

    std::uint32_t ip;
    std::string codec;
    std::string format;
    Stream_Type stream_type;
    unsigned short   port;
};

struct SPS_Data{
    int width;
    int height;
    std::string format;
};

class Video_Stream_Info : public Stream_Base{
public:
    Video_Stream_Info(){}
    ~Video_Stream_Info(){}
    Video_Stream_Info(const Video_Stream_Info& sb)
    {
        this->ip = sb.ip;
        this->codec = sb.codec;
        this->format = sb.format;
        this->stream_type = sb.stream_type;
        this->port = sb.port;
        this->sps = sb.sps;
    }
    Video_Stream_Info& operator = (const Video_Stream_Info& sb)
    {
        this->ip = sb.ip;
        this->codec = sb.codec;
        this->format = sb.format;
        this->stream_type = sb.stream_type;
        this->port = sb.port;
        this->sps = sb.sps;
    }

    std::string sps;
};

struct Audio_Stream_Info : public Stream_Base{
public:
    Audio_Stream_Info(){}
    ~Audio_Stream_Info(){}
};

struct Video_Region{
    std::string posid;
    std::int32_t left;
    std::int32_t top;
    std::string relativesize;
};

struct Video_Layouts{
    std::vector<Video_Region> layouts;
};

struct SIP_Info2MP{
    std::unordered_map<std::string, std::uint32_t> posid_ip;
    std::unordered_map<std::string, std::uint32_t> rtpid_ip;
    std::unordered_map<std::string, unsigned short> dst_rtp_port;
    unsigned short   src_rtp_port;
    std::uint32_t   mp_ip;
};

/*　SIP协议报文基类
 */
class SIP_Base_CMD{
public:
    SIP_Base_CMD() :
        type(null_commandname),
        seq(-1)
    {}
    SIP_Base_CMD(Command_Name_Type& type_, std::int32_t seq_) :
        type(type_),
        seq(seq_)
    {}
    virtual ~SIP_Base_CMD() {}

    Command_Name_Type type;
    std::int32_t    seq;
};

class MP_Media_Capability{
public:
    std::string mp_id;
    std::uint32_t mp_ip;
    int audio_port;
    int video_port;

    int max_decode_count;
    int max_encode_count;
    int max_instream_count;
    int max_outstream_count;

};

/*　SIP_AGNENT 收取到合成器、混音器申请指令
 */
class Request : public SIP_Base_CMD{
public:
    Request() :
        mixser_id(""),
        mixinput(-1),
        inputcodec(""),
        mixoutput(-1),
        outputcodec("")
    {}
    ~Request() {}

    std::string    mixser_id;

    Posids src_posids;
    Posids dst_posids;

    std::int32_t    mixinput;

    std::string     inputcodec;

    std::int32_t    mixoutput;

    std::string     outputcodec;

};

/*　SIP_AGNENT 收取到合成器、混音器释放指令
 */
class Release_Request : public SIP_Base_CMD{
public:
    Release_Request() :
        mixerid("")
    {}
    ~Release_Request() {}

    std::string     mixerid;
};

/* 提供给SIP_AGENT回复的数据结构
 */
class Response : public SIP_Base_CMD{
public:
    Response() :
        mixerid(""),
        result(""),
        src(Posids()),
        dst(Posids()),
        reason()
    {}

    ~Response() {}
    std::string     mixerid;
    std::string     result;
    Posids  src;
    Posids  dst;
    std::string     reason;
};

/* 音频混音器设置数据结构
 */
class Audio_Mixser_Ctr : public SIP_Base_CMD{
public:
    Audio_Mixser_Ctr() {}

    ~Audio_Mixser_Ctr() {}

    std::string    mixserid;
    std::vector<SRC_DST_Mixer>  src_mixser_list;
    std::vector<SRC_DST_Mixer>  dst_mixser_list;
    SIP_Info2MP sipinfo;
};

/* 视频合成器设置数据结构
 */
class Video_Synec_Ctr : public SIP_Base_CMD{
public:
    Video_Synec_Ctr() {}

    ~Video_Synec_Ctr() {}

    std::string    mixserid;
    std::vector<SRC_DST_Synec>  src_synec_list;
    std::vector<Video_Region>   videolayout;
    std::vector<SRC_DST_Synec>  dst_synec_list;

    SIP_Info2MP sipinfo;
};

/* 转发器设置数据结构
 */
class Forward_Ctr : public SIP_Base_CMD{
public:
    Forward_Ctr() :
        mp_ip(0)
    {}

    ~Forward_Ctr() {}

    std::vector<Fwd_Rtpid_Rlt>   fwd_lists;

    std::unordered_map<std::string, Stream_Base*>  streams;
    std::uint32_t   mp_ip;
};

/* 转发器设置数据结构
 */
class Forward_Drop : public SIP_Base_CMD{
public:
    Forward_Drop() {}

    std::vector<std::string>   drop_lists;
};


//for ms
class Iframe_Apply
{
public:
    Iframe_Apply():commandname("iframeapply"),seq(1){}
    std::string commandname;
    int seq;
    int max_require;
    Posids rtpids_require;
    Posids rtpids_offer;

};
class Chanstatus
{
public:
    Chanstatus():commandname("chanstatus"),seq(1){}
    std::string commandname;
    int seq;
    std::string devid;
    std::string groupid;
    std::string channelstatus;
    int chanid;
};
class Chan_Store_Req
{
public:
    Chan_Store_Req():commandname("chanstorereq"),seq(1){}
    std::string commandname;
    int seq;
    std::string filename;
    int rtpid;
    std::string codec;
    int timeduration;
};
class Chan_Store_Req_Res
{
public:
    Chan_Store_Req_Res():commandname("chanstorereq"),seq(1){}
    std::string commandname;
    int seq;
    std::string filename;
    std::string sid;
};

enum E_Ring_Type
{
    E_ONLINE,
    E_OFFLINE,
    E_RINGING
};

class Src_Type
{
public:

    std::string username;
    std::string userid;//for tmp use ,as device addr
    std::string postfix;
    E_Ring_Type ring;
    std::string url;    //use in dst as save file name
    std::string video;
    std::string audio;
};
class Media_Push
{
public:
    Media_Push():commandname("mediapush"),seq(1){}
    std::string commandname;
    int seq;
    std::string operation;
    Src_Type src;
    Src_Type dst;
};

class Store_Set
{
public:
    Store_Set():commandname("storeset"),seq(1){}
    std::string commandname;
    int seq;
    std::string operation;
    std::string devid;
    std::string filename;
    std::string fileformat;
    int timeduration;
};
class OptStatus{
public:
    OptStatus():commandname("optStatus"),seq(1)
    {}
    std::string commandname;
    int seq;
    std::string response;
    std::string err;
};
class Device_Status
{
public:
    Device_Status():commandname("devicestatus"),seq(1)
    {}
    std::string commandname;
    int seq;
    std::string devname;
    std::string devid;
    std::string groupid;
    std::string status;
};

class File_End_Notify
{
public:
    std::string    commandname;    // "filetoend"
    int     seq;
    std::string    filename;
    std::string    devid;

    File_End_Notify():commandname("filetoend"),seq(1)
    {
    }
};

class Store_Notify
{
public:
    std::string    commandname;    // "storenotify"
    int     seq;
    std::string    filename;
    std::string    devid;
    std::string    error;

    Store_Notify():commandname("storenotify"),seq(1)
    {
    }
};

class Name_Query
{
public:
    std::string    commandname; // "filequery"
    int     seq;
    std::string    mode;       // "file"
    std::string    filename;

    Name_Query():commandname("filequery"),seq(1)
    {
    }
};

class Time_Query
{
public:
    std::string    commandname;    // "filequery"
    int     seq;
    std::string    mode[32];           // "time"
    std::string    recordstarttime;
    std::string    recordstoptime;

    Time_Query():commandname("filequery"), seq(1)
    {}
};

class File_Prperty
{
public:
    std::string     filename;
    std::string     recordtime;
    std::string     filensize;
    std::string     fileduration;
    std::string     filedescript;

    File_Prperty()
    {

    }
};

class Query_Response
{
public:
    std::string     commandname;    // "querylist"
    std::string     mode;           // "file" | "time"
    int     seq;
    std::list<File_Prperty>  filelist;

    Query_Response():commandname("querylist"), seq(0)
    {
    }
};
#endif // SIP_COMMAND_MSG_H
