QT += core
QT -= gui

CONFIG += c++11

INCLUDEPATH += ../python/include
INCLUDEPATH += ../src

DEFINES += ENABLE_MEMORY_TRACE=\\\"\\\"

TARGET = MemoryTracer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../src/MemoryTrace.cpp

HEADERS += \
    ../src/MemoryTrace.h

windows:{
    LIBS += $$PWD/../python/lib/libpython36.a
}
