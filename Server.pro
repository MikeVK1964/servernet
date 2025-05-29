QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE     = app
SOURCES	     = main.cpp \
               MyServer.cpp \
               doctras.cpp \
               maketras.cpp \
               mknetserver.cpp
QT          += network
HEADERS      = MyServer.h \
    DocTrace.h \
    maketras.h \
    mknetsalut.h
win32:TARGET = ../MyServer
