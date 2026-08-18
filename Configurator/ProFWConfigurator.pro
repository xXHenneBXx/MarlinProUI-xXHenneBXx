QT       += core gui widgets

CONFIG += c++17
TARGET = ProFWConfigurator
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    configgenerator.cpp

HEADERS += \
    mainwindow.h \
    configgenerator.h

FORMS += \
    mainwindow.ui

RESOURCES += \
	resources.qrc

RC_ICONS = \
	Doc/Logo.ico

DISTFILES +=
