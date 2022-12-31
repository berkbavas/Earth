#include "Model.h"

Earth::Model::Model(const QString& modelName, QObject* parent)
    : Node(parent)
    , mColor(1, 1, 1, 1)
    , mAmbient(0.5f)
    , mDiffuse(1.0f)
    , mSpecular(0.25f)
    , mShininess(4.0f)
{
    mName = modelName;
}

Earth::Model::~Model() {}