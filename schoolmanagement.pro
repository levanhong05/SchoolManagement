#-------------------------------------------------
#
# Project created by QtCreator 2013-11-05T05:11:36
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++11

linux-*: QMAKE_CXX = g++-4.8

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SchoolPersonnelManagement
TEMPLATE = app

SOURCES +=  main.cpp\
            const.cpp \
            person.cpp \
            student.cpp \
            teacher.cpp \
            mainwindow.cpp \
            postgraduate.cpp \
            studentmodel.cpp \
            teachermodel.cpp \
            postgraduatemodel.cpp \
            teachingassistant.cpp \
            commonfilterdialog.cpp \
            commontabledelegate.cpp \
            teachingassistantmodel.cpp

HEADERS  += const.h \
            person.h \
            student.h \
            teacher.h \
            mainwindow.h \
            postgraduate.h \
            studentmodel.h \
            teachermodel.h \
            postgraduatemodel.h \
            teachingassistant.h \
            commonfilterdialog.h \
            commontabledelegate.h \
            teachingassistantmodel.h

FORMS    += mainwindow.ui

RESOURCES += \
    toolbar_icon.qrc
