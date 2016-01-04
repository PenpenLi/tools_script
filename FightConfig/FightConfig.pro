#-------------------------------------------------
#
# Project created by QtCreator 2014-11-11T09:58:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FightConfig
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    json/internal/pow10.h \
    json/internal/stack.h \
    json/internal/strfunc.h \
    json/document.h \
    json/filestream.h \
    json/prettywriter.h \
    json/rapidjson.h \
    json/reader.h \
    json/stringbuffer.h \
    json/writer.h

FORMS    += mainwindow.ui
