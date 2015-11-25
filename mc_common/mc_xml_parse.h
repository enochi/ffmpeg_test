#ifndef MC_XML_PARSE_HANDLE_H
#define MC_XML_PARSE_HANDLE_H

/* create: 2015-08-31
 * author: chpan
 * modify: 2015-08-31
 */
#include "sip_command_msg.h"
using std::string;
class MC_XML_Parse_Handle
{
public:
    MC_XML_Parse_Handle();

    static MC_XML_Parse_Handle* instance()
    {
        static MC_XML_Parse_Handle object;
        return &object;
    }

    bool from_xml(unsigned char const* xml_str, const int xml_str_len, Request& req);
    bool to_xml(Request* req, unsigned char* xml_str, int* xml_str_len);

    bool from_xml(unsigned char const* xml_str, const int xml_str_len, Release_Request& req);

    bool from_xml(unsigned char const* xml_str, const int xml_str_len, Audio_Mixser_Ctr& ctr);
    bool from_xml(unsigned char const* xml_str, const int xml_str_len, Video_Synec_Ctr& ctr);
    bool to_xml(Audio_Mixser_Ctr* req, unsigned char * xml_str, int* xml_str_len);
    bool to_xml(Video_Synec_Ctr* req, unsigned char* xml_str, int* xml_str_len);

    bool from_xml(unsigned char const* xml_str, const int xml_str_len, Forward_Ctr& ctr);
    bool to_xml(Forward_Ctr* req, unsigned char* xml_str, int* xml_str_len);

    // mp_agent and ucs_agent@ call
    bool from_xml(unsigned char const* xml_str, const int xml_str_len, Response_OptStatus& res);
    bool to_xml(Response_OptStatus* req, unsigned char * xml_str, int* xml_str_len);

    bool from_xml(unsigned char const* xml_str, const int xml_str_len, Notify_Message& ctr);
    bool to_xml(Notify_Message* req, unsigned char* xml_str, int* xml_str_len);

    bool from_xml(unsigned char const* xml_str, const int xml_str_len, Response& ctr);
    bool to_xml(Response* rep, unsigned char* xml_str, int* xml_str_len);

    //for ms
    #if 0//todo ¸ÄÄ£°å
    template<class T>
    bool from_xml( const string & xml,T &data);
    template<class T>
    bool to_xml(T &data, string & xml,const string root="Response");
 

#else
    bool from_xml(const string & xml,Iframe_Apply &data);
    bool to_xml(Iframe_Apply &data, string & xml,const string root="Control");

    bool from_xml(const string & xml,Chanstatus &data);
    bool to_xml(Chanstatus &data, string & xml,const string root="Notify");

    bool from_xml(const string & xml,Chan_Store_Req &data);
    bool to_xml(Chan_Store_Req &data, string & xml,const string root="Request");

    bool from_xml(const string & xml,Chan_Store_Req_Res &data);
    bool to_xml(Chan_Store_Req_Res &data, string & xml,const string root="Response");

    bool from_xml(const string & xml,Media_Push &data);
    bool to_xml(Media_Push &data, string & xml,const string root="Control");

    bool from_xml(const string & xml,Store_Set &data);
    bool to_xml(Store_Set &data, string & xml,const string root="Control");

    bool from_xml(const string & xml,OptStatus &data);
    bool to_xml(OptStatus &data, string & xml,const string root="Response");

    bool from_xml(const string & xml,Device_Status &data);
    bool to_xml(Device_Status &data, string & xml,const string root="Notify");


    bool from_xml(const string & xml,File_End_Notify &data);
    bool to_xml(File_End_Notify &data, string & xml,const string root="Notify");

    bool from_xml(const string & xml,Store_Notify &data);
    bool to_xml(Store_Notify &data, string & xml,const string root="Notify");

    bool from_xml(const string & xml,Name_Query &data);
    bool to_xml(Name_Query &data, string & xml,const string root="Control");

    bool from_xml(const string & xml,Time_Query &data);
    bool to_xml(Time_Query &data, string & xml,const string root="Control");

    bool from_xml(const string & xml,Query_Response &data);
    bool to_xml(Query_Response &data, string & xml,const string root="Response");



#endif

};

#endif // MC_XML_PARSE_HANDLE_H
