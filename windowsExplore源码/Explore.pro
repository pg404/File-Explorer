#-------------------------------------------------
#
# Project created by QtCreator 2017-07-14T15:26:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Explore
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    searchthread.cpp \
    filterdailog.cpp \
    widget.cpp \
    showcpumemory.cpp \
    showdatetime.cpp \
    showdevicesize.cpp \
    run.cpp \
    task.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    searchthread.h \
    filterdailog.h \
    widget.h \
    showcpumemory.h \
    showdatetime.h \
    showdevicesize.h \
    run.h \
    task.h \
    dialog.h

FORMS    += mainwindow.ui \
    filterdailog.ui \
    widget.ui \
    run.ui \
    task.ui \
    dialog.ui

RESOURCES += \
    icon.qrc

DISTFILES +=
RC_FILE = myapp.rc
