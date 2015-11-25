#ifndef SPEND_TIMER_LOGGER_H
#define SPEND_TIMER_LOGGER_H
#include "Foundation/include/Poco/Timestamp.h"
#include <string>
class Spend_Timer_Logger
{
public:
    Spend_Timer_Logger(const char *msg,bool auto_print=true);
    ~Spend_Timer_Logger();
    void print_elapsed();
    int get_elapsed();
    int update();
private:
    Poco::Timestamp _last;
    std::string _msg;
    bool _auto_print;
};
#endif // SPEND_TIMER_LOGGER_H
