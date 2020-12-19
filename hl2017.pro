TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_CXXFLAGS +=-Ofast -march=native
LIBS+= -lpthread -lstdc++fs
SOURCES += main.cpp \
    parsedata.cpp \
    handler.cpp \
    getparams.cpp \
    misc.cpp \
    fetchVisits.cpp \
    countAvg.cpp \
    cycle.cpp \
    master.cpp \
    worker.cpp \
    sockshare.cpp

HEADERS += \
    headers.h
