QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

INCLUDEPATH += ./Functionality
INCLUDEPATH += ./MainClasses
INCLUDEPATH += ./Math
INCLUDEPATH += ./Objects


SOURCES += main.cpp \
    Functionality/camera.cpp \
    Functionality/objloader.cpp \
    Objects/cube.cpp \
    Objects/disc.cpp \
    Objects//door.cpp \
    Objects/generate.cpp \
    Objects/house.cpp \
    Functionality/interactiveobject.cpp \
    Objects/light.cpp \
    logger.cpp \
    mainwindow.cpp \
    Math/matrix3x3.cpp \
    Math/matrix4x4.cpp \
    Objects/npc.cpp \
    Objects/octahedronball.cpp \
    Objects/plan.cpp \
    Objects/quadtre.cpp \
    Objects/quadtree.cpp \
    MainClasses/renderwindow.cpp \
    Objects/rollingball.cpp \
    Functionality/shader.cpp \
    Functionality/terrain.cpp \
    Functionality/texture.cpp \
    Math/trianglesurface.cpp \
    Objects/trofee.cpp \
    Math/vector2d.cpp \
    Math/vector3d.cpp \
    Math/vertex.cpp \
    Math/visualcurve.cpp \
    MainClasses/visualobject.cpp \
    Objects/vsflate.cpp \
    Objects/xyz.cpp

HEADERS += \
    Functionality/camera.h \
    Functionality/objloader.h \
    Objects/cube.h \
    Objects/disc.h \
    Objects/door.h \
    Math/gltypes.h \
    Objects/generate.h \
    Objects/house.h \
    Functionality/interactiveobject.h \
    Objects/light.h \
    logger.h \
    mainwindow.h \
    Math/math_constants.h \
    Math/matrix3x3.h \
    Math/matrix4x4.h \
    Objects/npc.h \
    Objects/octahedronball.h \
    Objects/plan.h \
    Objects/quadtre.h \
    Objects/quadtree.h \
    MainClasses/renderwindow.h \
    Objects/rollingball.h \
    Functionality/shader.h \
    Math/stb_image.h \
    Functionality/terrain.h \
    Functionality/texture.h \
    Math/trianglesurface.h \
    Objects/trofee.h \
    Math/vector2d.h \
    Math/vector3d.h \
    Math/vertex.h \
    Math/visualcurve.h \
    MainClasses/visualobject.h \
    Objects/vsflate.h \
    Objects/xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Phongshader.frag \
    Phongshader.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert
