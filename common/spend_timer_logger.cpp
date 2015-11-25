#include "spend_timer_logger.h"

#include <cstdio>
using namespace std;


Spend_Timer_Logger::Spend_Timer_Logger(const char *msg,bool auto_print ) : _msg(msg),_auto_print(auto_print)
{

}
void Spend_Timer_Logger::print_elapsed()
{

    fprintf(stderr, "%s +++++++++++++ end spend time : [ %ldd]\n", _msg.c_str(),_last.elapsed());
    _last.update();
}
int Spend_Timer_Logger::get_elapsed()
{

    return (int)_last.elapsed();

}
int Spend_Timer_Logger::update()
{
    _last.update();
    return 0;
}
Spend_Timer_Logger::~Spend_Timer_Logger()
{
    if(_auto_print)
    {


        fprintf(stderr, "%s +++++++++++++ end spend time : [ %lld]\n", _msg.c_str(), _last.elapsed());
    }


}
