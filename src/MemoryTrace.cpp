#include <string>
#include <Python.h>
#include <new.h>
#include <mutex>

namespace MemoryTrace
{
    bool init();
    const std::string& dump();
    void dumpToFile(const char* fileName);

    class Recorder
    {
    public:
        Recorder()
            :   m_module(NULL),
                m_class(NULL),
                m_obj(NULL)
        {

        }
        ~Recorder()
        {
            Py_CLEAR(m_obj);
            Py_CLEAR(m_class);
            Py_CLEAR(m_module);
            //Py_Finalize();
        }

        bool init()
        {
            try
            {
                m_strDump.resize(4096);
            }
            catch(...)
            {
                return false;
            }

            Py_Initialize();

            do
            {
                m_module = PyImport_ImportModule("recorder");
                if (!m_module)
                    break;

                m_class = PyObject_GetAttrString(m_module, "Recorder");
                if (!m_class)
                    break;

                 m_obj = PyObject_CallObject(m_class, NULL);
                if (!m_obj)
                    break;

                return true;
            } while (false);

            Py_CLEAR(m_obj);
            Py_CLEAR(m_class);
            Py_CLEAR(m_module);

            m_obj = NULL;
            m_class = NULL;
            m_module = NULL;

            //Py_Finalize();
            return false;
        }

        void add(void *p, const char* file, long line)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            PyObject_CallMethod(m_obj, "add", "(isi)", p, file, line);
        }

        void remove(void *p)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            PyObject_CallMethod(m_obj, "remove", "(i)", p);
        }

        const std::string& dump()
        {
            std::lock_guard<std::mutex> guard(m_mutex);

            m_strDump = "";
            PyObject *ret = PyObject_CallMethod(m_obj, "dump", NULL);

            do
            {
                if (!ret)
                {
                    PyObject *err = PyErr_Occurred();
                    if (err)
                    {
                        PyErr_Print();
                    }
                    break;
                }

                char *s = NULL;
                try
                {
                    if(PyArg_Parse(ret, "s", &s))
                        m_strDump = s;
                }
                catch(...)
                {

                }

            }
            while (false);
            Py_CLEAR(ret);

            return m_strDump;
        }

        void dumpToFile(const char* fileName)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            PyObject_CallMethod(m_obj, "dumpToFile", "(s)", fileName);
        }


    private:
        std::string m_strDump;

        PyObject* m_module;
        PyObject* m_class;
        PyObject* m_obj;

        std::mutex m_mutex;
    };

    Recorder gRecorder;
}


bool MemoryTrace::init()
{
    return MemoryTrace::gRecorder.init();
}

const std::string& MemoryTrace::dump()
{
    return MemoryTrace::gRecorder.dump();
}

void MemoryTrace::dumpToFile(const char* fileName)
{
    MemoryTrace::gRecorder.dumpToFile(fileName);
}


#ifdef ENABLE_MEMORY_TRACE
void* operator new(size_t size, const char* file, long line)
{
    void *ret = malloc(size);
    if (ret)
    {
        MemoryTrace::gRecorder.add(ret, file, line);
    }
    else
        throw std::bad_alloc();

    return ret;
}

void* operator new[](size_t size, const char* file, long line)
{
    return ::operator new(size, file, line);
}

void operator delete(void *p)
{
    free(p);
    MemoryTrace::gRecorder.remove(p);
}

void operator delete[](void *p)
{
    ::operator delete(p);
}
#endif
