#pragma once

#include <QObject>

namespace Earth
{
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
}
