QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    controller.cpp \
    main.cpp \
    mainwindow.cpp \
    model/item.cpp \
    model/memo.cpp \
    model/memo_todo.cpp \
    model/reminder.cpp \
    model/todo.cpp \
    model/todo_reminder.cpp \
    widget/editpagewidget.cpp \
    widget/itemwidget.cpp \
    widget/listitemwidget.cpp \
    widget/selectbutton.cpp \
    widget/selectbuttongroup.cpp

HEADERS += \
    controller.h \
    mainwindow.h \
    model/item.h \
    model/memo.h \
    model/memo_todo.h \
    model/model.h \
    model/reminder.h \
    model/todo.h \
    model/todo_reminder.h \
    myvector.h \
    widget/editpagewidget.h \
    widget/itemwidget.h \
    widget/listitemwidget.h \
    widget/selectbutton.h \
    widget/selectbuttongroup.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
