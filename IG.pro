QT       += core gui serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    deletemsgbutton.cpp \
    igtable.cpp \
    main.cpp \
    mainwindow.cpp \
    serialbus/bitratebox.cpp \
    serialbus/connectdialog.cpp

HEADERS += \
    deletemsgbutton.h \
    igtable.h \
    mainwindow.h \
    serialbus/bitratebox.h \
    serialbus/connectdialog.h

FORMS += \
    mainwindow.ui \
    serialbus/connectdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ig.qrc

