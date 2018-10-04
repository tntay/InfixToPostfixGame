#-------------------------------------------------
#
# Project created by QtCreator 2017-11-29T10:25:07
#
#-------------------------------------------------

QT       += core gui \
            multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InfixToPostfixGame
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


 SOURCES += \
        main.cpp \
        view.cpp \
    model.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/Shapes/b2ChainShape.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/Shapes/b2CircleShape.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/b2BroadPhase.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/b2CollideCircle.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/b2CollideEdge.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/b2CollidePolygon.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/b2Collision.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/b2Distance.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/b2DynamicTree.cpp \
    ../../Box2D-master/Box2D/Box2D/Collision/b2TimeOfImpact.cpp \
    ../../Box2D-master/Box2D/Box2D/Common/b2BlockAllocator.cpp \
    ../../Box2D-master/Box2D/Box2D/Common/b2Draw.cpp \
    ../../Box2D-master/Box2D/Box2D/Common/b2Math.cpp \
    ../../Box2D-master/Box2D/Box2D/Common/b2Settings.cpp \
    ../../Box2D-master/Box2D/Box2D/Common/b2StackAllocator.cpp \
    ../../Box2D-master/Box2D/Box2D/Common/b2Timer.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2Contact.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2Joint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/b2Body.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/b2ContactManager.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/b2Fixture.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/b2Island.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/b2World.cpp \
    ../../Box2D-master/Box2D/Box2D/Dynamics/b2WorldCallbacks.cpp \
    ../../Box2D-master/Box2D/Box2D/Rope/b2Rope.cpp \
    equations.cpp

HEADERS += \
        view.h \
    model.h \
    spriteinfo.h \
    equations.h

FORMS += \
        view.ui

win32: LIBS += -L$$PWD\..\..\SFML-2.4.2-windows\lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

win32: LIBS += -L$$PWD\..\..\SFML-2.4.2-windows\bin

win32: INCLUDEPATH += $$PWD\..\..\SFML-2.4.2-windows\include

win32: INCLUDEPATH += $$PWD\..\..\Box2D-master\Box2D


#Yoonki
#win32: LIBS += -L$$PWD\..\..\..\SFML-2.4.2-windows\lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

#win32: LIBS += -L$$PWD\..\..\..\SFML-2.4.2-windows\bin

#win32: INCLUDEPATH += $$PWD\..\..\..\SFML-2.4.2-windows\include

#win32: INCLUDEPATH += $$PWD\..\..\..\Box2D-master\Box2D

#win32: LIBS += -L$$PWD/../../../Box2D-master/ -lBox2D


RESOURCES += \
    resources.qrc \



macx: LIBS += -L$$PWD/../../SFML-2.4.2-osx-clang/lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
macx: INCLUDEPATH += $$PWD/../../SFML-2.4.2-osx-clang/include
macx: DEPENDPATH += $$PWD/../../SFML-2.4.2-osx-clang/include
macx: INCLUDEPATH += $$PWD/../../Box2D-master/Box2D
macx: DEPENDPATH += $$PWD/../../Box2D-master/Box2D
macx: QMAKE_POST_LINK="install_name_tool -add_rpath $$PWD/../../SFML-2.4.2-osx-clang/lib $$OUT_PWD/InfixToPostfixGame.app/Contents/MacOS/InfixToPostfixGame"
