#include <QCoreApplication>
//#include <AuthServer.h>
//#include <MessageServer.h>
#include <ServerManager.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServerManager sm;
    //sm.start();

    return a.exec();
}
