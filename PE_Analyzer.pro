#-------------------------------------------------
#
# Project created by QtCreator 2018-11-27T21:23:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PE_Analyzer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        widget.cpp \
    subordinate_function.cpp \
    fileheader.cpp \
    optionalheader.cpp \
    sectionheader.cpp \
    details.cpp \
    directories.cpp \
    sections.cpp \
    dosheader.cpp \
    importtable.cpp \
    file_location_calculator.cpp \
    import_descriptor.cpp \
    add_import.cpp

HEADERS += \
        widget.h \
    subordinate_function.h \
    sectionheader.h \
    details.h \
    directories.h \
    sections.h \
    allheader.h \
    dosheader.h \
    fileheader.h \
    optionalheader.h \
    importtable.h \
    file_location_calculator.h \
    import_descriptor.h \
    add_import.h

FORMS += \
        widget.ui \
    details.ui \
    directories.ui \
    sections.ui \
    importtable.ui \
    file_location_calculator.ui \
    add_import.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
