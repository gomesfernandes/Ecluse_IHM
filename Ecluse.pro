#-------------------------------------------------
#
# Project created by QtCreator 2017-03-03T22:17:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ecluse
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        ecluse.cpp \
    loginwindow.cpp \
    signallumineux.cpp \
    porte.cpp \
    vanne.cpp

HEADERS  += ecluse.h \
    loginwindow.h \
    signallumineux.h \
    porte.h \
    vanne.h \
    constantes.h

FORMS    += ecluse.ui \
    loginwindow.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    images/porte1.png \
    images/porte2.png \
    images/feu_rouge.png \
    images/feu_vert.png
