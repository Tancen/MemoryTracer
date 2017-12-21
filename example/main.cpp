#include <qdebug.h>
#include <thread>
#include <qfileinfo.h>
#include <qdir.h>
#include "MemoryTrace.h"

class A
{
public:
    int a;
};


int main(int argc, char *argv[])
{
#ifdef ENABLE_MEMORY_TRACE
    qDebug("ENABLE_MEMORY_TRACE");
#endif

    if (!MemoryTrace::init())
    {
        qDebug("MemoryTrace::init failed");
        return 1;
    }

    std::thread thread1([]()
    {
        for (int i = 0; i < 1000; ++i)
        {
            int *p = new int(1);
            if (rand() & 1)
                delete p;
        }
    });

    std::thread thread2([]()
    {
        for (int i = 0; i < 1000; ++i)
        {
            int *p = new int[2];
            if (rand() & 1)
                delete[] p;
        }
    });

    std::thread thread3([]()
    {
        for (int i = 0; i < 1000; ++i)
        {
            A *p = new A();
            if (rand() & 1)
                delete p;
        }
    });

    thread1.join();
    thread2.join();
    thread3.join();


    QFileInfo fileInfo(argv[0]);
    std::string fileName = fileInfo.absoluteDir().absolutePath().toStdString() + "\\dump.log";

    qDebug(MemoryTrace::dump().c_str());
    MemoryTrace::dumpToFile(fileName.c_str());

    qDebug("exit");
    return 0;
}
