#include <QCoreApplication>
//#include "mythread.h"
#include "reciver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    thread TD1("asdfs");
//    TD1.HW();


//    MyThread thread1("A"), thread2("B"), thread3("C");

//    thread1.start();
//    thread2.start();
//    thread3.start();

    return a.exec();
}
