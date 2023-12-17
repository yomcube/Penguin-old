QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/editorsignals.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/penguindata.cpp \
    src/penguinsettings.cpp

HEADERS += \
    include/gui/gui.h \
    include/penguin.h \
    include/penguin/constants.h \
    include/penguin/crc.h \
    include/penguin/savefile.h \
    include/penguin/types.h \
    mainwindow.h \
    penguinsettings.h

INCLUDEPATH += \
    include/ \
    include/gui/

FORMS += \
    mainwindow.ui \
    penguinsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

