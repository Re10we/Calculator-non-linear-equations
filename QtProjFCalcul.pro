QT       += core gui printsupport network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Derivative/expression.cxx \
    Derivative/operand.cxx \
    Derivative/parser.cxx \
    Methods/CPPFiles/InstrumentalClass.cpp \
    Methods/CPPFiles/MBroyden.cpp \
    Methods/CPPFiles/MIteration.cpp \
    Methods/CPPFiles/MNewtona.cpp \
    Methods/CPPFiles/MSeidel.cpp \
    Parser/SimpleParser.cpp \
    Interfaces/dialogclients.cpp \
    Interfaces/lobbywindow.cpp \
    Interfaces/mainwindow.cpp \
    Server/server.cpp \
    aboutprogram.cpp \
    helpwindow.cpp \
    main.cpp \
    Graph/qcustomplot.cpp \
    WinDark/winDark.cpp \
    packcreate.cpp

HEADERS += \
    Derivative/auxiliary.h \
    Derivative/common.h \
    Derivative/exceptions.h \
    Derivative/expression.h \
    Derivative/operand.h \
    Derivative/parser.h \
    Derivative/tree.h \
    Methods/HFiles/InstrumentalClass.h \
    Methods/HFiles/MBroyden.h \
    Methods/HFiles/MIteration.h \
    Methods/HFiles/MNewtona.h \
    Methods/HFiles/MSeidel.h \
    Methods/HFiles/Methods.h \
    Parser/SimpleParser.h\
    Interfaces/dialogclients.h \
    Interfaces/lobbywindow.h \
    Interfaces/mainwindow.h \
    Graph/qcustomplot.h \
    Server/server.h \
    WinDark/winDark.h \
    aboutprogram.h \
    helpwindow.h \
    packcreate.h

FORMS += \
    aboutprogram.ui \
    dialogclients.ui \
    helpwindow.ui \
    lobbywindow.ui \
    mainwindow.ui \
    packcreate.ui

TRANSLATIONS =QtLanguage_ukr.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:RC_FILE = file.rc


DISTFILES += \

RESOURCES += \
    Icons.qrc
