#include "mainwindow.h"
#include <QApplication>
#include <QTableWidget>
#include <QTabWidget>
#include "common/signal_handler.h"
QApplication * g_a = NULL;
void *exit_thread(void *)
{
    printf("mp tools quit\n");
    if (g_a)
        g_a->exit();

}

int main(int argc, char *argv[])
{
    //signal_init();

    QApplication a(argc, argv);
    g_a = &a;
    MainWindow w;
    w.show();


    return a.exec();
}
