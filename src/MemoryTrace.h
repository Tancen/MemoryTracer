#ifndef MEMORYTRACE_H
#define MEMORYTRACE_H

#include <string>

namespace MemoryTrace
{
    bool init();
    const std::string& dump();
    void dumpToFile(const char* fileName);
}

#ifdef ENABLE_MEMORY_TRACE
    void* operator new(size_t size, const char* file, long line);
    void* operator new[](size_t size, const char* file, long line);
    void operator delete(void *p);
    void operator delete[](void *p);

    #define new new(__FILE__, __LINE__)
#endif

#endif // MEMORYTRACE_H
