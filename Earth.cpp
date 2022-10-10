#include "Earth.h"

#include <QImageReader>

Earth::Earth(QObject *parent)
    : Model("Earth", parent)
{
    QImageReader::setAllocationLimit(0);

    // Texture
    {
        QImage image("Resources/Textures/world.200409.3x21600x10800.jpg");
        image.mirror();
        mTexture = new QOpenGLTexture(image);
        mTexture->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
        mTexture->setMinMagFilters(QOpenGLTexture::Filter::LinearMipMapLinear, QOpenGLTexture::Filter::Linear);
    }

    // Height Map
    {
        QImage image("Resources/HeightMaps/gebco_08_rev_elev_21600x10800.png");
        image.mirror();
        mHeightMap = new QOpenGLTexture(image);
        mHeightMap->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
        mHeightMap->setMinMagFilters(QOpenGLTexture::Filter::LinearMipMapLinear, QOpenGLTexture::Filter::Linear);
    }

    mAmbient = 1.0f;
}

Earth *Earth::instance()
{
    static Earth instance;
    return &instance;
}

QOpenGLTexture *Earth::texture() const
{
    return mTexture;
}

QOpenGLTexture *Earth::heightMap() const
{
    return mHeightMap;
}

void Earth::drawGUI()
{
    ImGui::SetNextWindowSize(ImVec2(420, 820), ImGuiCond_FirstUseEver);
    ImGui::Begin("Earth");
    Model::drawGUI();
    ImGui::End();
}
