#ifndef XML_BINDING_ALL_H
#define XML_BINDING_ALL_H
#include "sip_command_msg.h"

template<>
void convert_from_string<Command_Name_Type>( char const * strIn, Command_Name_Type *command_name_type )
{
    for (int type = audiomixapply; type != null_commandname+1; ++type)
    {
        if (strncmp(strIn, command_name_type_str[type], strlen(command_name_type_str[type])) == 0)
        {
            *command_name_type = (Command_Name_Type)type;
        }
    }
}

template<>
char const* convert_to_string<Command_Name_Type>( Command_Name_Type const& type )
{
    return command_name_type_str[type];
}


template<>
char const* convert_to_string<E_Ring_Type>( E_Ring_Type const& type )
{
    const char * ret = "";
    switch(type)
    {
    case E_ONLINE:
        ret = "online";
        break;
    case E_OFFLINE:
        ret = "offline";
        break;
    case E_RINGING:
        ret = "ringing";
        break;
    default:
        ret = "";

    }
    return ret;
}

template<>
void convert_from_string<E_Ring_Type>( char const * strIn, E_Ring_Type *ring_type )
{
    if(strcmp(strIn,"online") == 0)
    {
        *ring_type = E_ONLINE;
    }
    else if(strcmp(strIn,"offline") == 0)
    {
        *ring_type = E_OFFLINE;
    }
    else if(strcmp(strIn,"ringing") == 0)
    {
        *ring_type = E_RINGING;
    }
    else
    {
        fprintf(stderr,"ring type error\n");
    }

}

TiXml_Binding<Posids> const *
get_tixml_binding(Posids const &, Identity<Posids> )
{
    static Member_TiXml_Binding<Posids> binding;

    if( binding.empty() )
    {
        binding.add_member( "posid", member<Posids, std::vector<std::string> >(&Posids::posids) );
    }
    return &binding;
}

TiXml_Binding<Rtpids> const *
get_tixml_binding(Rtpids const &, Identity<Rtpids> )
{
    static Member_TiXml_Binding<Rtpids> binding;

    if( binding.empty() )
    {
        binding.add_member( "rtpid", member(&Rtpids::rtpids) );
    }
    return &binding;
}

TiXml_Binding<Response_OptStatus> const *
get_tixml_binding(Response_OptStatus const &, Identity<Response_OptStatus> )
{
    static Member_TiXml_Binding<Response_OptStatus> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&Response_OptStatus::type) );
        binding.add_member( "seq", member(&Response_OptStatus::seq) );
        binding.add_member( "response", member(&Response_OptStatus::response) );
    }
    return &binding;
}

TiXml_Binding<Notify_Message> const *
get_tixml_binding(Notify_Message const &, Identity<Notify_Message> )
{
    static Member_TiXml_Binding<Notify_Message> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&Notify_Message::type) );
        binding.add_member( "seq", member(&Notify_Message::seq) );
        binding.add_member( "devid", member(&Notify_Message::devid) );
        binding.add_member( "groupid", member(&Notify_Message::groupid) );
        binding.add_member( "channelstatus", member(&Notify_Message::channelstatus) );
        binding.add_member( "chanid", member(&Notify_Message::chanid) );
    }
    return &binding;
}

TiXml_Binding<Fwd_Rtpid_Rlt> const *
get_tixml_binding(Fwd_Rtpid_Rlt const &, Identity<Fwd_Rtpid_Rlt> )
{
    static Member_TiXml_Binding<Fwd_Rtpid_Rlt> binding;

    if( binding.empty() )
    {
        binding.add_member( "max-dst", member(&Fwd_Rtpid_Rlt::max_dst) );
        binding.add_member( "src", member(&Fwd_Rtpid_Rlt::src_rtpid) );
        binding.add_member( "dst", member(&Fwd_Rtpid_Rlt::dst_rtpids) );
    }
    return &binding;
}

TiXml_Binding<SRC_DST_Synec> const *
get_tixml_binding(SRC_DST_Synec const &, Identity<SRC_DST_Synec> )
{
    static Member_TiXml_Binding<SRC_DST_Synec> binding;

    if( binding.empty() )
    {
        binding.add_member( "rtpid", member(&SRC_DST_Synec::rtpid) );
        binding.add_member( "posid", member(&SRC_DST_Synec::posid) );
        binding.add_member( "codec", member(&SRC_DST_Synec::codec) );
        binding.add_member( "format", member(&SRC_DST_Synec::format) );
        binding.add_member( "bandwidth", member(&SRC_DST_Synec::bandwidth) );
    }
    return &binding;
}

TiXml_Binding<SRC_DST_Mixer> const *
get_tixml_binding(SRC_DST_Mixer const &, Identity<SRC_DST_Mixer> )
{
    static Member_TiXml_Binding<SRC_DST_Mixer> binding;

    if( binding.empty() )
    {
        binding.add_member( "rtpid", member(&SRC_DST_Mixer::rtpid) );
        binding.add_member( "posid", member(&SRC_DST_Mixer::posid) );
        binding.add_member( "codec", member(&SRC_DST_Mixer::codec) );
        binding.add_member( "format", member(&SRC_DST_Mixer::format) );
    }
    return &binding;
}

TiXml_Binding<SIP_Base_CMD> const *
get_tixml_binding(SIP_Base_CMD const &, Identity<SIP_Base_CMD> )
{
    static Member_TiXml_Binding<SIP_Base_CMD> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&SIP_Base_CMD::type) );
        binding.add_member( "seq", member(&SIP_Base_CMD::seq) );
    }
    return &binding;
}

TiXml_Binding<Request> const *
get_tixml_binding(Request const &, Identity<Request> )
{
    static Member_TiXml_Binding<Request> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member<Request, Command_Name_Type>(&Request::type) );
        binding.add_member( "seq", member<Request, std::int32_t>(&Request::seq) );
        binding.add_member( "mixinput", member(&Request::mixinput) );
        binding.add_member( "inputcodec", member(&Request::inputcodec) );
        binding.add_member( "mixoutput", member(&Request::mixoutput) );
        binding.add_member( "outputcodec", member(&Request::outputcodec) );
    }
    return &binding;
}

TiXml_Binding<Release_Request> const *
get_tixml_binding(Release_Request const &, Identity<Release_Request> )
{
    static Member_TiXml_Binding<Release_Request> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member<Release_Request, Command_Name_Type>(&Release_Request::type) );
        binding.add_member( "seq", member<Release_Request, std::int32_t>(&Release_Request::seq) );
        binding.add_member( "mixerid", member(&Release_Request::mixerid) );
    }
    return &binding;
}

TiXml_Binding<Response> const *
get_tixml_binding(Response const &dataIn, Identity<Response> )
{
    static Member_TiXml_Binding<Response> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member<Response, Command_Name_Type>(&Response::type) );
        binding.add_member( "seq", member<Response, std::int32_t>(&Response::seq) );
        binding.add_member( "mixerid", member<Response, std::string>(&Response::mixerid) );
        if (dataIn.result == "OK")
        {
            binding.add_member( "src", member<Response, Posids>(&Response::src) );
            binding.add_member( "dst", member(&Response::dst) );
        }
        binding.add_member( "result", member<Response, std::string>(&Response::result) );
        if (dataIn.result == "ERROR")
        {
            binding.add_member( "reason", member<Response, std::string>(&Response::reason) );
        }
    }
    return &binding;
}

TiXml_Binding<Audio_Mixser_Ctr> const *
get_tixml_binding(Audio_Mixser_Ctr const &, Identity<Audio_Mixser_Ctr> )
{
    static Member_TiXml_Binding<Audio_Mixser_Ctr> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member<Audio_Mixser_Ctr, Command_Name_Type>(&Audio_Mixser_Ctr::type) );
        binding.add_member( "seq", member<Audio_Mixser_Ctr, std::int32_t>(&Audio_Mixser_Ctr::seq) );
        binding.add_member( "mixerid", member<Audio_Mixser_Ctr, std::string>(&Audio_Mixser_Ctr::mixserid) );
        binding.add_member( "src_mixser_list", member(&Audio_Mixser_Ctr::src_mixser_list) );
        binding.add_member( "dst_mixser_list", member(&Audio_Mixser_Ctr::dst_mixser_list) );
    }
    return &binding;
}

TiXml_Binding<Video_Region> const *
get_tixml_binding(Video_Region const &, Identity<Video_Region> )
{
    static Member_TiXml_Binding<Video_Region> binding;

    if( binding.empty() )
    {
        binding.add_member( "posid", member(&Video_Region::posid) );
        binding.add_member( "left", member(&Video_Region::left) );
        binding.add_member( "top", member(&Video_Region::top) );
        binding.add_member( "relativesize", member(&Video_Region::relativesize) );

    }
    return &binding;
}

TiXml_Binding<Video_Synec_Ctr> const *
get_tixml_binding(Video_Synec_Ctr const &, Identity<Video_Synec_Ctr> )
{
    static Member_TiXml_Binding<Video_Synec_Ctr> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member<Video_Synec_Ctr, Command_Name_Type>(&Video_Synec_Ctr::type) );
        binding.add_member( "seq", member<Video_Synec_Ctr, std::int32_t>(&Video_Synec_Ctr::seq) );
        binding.add_member( "mixserid", member(&Video_Synec_Ctr::mixserid) );
        binding.add_member( "src", member(&Video_Synec_Ctr::src_synec_list) );
        binding.add_member( "videolayout", member(&Video_Synec_Ctr::videolayout) );
        binding.add_member( "dst", member(&Video_Synec_Ctr::dst_synec_list) );

    }
    return &binding;
}

TiXml_Binding<Forward_Ctr> const *
get_tixml_binding(Forward_Ctr const &, Identity<Forward_Ctr> )
{
    static Member_TiXml_Binding<Forward_Ctr> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member<Forward_Ctr, Command_Name_Type>(&Forward_Ctr::type) );
        binding.add_member( "seq", member<Forward_Ctr, std::int32_t>(&Forward_Ctr::seq) );
        binding.add_member( "switchlists", member(&Forward_Ctr::fwd_lists) );
    }
    return &binding;
}

//for ms
TiXml_Binding<Iframe_Apply> const *
get_tixml_binding(Iframe_Apply const &, Identity<Iframe_Apply> )
{
    static Member_TiXml_Binding<Iframe_Apply> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member<Iframe_Apply, std::string>(&Iframe_Apply::commandname) );
        binding.add_member( "seq", member<Iframe_Apply, int>(&Iframe_Apply::seq) );
        binding.add_member( "max-require", member(&Iframe_Apply::max_require) );
        binding.add_member( "require", member(&Iframe_Apply::rtpids_require) );
        binding.add_member( "offer", member(&Iframe_Apply::rtpids_offer) );

    }
    return &binding;
}

TiXml_Binding<Chanstatus> const *
get_tixml_binding(Chanstatus const &, Identity<Chanstatus> )
{
    static Member_TiXml_Binding<Chanstatus> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member<Chanstatus, std::string>(&Chanstatus::commandname) );
        binding.add_member( "seq", member<Chanstatus, int>(&Chanstatus::seq ) );
        binding.add_member( "devid", member(&Chanstatus::devid) );
        binding.add_member( "groupid", member(&Chanstatus::groupid) );
        binding.add_member( "channelstatus", member(&Chanstatus::channelstatus) );
        binding.add_member( "chanid", member(&Chanstatus::chanid) );

    }
    return &binding;
}

TiXml_Binding<Chan_Store_Req> const *
get_tixml_binding(Chan_Store_Req const &, Identity<Chan_Store_Req> )
{
    static Member_TiXml_Binding<Chan_Store_Req> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&Chan_Store_Req::commandname));
        binding.add_member( "seq", member(&Chan_Store_Req::seq)) ;
        binding.add_member( "codec", member(&Chan_Store_Req::codec) );
        binding.add_member( "filename", member(&Chan_Store_Req::filename) );
        binding.add_member( "rtpid", member(&Chan_Store_Req::rtpid) );
        binding.add_member( "timeduration", member(&Chan_Store_Req::timeduration) );

    }
    return &binding;
}

TiXml_Binding<Chan_Store_Req_Res> const *
get_tixml_binding(Chan_Store_Req_Res const &, Identity<Chan_Store_Req_Res> )
{
    static Member_TiXml_Binding<Chan_Store_Req_Res> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&Chan_Store_Req_Res::commandname));
        binding.add_member( "seq", member(&Chan_Store_Req_Res::seq)) ;
        binding.add_member( "sid", member(&Chan_Store_Req_Res::sid) );
        binding.add_member( "filename", member(&Chan_Store_Req_Res::filename) );
    }
    return &binding;
}

TiXml_Binding<Src_Type> const *
get_tixml_binding(Src_Type const &, Identity<Src_Type> )
{
    static Member_TiXml_Binding<Src_Type> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&Src_Type::audio));
        binding.add_member( "ring", member(&Src_Type::ring)) ;
        binding.add_member( "postfix", member(&Src_Type::postfix) );
        binding.add_member( "url", member(&Src_Type::url) );
        binding.add_member( "userid", member(&Src_Type::userid) );
        binding.add_member( "username", member(&Src_Type::username) );
        binding.add_member( "video", member(&Src_Type::video) );
    }
    return &binding;
}

TiXml_Binding<Media_Push> const *
get_tixml_binding(Media_Push const &, Identity<Media_Push> )
{
    static Member_TiXml_Binding<Media_Push> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&Media_Push::commandname));
        binding.add_member( "seq", member(&Media_Push::seq)) ;
        binding.add_member( "src", member(&Media_Push::src) );
        binding.add_member( "dst", member(&Media_Push::dst) );
        binding.add_member( "operation", member(&Media_Push::operation) );
    }
    return &binding;
}
TiXml_Binding<Store_Set> const *
get_tixml_binding(Store_Set const &, Identity<Store_Set> )
{
    static Member_TiXml_Binding<Store_Set> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&Store_Set::commandname));
        binding.add_member( "seq", member(&Store_Set::seq)) ;
        binding.add_member( "devid", member(&Store_Set::devid) );
        binding.add_member( "filename", member(&Store_Set::filename) );
        binding.add_member( "fileformat", member(&Store_Set::fileformat) );
        binding.add_member( "operation", member(&Store_Set::operation) );
        binding.add_member( "timeduration", member(&Store_Set::timeduration) );
    }
    return &binding;
}

TiXml_Binding<OptStatus> const *
get_tixml_binding(OptStatus const &, Identity<OptStatus> )
{
    static Member_TiXml_Binding<OptStatus> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&OptStatus::commandname));
        binding.add_member( "seq", member(&OptStatus::seq)) ;
        binding.add_member( "response", member(&OptStatus::response) );
        binding.add_member( "err", member(&OptStatus::err) );
    }
    return &binding;
}
TiXml_Binding<Device_Status> const *
get_tixml_binding(Device_Status const &, Identity<Device_Status> )
{
    static Member_TiXml_Binding<Device_Status> binding;

    if( binding.empty() )
    {
        binding.add_member( "commandname", member(&Device_Status::commandname));
        binding.add_member( "seq", member(&Device_Status::seq)) ;
        binding.add_member( "devid", member(&Device_Status::devid) );
        binding.add_member( "devname", member(&Device_Status::devname) );
        binding.add_member( "groupid", member(&Device_Status::groupid) );
        binding.add_member( "status", member(&Device_Status::status) );
    }
    return &binding;
}



TiXml_Binding<File_End_Notify> const *
get_tixml_binding(File_End_Notify const &, Identity<File_End_Notify>)
{
    static Member_TiXml_Binding<File_End_Notify> binding;

    if( binding.empty() )
    {
        binding.add_member("commandname", member(&File_End_Notify::commandname));
        binding.add_member("seq", member<File_End_Notify>(&File_End_Notify::seq));
        binding.add_member("filename", member(&File_End_Notify::filename));
        binding.add_member("devid", member(&File_End_Notify::devid));
    }
    return &binding;
}

TiXml_Binding<Store_Notify> const *
get_tixml_binding(Store_Notify const &, Identity<Store_Notify>)
{
    static Member_TiXml_Binding<Store_Notify> binding;

    if( binding.empty() )
    {
        binding.add_member("commandname", member(&Store_Notify::commandname));
        binding.add_member("seq", member<Store_Notify>(&Store_Notify::seq));
        binding.add_member("filename", member(&Store_Notify::filename));
        binding.add_member("devid", member(&Store_Notify::devid));
        binding.add_member("error", member(&Store_Notify::error));
    }
    return &binding;
}

TiXml_Binding<Name_Query> const *
get_tixml_binding(Name_Query const &, Identity<Name_Query>)
{
    static Member_TiXml_Binding<Name_Query> binding;

    if( binding.empty() )
    {
        binding.add_member("commandname", member(&Name_Query::commandname));
        binding.add_member("seq", member<Name_Query>(&Name_Query::seq));
        binding.add_member("mode", member(&Name_Query::mode));
        binding.add_member("filename", member(&Name_Query::filename));
    }
    return &binding;
}

TiXml_Binding<Time_Query> const *
get_tixml_binding(Time_Query const &, Identity<Time_Query>)
{
    static Member_TiXml_Binding<Time_Query> binding;

    if( binding.empty() )
    {
        binding.add_member("commandname", member(&Time_Query::commandname));
        binding.add_member("seq", member<Time_Query>(&Time_Query::seq));
        binding.add_member("mode", member(&Time_Query::mode));
        binding.add_member("recordstarttime", member(&Time_Query::recordstarttime));
        binding.add_member("recordstoptime", member(&Time_Query::recordstoptime));
    }
    return &binding;
}

TiXml_Binding<File_Prperty> const *
get_tixml_binding(File_Prperty const &, Identity<File_Prperty>)
{
    static Member_TiXml_Binding<File_Prperty> binding;

    if( binding.empty() )
    {
        binding.add_member("filename", member(&File_Prperty::filename));
        binding.add_member("recordtime", member(&File_Prperty::recordtime));
        binding.add_member("filensize", member(&File_Prperty::filensize));
        binding.add_member("fileduration", member(&File_Prperty::fileduration));
        binding.add_member("filedescript", member(&File_Prperty::filedescript));
    }
    return &binding;
}



TiXml_Binding<std::list<File_Prperty> > const *
get_tixml_binding(std::list<File_Prperty> const &, Identity<std::list<File_Prperty> >)
{
    static Stl_Container_TiXml_Binding< File_Prperty, std::list<File_Prperty> >binding(true, "fileprperty");
    return &binding;
}

TiXml_Binding<Query_Response> const *
get_tixml_binding(Query_Response const &, Identity<Query_Response>)
{
    static Member_TiXml_Binding<Query_Response> binding;

    if( binding.empty() )
    {
        binding.add_member("commandname", member(&Query_Response::commandname));
        binding.add_member("mode", member(&Query_Response::mode));
        binding.add_member("seq", member<Query_Response>(&Query_Response::seq));
        binding.add_member("filelist", member<Query_Response, std::list<File_Prperty> >(&Query_Response::filelist));
    }
    return &binding;
}





#endif // XML_BINDING_ALL_H

