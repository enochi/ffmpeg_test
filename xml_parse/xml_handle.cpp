#include "tinybind/my_tinybind.h"

#include"xml_handle.h"
#include "common/video_stream_description.h"
XML_Handle::XML_Handle(){}

bool XML_Handle::table_from_xml(unsigned char const *xml_str, int xml_str_len, Table &table)
{
    string str_xml((char const *)xml_str, (size_t)xml_str_len);
    istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement  *xml_element = xml_doc.RootElement ();

    bool ret = bind_from_xml( *xml_element, &table );

    return ret;
}





bool XML_Handle::table_to_xml(  Table * table, unsigned char const *xml_str, int *xml_str_len/*in/out*/)
{
    //Table _table = *table;
    TiXmlElement xml_element("suyun_object_protocol");
    bool ret = bind_to_xml( &xml_element, *table );

    ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);
    return ret;
}





bool XML_Handle::device_from_xml(const unsigned char *xml_str, int xml_str_len, Device &device)
{
    string str_xml((char const *)xml_str, (size_t)xml_str_len);
    istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement  *xml_element = xml_doc.RootElement ();

    bool ret = bind_from_xml( *xml_element, &device );

    return ret;


}


bool XML_Handle::device_to_xml(Device *device, const unsigned char *xml_str, int *xml_str_len)
{
    //Device _device = *device;
    TiXmlElement xml_element("suyun_object_protocol");
    bool ret = bind_to_xml( &xml_element, *device );

    ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);
    return ret;
}






template<>
char const * convert_to_string<Device_Type>( Device_Type const& type )
{
    static char sz[64];
    sprintf(sz, "%d", (const int&)type);
    return sz;
}

template<>
void convert_from_string<Device_Type>( char const * strIn, Device_Type *type )
{
    *type = (Device_Type)atoi(strIn);
}


template<>
void convert_from_string<Audio_Encode_Type>( char const * strIn, Audio_Encode_Type *type )
{
    *type = (Audio_Encode_Type)atoi(strIn);
}


template<>
char const * convert_to_string<Audio_Encode_Type>( Audio_Encode_Type const& type )
{
    static char sz[64];
    sprintf(sz, "%d", (const int&)type);
    return sz;
}


template<>
char const * convert_to_string<WIS::UUID>( WIS::UUID const& uuid )
{
    static char sz[UUID_SIZE + 1] = {0};
    memcpy(sz, uuid.uuid, UUID_SIZE);
    sz[UUID_SIZE] = 0;
    return sz;
}


template<>
void convert_from_string<WIS::UUID>( char const * strIn, WIS::UUID *uuid )
{
    memcpy(uuid->uuid, strIn, UUID_SIZE);
}

template<>
void convert_from_string<Video_Stream_Type>( char const * strIn, Video_Stream_Type *type )
{
    *type = (Video_Stream_Type)atoi(strIn);
}


template<>
char const * convert_to_string<Video_Stream_Type>( Video_Stream_Type const& type )
{
    static char sz[64];
    sprintf(sz, "%d", (const int&)type);
    return sz;
}

template<>
void convert_from_string<Direct_Type>( char const * strIn, Direct_Type *type )
{
    *type = (Direct_Type)atoi(strIn);
}


template<>
char const * convert_to_string<Direct_Type>( Direct_Type const& type )
{
    static char sz[64];
    sprintf(sz, "%d", (const int&)type);
    return sz;
}


//******************Here is binding******************

TiXml_Binding<MP_ID> const *
get_tixml_binding(MP_ID const &, Identity<MP_ID> )
{
    static Member_TiXml_Binding<MP_ID> binding;

    if( binding.empty() )
    {
        binding.add_member( "mp_id", member<MP_ID>(&MP_ID::mp_id) );

    }
    return &binding;
}



TiXml_Binding<Relation_ID> const *
get_tixml_binding(Relation_ID const &, Identity<Relation_ID> )
{
    static Member_TiXml_Binding<Relation_ID> binding;

    if( binding.empty() )
    {
        binding.add_member( "relation_id", member<Relation_ID>(&Relation_ID::relation_id) );

    }
    return &binding;
}

TiXml_Binding<Conf_ID> const *
get_tixml_binding(Conf_ID const &, Identity<Conf_ID> )
{
    static Member_TiXml_Binding<Conf_ID> binding;

    if( binding.empty() )
    {
        binding .add_member("id",member<Conf_ID>(&Conf_ID::id));
        binding.add_member("uuid", member<Conf_ID>(&Conf_ID::uuid) );

    }
    return &binding;
}






TiXml_Binding<Video_Stream_Description> const *
get_tixml_binding(Video_Stream_Description const &, Identity<Video_Stream_Description> )
{
    static Member_TiXml_Binding<Video_Stream_Description> binding;

    if( binding.empty() )
    {
        binding.add_member( "video_stream_type", member<Video_Stream_Description>(&Video_Stream_Description::stream_type) );

    }
    return &binding;
}


TiXml_Binding<Audio_Stream_Description> const *
get_tixml_binding(Audio_Stream_Description const &, Identity<Audio_Stream_Description> )
{
    static Member_TiXml_Binding<Audio_Stream_Description> binding;

    if( binding.empty() )
    {
        binding.add_member( "audio_stream_type", member<Audio_Stream_Description>(&Audio_Stream_Description::audio_type) );

    }
    return &binding;
}



TiXml_Binding<Device> const *
get_tixml_binding( Device const &, Identity<Device> )
{
    static Member_TiXml_Binding<Device> binding;

    if( binding.empty() )
    {
        binding.add_member("uuid",member(&Device::uuid));
        binding.add_member( "from_ip", member(&Device::from_ip) );
        binding.add_member( "to_ip", member(&Device::to_ip) );
        binding.add_member( "video_port", member(&Device::video_port));
        binding.add_member("audio_port",member(&Device::audio_port));
        binding.add_member("from_video_stream_description",member<Device,Video_Stream_Description>(&Device::from_video_stream_desc));
         binding.add_member("to_video_stream_description",member<Device,Video_Stream_Description>(&Device::to_video_stream_desc));

        binding.add_member("audio_stream_description",member<Device,Audio_Stream_Description>(&Device::audio_stream_desc));
        binding.add_member( "device_type", member(&Device::device_type) );
        binding.add_member("direct_type",member(&Device::direct_type));
        binding.add_member("mp_id",member(&Device::mp_id));

        binding.add_member("qos_ip",member(&Device::qos_ip));
    }
    return &binding;
}

TiXml_Binding<MP_Forward_Relation> const *
get_tixml_binding( MP_Forward_Relation const &, Identity<MP_Forward_Relation> )
{
    static Member_TiXml_Binding<MP_Forward_Relation> binding;

    if( binding.empty() )
    {
        binding.add_member( "device", member<MP_Forward_Relation,Device>(&MP_Forward_Relation::device) );
        binding.add_member( "device_list", member<MP_Forward_Relation,list<Device> >(&MP_Forward_Relation::device_list) );
    }
    return &binding;
}



TiXml_Binding<Vsyn_Subtitle_Param> const *
get_tixml_binding( Vsyn_Subtitle_Param const &, Identity<Vsyn_Subtitle_Param> )
{
    static Member_TiXml_Binding<Vsyn_Subtitle_Param> binding;

    if( binding.empty() )
    {
        binding.add_member( "font", member(&Vsyn_Subtitle_Param::font) );
        binding.add_member( "pt", member(&Vsyn_Subtitle_Param::pt) );
        binding.add_member( "dpi", member(&Vsyn_Subtitle_Param::dpi) );
        binding.add_member( "color", member(&Vsyn_Subtitle_Param::color) );
        binding.add_member( "bgcolor", member(&Vsyn_Subtitle_Param::bgcolor) );
        binding.add_member( "transparence", member(&Vsyn_Subtitle_Param::transparence) );
        binding.add_member( "scrollspeed", member(&Vsyn_Subtitle_Param::scrollspeed) );
        binding.add_member( "subtitle_x", member(&Vsyn_Subtitle_Param::subtitle_x ) );
        binding.add_member( "subtitle_y", member(&Vsyn_Subtitle_Param::subtitle_y) );
        binding.add_member( "subtitle_width", member(&Vsyn_Subtitle_Param::subtitle_width) );
        binding.add_member( "subtitle_height", member(&Vsyn_Subtitle_Param::subtitle_height) );
        binding.add_member( "subtitle", member(&Vsyn_Subtitle_Param::subtitle) );

    }
    return &binding;
}




TiXml_Binding<Vsyn_Layout> const *
get_tixml_binding( Vsyn_Layout const &, Identity<Vsyn_Layout> )
{
    static Member_TiXml_Binding<Vsyn_Layout> binding;

    if( binding.empty() )
    {
        binding.add_member( "left", member(&Vsyn_Layout::left) );
        binding.add_member( "top", member(&Vsyn_Layout::top) );
        binding.add_member( "right", member(&Vsyn_Layout::right) );
        binding.add_member( "bottom", member(&Vsyn_Layout::bottom) );

    }
    return &binding;
}


TiXml_Binding<From_To> const *
get_tixml_binding( From_To const &, Identity<From_To> )
{
    static Member_TiXml_Binding<From_To> binding;

    if( binding.empty() )
    {

        binding.add_member( "to", member<From_To,list<Device> >(&From_To::to) );
        binding.add_member("from",member<From_To,list<Device> >(&From_To::from));
    }
    return &binding;
}


TiXml_Binding<Device_Location> const *
get_tixml_binding( Device_Location const &, Identity<Device_Location> )
{
    static Member_TiXml_Binding<Device_Location> binding;

    if( binding.empty() )
    {
        binding.add_member( "device", member<Device_Location,Device>(&Device_Location::device) );
        binding.add_member( "vsyn_layout", member<Device_Location,Vsyn_Layout>(&Device_Location::vsyn_layout) );
    }
    return &binding;
}



TiXml_Binding<Relation> const *
get_tixml_binding( Relation const &, Identity<Relation> )
{
    static Member_TiXml_Binding<Relation> binding;

    if( binding.empty() )
    {
        binding.add_member("mp_id",member<Relation>(&Relation::mp_id));
        binding.add_member("conf_id",member<Relation,Conf_ID>(&Relation::confid));
        binding.add_member("relation_id",member<Relation,Relation_ID>(&Relation::relation_id));
        binding.add_member( "from_to", member<Relation,From_To>(&Relation::from_to) );
        binding.add_member( "device_location", member<Relation,list<Device_Location> >(&Relation::vsyn_layouts) );
        binding.add_member( "vsyn_subtitle_param", member<Relation,list<Vsyn_Subtitle_Param> >(&Relation::vsyn_subtitle) );

    }
    return &binding;
}


TiXml_Binding<Table> const *
get_tixml_binding( Table const &, Identity<Table> )
{
    static Member_TiXml_Binding<Table> binding;
    if( binding.empty() )
    {
        binding.add_member( "audio_relation", member<Table,list<Relation> >(&Table::audio_relation) );
        binding.add_member( "video_relation", member<Table,list<Relation> >(&Table::video_relation) );
        binding.add_member( "encode_bitrate", member(&Table::encode_bitrate) );
        binding.add_member( "conf_bitrate", member(&Table::conf_bitrate) );
    }
    return &binding;
}

TiXml_Binding<Video_Forward> const *
get_tixml_binding( Video_Forward const &, Identity<Video_Forward> )
{
    static Member_TiXml_Binding<Video_Forward> binding;
    if( binding.empty() )
    {
        binding.add_member( "video_forward", member<Video_Forward,list<MP_Forward_Relation > >(&Video_Forward::video_forward) );
        binding.add_member( "qos_video_forward", member<Video_Forward,list<MP_Forward_Relation > >(&Video_Forward::qos_video_forward) );
    }
    return &binding;
}

TiXml_Binding<Audio_Forward> const *
get_tixml_binding( Audio_Forward const &, Identity<Audio_Forward> )
{
    static Member_TiXml_Binding<Audio_Forward> binding;
    if( binding.empty() )
    {
        binding.add_member( "audio_forward", member<Audio_Forward,list<MP_Forward_Relation > >(&Audio_Forward::audio_forward) );
        binding.add_member( "qos_audio_forward", member<Audio_Forward,list<MP_Forward_Relation > >(&Audio_Forward::qos_audio_forward) );
    }
    return &binding;
}
