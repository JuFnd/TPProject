#include <QApplication>
#include <SessionManager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SessionManager client;
    client.start();

    return a.exec();
}
