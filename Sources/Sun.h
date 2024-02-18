#pragma once

#include "Model.h"

#include <QVector4D>
#include <QVector3D>

namespace Earth
{
    class Sun : public Model
    {
    public:
        Sun();

    protected:
        DEFINE_MEMBER(QVector3D, Direction);
    };
}
