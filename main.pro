# define your application name here
TARGET = yourAppName

# add your source cpp files here
SOURCES += \
    main.cpp \
    mainWidget.cpp \
    toolsWidget.cpp \
    drawingWidgetItems.cpp \
    drawingWidget.cpp \
    animationWidgetItems.cpp \
    animationWidget.cpp \
    scene.cpp \
    animatedPoint.cpp \
    curve1DBezier.cpp \
    curve1DBezierParam.cpp

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
    curve1DBezier.h \
    curve1DBezierParam.h \
    curve2DLinear.h \
    curve2DBspline.h \
    curve2DClosedBspline.h \
    curve2DLinearClosed.h \
    curve2DBezier.h \
    curve2DCatmullRomClosed.h \
    curve2DCubicHermiteSpline.h \
    curve2DClosedCubicHermiteSpline.h \
    animatedPoint.h

TEMPLATE = app

INCLUDEPATH += eigen

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-misleading-indentation -Wno-deprecated-declarations

QT += core gui widgets
