QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    composewindow.cpp \
    copiesdao.cpp \
    copiesmanager.cpp \
    csvfile.cpp \
    hashtable.cpp \
    mail.cpp \
    maildao.cpp \
    mailindex.cpp \
    mailmanager.cpp \
    mailsearcher.cpp \
    main.cpp \
    mainwindow.cpp \
    modifywindow.cpp \
    passwordbox.cpp \
    primaryindex.cpp \
    searcheswindow.cpp \
    secondaryindex.cpp

HEADERS += \
    avlnode.h \
    avltree.h \
    composewindow.h \
    copiesdao.h \
    copiesmanager.h \
    csvfile.h \
    hashtable.h \
    list.h \
    mail.h \
    maildao.h \
    mailindex.h \
    mailmanager.h \
    mailsearcher.h \
    mainwindow.h \
    modifywindow.h \
    passwordbox.h \
    primaryindex.h \
    searcheswindow.h \
    secondaryindex.h

FORMS += \
    composewindow.ui \
    mainwindow.ui \
    modifywindow.ui \
    passwordbox.ui \
    searcheswindow.ui

RC_ICONS = icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
