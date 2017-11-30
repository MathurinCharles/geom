# define your application name here 
TARGET = yourAppName

# add your source cpp files here 
SOURCES += \
    curve1DBezierParam.cpp \
    curve1DBezier.cpp \
    main.cpp \
    mainWidget.cpp \
    toolsWidget.cpp \
    drawingWidgetItems.cpp \
    drawingWidget.cpp \
    animationWidgetItems.cpp \
    animationWidget.cpp \
    scene.cpp \
    animatedPoint.cpp 

# add your header files here
HEADERS += \
    mainWidget.h \
    toolsWidget.h \
    drawingWidgetItems.h \
    drawingWidget.h \
    animationWidgetItems.h \
    animationWidget.h \
    scene.h \
    curve1D.h \
    curve2D.h \
    curve1DLinear.h \
    curve1DBezierParam.h \
    curve1DBezier.h \
    curve2DLinear.h \
    curve2DBezierBasic.h \
    curve2DBezier.h \
    animatedPoint.h 

TEMPLATE = app

INCLUDEPATH += eigen

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-misleading-indentation -Wno-deprecated-declarations

QT += core gui widgets
