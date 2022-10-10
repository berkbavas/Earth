#include "Model.h"

Model::Model(const QString &modelName, QObject *parent)
    : Node(parent)
    , mColor(1, 1, 1, 1)
    , mAmbient(0.5f)
    , mDiffuse(0.75f)
    , mSpecular(0.25f)
    , mShininess(4.0f)
{
    mName = modelName;
    mNodeType = Node::NodeType::Model;
}

Model::~Model() {}

void Model::drawGUI()
{
    // Shading Parameters
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Shading Parameters:");
        ImGui::SliderFloat("Ambient##Model", &mAmbient, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Model", &mDiffuse, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Model", &mSpecular, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Shininess##Model", &mShininess, 1.0f, 128.0f, "%.3f");
        ImGui::ColorEdit4("Color##Model", (float *) &mColor);
    }

    Node::drawGUI();
}

const QVector4D &Model::color() const
{
    return mColor;
}

void Model::setColor(const QVector4D &newColor)
{
    mColor = newColor;
}

float Model::ambient() const
{
    return mAmbient;
}

void Model::setAmbient(float newAmbient)
{
    mAmbient = newAmbient;
}

float Model::diffuse() const
{
    return mDiffuse;
}

void Model::setDiffuse(float newDiffuse)
{
    mDiffuse = newDiffuse;
}

float Model::specular() const
{
    return mSpecular;
}

void Model::setSpecular(float newSpecular)
{
    mSpecular = newSpecular;
}

float Model::shininess() const
{
    return mShininess;
}

void Model::setShininess(float newShininess)
{
    mShininess = newShininess;
}
