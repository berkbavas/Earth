#pragma once

#include "Node.h"

namespace Earth
{
    class Model : public Node
    {
    public:
        explicit Model(const QString& modelName, QObject* parent = nullptr);
        virtual ~Model();

    protected:
        DEFINE_MEMBER(QVector4D, Color);
        DEFINE_MEMBER(float, Ambient);
        DEFINE_MEMBER(float, Diffuse);
        DEFINE_MEMBER(float, Specular);
        DEFINE_MEMBER(float, Shininess);
        DEFINE_MEMBER(QString, Name);
    };
}
