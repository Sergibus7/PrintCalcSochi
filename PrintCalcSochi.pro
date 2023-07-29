QT       += core gui printsupport #network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a
# QMAKE_CXXFLAGS += /std:c++20
RC_ICONS = ColoritIcon.ico
#INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt"
#LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64"

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AboutDlg.cpp \
    DeleteImpDlg.cpp \
    ImpWizard.cpp \
    LogWindow.cpp \
    MsgBox.cpp \
    Product.cpp \
    ProductDlg.cpp \
    SelectImpDlg.cpp \
    SettingsDlg.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    AboutDlg.hpp \
    DeleteImpDlg.h \
    Globals.h \
    ImpWizard.h \
    LogWindow.h \
    MainWindow.h \
    MsgBox.h \
    OrderDlg.h \
    Product.h \
    ProductDlg.h \
    SelectImpDlg.h \
    SettingsDlg.h

FORMS += \
    AboutDlg.ui \
    DeleteImpDlg.ui \
    ImpWizard.ui \
    LogWindow.ui \
    MainWindow.ui \
    ProductDlg.ui \
    SelectImpDlg.ui \
    SettingsDlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
