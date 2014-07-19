#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T13:42:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = regex-engine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    infixtopostfixconverter.cpp \
    state.cpp \
    nfa.cpp

HEADERS  += mainwindow.h \
    infixtopostfixconverter.h \
    state.h \
    nfa.h

FORMS    += mainwindow.ui

# Some C++11 features were used so I added the flag for the compiler.
QMAKE_CXXFLAGS += -std=c++11
