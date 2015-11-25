#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <signal.h>
#include <pthread.h>
#include <execinfo.h>
#include <unistd.h>
#if 1
#include "log.h"
void signal_clear()
{
    struct sigaction act;
    //忽略以下信号
    act.sa_handler = SIG_IGN;
    sigaction ( SIGINT,  &act, NULL );
}
extern pthread_t exit_tid;

extern void *exit_thread(void *);

void signal_routine(int signo)
{
    printf("signal_routine num:%d\n",signo);
     MY_DEBUG("%?\n");
    if (SIGUSR1 == signo || SIGBUS==signo ||SIGINT == signo)
    {
        signal_clear();
        exit_thread(NULL);
        //        int rc = pthread_create(&exit_tid, NULL, &exit_thread, NULL);
        //        if(rc != 0)
        //        {
        //            MY_DEBUG("create exit thread failed");
        //            exit(-1);
        //        }
        //        pthread_detach(exit_tid);
        return;
    }


    SIG_DFL(signo);


}

void signal_init()
{
    struct sigaction act;
    act.sa_handler = signal_routine;
    act.sa_flags   = 0;
    sigemptyset ( &act.sa_mask );

    sigaction ( SIGUSR1, &act, NULL );
    sigaction ( SIGUSR2, &act, NULL );
    sigaction ( SIGBUS, &act, NULL );
    sigaction ( SIGSEGV, &act, NULL );
    sigaction ( SIGINT,  &act, NULL );
    //忽略以下信号
    act.sa_handler = SIG_IGN;
    sigaction ( SIGTERM, &act, NULL );
    sigaction ( SIGHUP,  &act, NULL );
    //
    sigaction ( SIGQUIT, &act, NULL );
    sigaction ( SIGABRT, &act, NULL );
}
#else

void printStack()
{
    unsigned int i = 0;
    void *bt[20];
    char **strings;
    size_t sz;

    fprintf(stderr, "main Stack trace (non-dedicated):\n");
    sz = backtrace(bt, 20);
    strings = backtrace_symbols(bt, sz);
    if (strings == NULL) {
        fprintf(stderr, "End of stack trace\n");
        return;
    }

    for(i = 0; i < sz; ++i){
        fprintf(stderr, "%s\n", strings[i]);
    }
    free(strings);
    fprintf(stderr, "End of stack trace\n");

}

void SignalHander(int signal)
{

    unsigned int tid = syscall(__NR_gettid); //gettid();
    fprintf(stderr, "--->SignalHander() main thread will exit by signal:%d,pid:%d\n",signal, tid);

    printStack();

    char name[32] = {0};

    printf("--->SignalHander() main thread exit thread name:%s\n",name);

    SIG_DFL(signal);
}

void GetBackTrace(int signal)
{
    printStack();

    unsigned int tid = syscall(__NR_gettid); //gettid();
    printf("--->PrintStack()  recieve signal:%d,pid:%d\n\n",signal, tid);

}

void CaptureAllSignal()
{
    int i = 0;
    for(i = 0; i < 32; i ++)
    {

        if ( (i == SIGPIPE)  )

        {
            signal(i, SIG_IGN);
        }

        else
        {
            signal(i, SignalHander);
        }
    }
}
#endif
#endif // SIGNAL_HANDLER_H
