#include "UserAgent.hxx"
#include "mc_common/mc_xml_parse.h"
using namespace resip;
static int process_cmd()
{
    std::cout<<"input cmd:";
    std::string line;
    while(std::getline(std::cin,line) )
    {
        int pos = line.find(" ");
        std::string cmd = line.substr(0,pos);

        if(cmd == "mediapush")
        {
            char to[256];
            char filename[256];
            int n = sscanf(line.c_str(),"%*s %s %s",to,filename);
             if(n != 2)
             {
                 fprintf(stderr,"input as mediapush to filename\n");
                 continue;
             }

//            Media_Push param;
//            param.src.url = filename;
//            param.src.userid = _contact.uri().user().c_str();
//            param.dst.userid = to;
            //send_media_push(param);
        }
        else if(cmd == "storeset")
        {
            char operation[256];
            char devid[256];
            char filename[256];
            int n = sscanf(line.c_str(),"%*s %s %s %s",operation,devid,filename);
            if(n != 3)
            {
                fprintf(stderr,"input as storeset operation devid filename\n");
                continue;
            }
           Store_Set param;
           param.devid = devid;
           param.filename = filename;
           param.operation = operation;
           //send_storeset(param);
        }
        else if(cmd == "invite")
        {

        }
        else
        {
            fprintf(stderr,"wrong cmd\n");
        }
        sleep(1);
    }
    return 0;
}
int main(int argc,char* argv[])
{
    UserAgent ua(argc,argv);
    ua.startup();
    while(1)
    {
        ua.process();
    }
}

