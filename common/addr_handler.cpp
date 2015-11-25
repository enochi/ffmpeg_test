#include "addr_handler.h"
#include "dotini_parser.h"

int Addr_Handler::get_mcu_login_port()
{
    int port = 1900;
    int rc = dotini_open("wis_meetting.ini");
    if ( rc == 0 )
    {
        port = dotini_get_integer( "wis_center", "login_port" );
        dotini_close();
    }

    return port;
}

int Addr_Handler::get_mcu_msg_port()
{
    int port = 1991;
    int rc = dotini_open("wis_meetting.ini");
    if ( rc == 0 )
    {
        port = dotini_get_integer( "wis_center", "msg_port" );
        dotini_close();
    }

    return port;
}
