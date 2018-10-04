#ifndef SPRITEINFO_H
#define SPRITEINFO_H

#include "Box2D/Box2D.h"
#include <QString>
#include <iostream>

using namespace std;

struct spriteInfo{

public:

    spriteInfo(float _width, float _height, QString _name, bool _movable){
        width= _width;
        height = _height;
        name=_name;
        movable = _movable;
    }

    float width;
    float height;
    QString name;
    bool movable;

};

#endif // SPRITEINFO_H

