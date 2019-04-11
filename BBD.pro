#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T17:05:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BBD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    book.cpp \
    student.cpp \
    podrobno.cpp \
    managementbd.cpp \
    addstudent.cpp \
    add.cpp

HEADERS  += mainwindow.h \
    book.h \
    student.h \
    podrobno.h \
    managementbd.h \
    add.h \
    addstudent.h

FORMS    += mainwindow.ui \
    podrobno.ui \
    add.ui \
    addstudent.ui

RESOURCES +=

win32 {
        RC_FILE += icon.rc
        OTHER_FILES += icon.rc
}
