#ifndef MOUSE_H
#define MOUSE_H

#include <QObject>

class Mouse
{
public:
    Mouse();

    Qt::MouseButton mPressedButton;
    float mX;
    float mY;
    float mZ;
    float mW;
    float mDx;
    float mDy;
    float mDz;
    float mDw;
};

#endif // MOUSE_H
