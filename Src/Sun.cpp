#include "Sun.h"

Earth::Sun::Sun()
    : Model("Sphere")
    , mDirection(1, 0, 0)
{
    mAmbient = 1.0f;
    mDiffuse = 1.0f;
    mSpecular = 1.0f;
}