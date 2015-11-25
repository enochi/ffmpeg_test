#include <string>

#include "mc_xml_parse.h"
#include "tinybind/my_tinybind.h"
#include "xml_binding.h"

MC_XML_Parse_Handle::MC_XML_Parse_Handle()
{
}

bool MC_XML_Parse_Handle::from_xml(const unsigned char* xml_str, const int xml_str_len, Response_OptStatus& res)
{
    bool fr = false;

    std::string str_xml((char const*)xml_str, (size_t)xml_str_len);
    std::istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &res);

    return fr;
}

bool MC_XML_Parse_Handle::to_xml(Response_OptStatus* req, unsigned char* xml_str, int* xml_str_len)
{
    bool fr = false;
    assert (req != NULL);
    TiXmlElement xml_element("Response");

    fr = bind_to_xml( &xml_element, *req );

    std::ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const unsigned char* xml_str, const int xml_str_len, Notify_Message& ctr)
{
    bool fr = false;

    std::string str_xml((char const*)xml_str, (size_t)xml_str_len);
    std::istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &ctr);

    return fr;
}

bool MC_XML_Parse_Handle::to_xml(Notify_Message* req, unsigned char* xml_str, int* xml_str_len)
{
    bool fr = false;
    assert (req != NULL);
    TiXmlElement xml_element("Notify");

    fr = bind_to_xml( &xml_element, *req );

    std::ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const unsigned char* xml_str, const int xml_str_len, Response& ctr)
{
    bool fr = false;

    std::string str_xml((char const*)xml_str, (size_t)xml_str_len);
    std::istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &ctr);

    return fr;
}

bool MC_XML_Parse_Handle::to_xml(Response* rep, unsigned char* xml_str, int* xml_str_len)
{
    bool fr = false;
    assert (rep != NULL);
    TiXmlElement xml_element("Reponse");

    fr = bind_to_xml( &xml_element, *rep );

    std::ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const unsigned char* xml_str, const int xml_str_len, Request& req)
{
    bool fr = false;

    std::string str_xml((char const*)xml_str, (size_t)xml_str_len);
    std::istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &req);

    return fr;
}

bool MC_XML_Parse_Handle::to_xml(Request* req, unsigned char* xml_str, int* xml_str_len)
{
    bool fr = false;
    assert (req != NULL);
    TiXmlElement xml_element("Request");

    fr = bind_to_xml( &xml_element, *req );

    std::ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const unsigned char* xml_str, const int xml_str_len, Release_Request& req)
{
    bool fr = false;

    std::string str_xml((char const*)xml_str, (size_t)xml_str_len);
    std::istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &req);

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const unsigned char* xml_str, const int xml_str_len, Audio_Mixser_Ctr& ctr)
{
    bool fr = false;

    std::string str_xml((char const*)xml_str, (size_t)xml_str_len);
    std::istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &ctr);

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const unsigned char* xml_str, const int xml_str_len, Video_Synec_Ctr& req)
{
    bool fr = false;

    std::string str_xml((char const*)xml_str, (size_t)xml_str_len);
    std::istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &req);

    return fr;
}

bool MC_XML_Parse_Handle::to_xml(Audio_Mixser_Ctr* ctr, unsigned char* xml_str, int* xml_str_len)
{
    bool fr = false;
    assert (ctr != NULL);
    TiXmlElement xml_element("Control");

    fr = bind_to_xml( &xml_element, *ctr );

    std::ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);

    return fr;
}

bool MC_XML_Parse_Handle::to_xml(Video_Synec_Ctr* ctr, unsigned char* xml_str, int* xml_str_len)
{
    bool fr = false;
    assert (ctr != NULL);
    TiXmlElement xml_element("Control");

    fr = bind_to_xml( &xml_element, *ctr );

    std::ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const unsigned char* xml_str, const int xml_str_len, Forward_Ctr& ctr)
{
    bool fr = false;

    std::string str_xml((char const*)xml_str, (size_t)xml_str_len);
    std::istringstream s(str_xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &ctr);

    return fr;
}

bool MC_XML_Parse_Handle::to_xml(Forward_Ctr* req, unsigned char* xml_str, int* xml_str_len)
{
    bool fr = false;
    assert (req != NULL);
    TiXmlElement xml_element("Control");

    fr = bind_to_xml( &xml_element, *req );

    std::ostringstream s;
    s << xml_element;

    *xml_str_len = s.str ().size () + 1;

    memcpy ((void*)xml_str, s.str ().c_str (), *xml_str_len);

    return fr;
}

//for ms
#if 1

bool MC_XML_Parse_Handle::from_xml(const string & xml,Iframe_Apply &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}

bool MC_XML_Parse_Handle::to_xml(Iframe_Apply &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}
bool MC_XML_Parse_Handle::from_xml(const string & xml,Chanstatus &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Chanstatus &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Chan_Store_Req &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Chan_Store_Req &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Chan_Store_Req_Res &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Chan_Store_Req_Res &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Media_Push &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Media_Push &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Store_Set &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Store_Set &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,OptStatus &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(OptStatus &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Device_Status &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Device_Status &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}


bool MC_XML_Parse_Handle::from_xml(const string & xml,File_End_Notify &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(File_End_Notify &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Store_Notify &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Store_Notify &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Name_Query &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Name_Query &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Time_Query &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Time_Query &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}

bool MC_XML_Parse_Handle::from_xml(const string & xml,Query_Response &data)
{
    bool fr = false;

    std::istringstream s(xml);

    TiXmlDocument xml_doc;

    s >> xml_doc;

    TiXmlElement* xml_element = xml_doc.RootElement ();

    fr = bind_from_xml(*xml_element, &data);

    return fr;
}
bool MC_XML_Parse_Handle::to_xml(Query_Response &data, string & xml,const string root)
{
    bool fr = false;
    TiXmlElement xml_element(root);

    fr = bind_to_xml( &xml_element, data );

    std::ostringstream s;
    s << xml_element;
    xml = s.str();

    return fr;
}
#endif



