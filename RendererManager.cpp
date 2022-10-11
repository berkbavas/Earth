#include "RendererManager.h"
#include "Helper.h"

#include <QDir>
#include <QImageReader>
#include <QtMath>

RendererManager::RendererManager(QObject *parent)
    : Manager(parent)
    , mCamera(nullptr)
    , mWidth(1600)
    , mHeight(900)
    , mMousePositionFBO(nullptr)
    , mPressedButton(Qt::NoButton)

{}

RendererManager *RendererManager::instance()
{
    static RendererManager instance;
    return &instance;
}

bool RendererManager::init()
{
    mShaderManager = ShaderManager::instance();
    mSun = Sun::instance();
    mEarth = Earth::instance();

    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    qInfo() << "Loading and creating all models...";

    QDir dir("Resources/Models");
    dir.setNameFilters(QStringList("*.obj"));
    auto files = dir.entryList(QDir::Filter::AllEntries);

    for (const auto &file : qAsConst(files))
    {
        QString modelName = file.left(file.lastIndexOf("."));
        ModelData *modelData = Helper::load(dir.path() + "/" + file);
        mModelsData.insert(modelName, modelData);

        if (modelData)
            modelData->create();
    }

    qInfo() << "All models are loaded.";

    mMousePositionFBOFormat.setSamples(0);
    mMousePositionFBOFormat.setAttachment(QOpenGLFramebufferObject::Attachment::Depth);
    mMousePositionFBOFormat.setInternalTextureFormat(GL_RGBA32F);

    mMousePositionFBO = new QOpenGLFramebufferObject(mWidth, mHeight, mMousePositionFBOFormat);

    int limit = QImageReader::allocationLimit();
    QImageReader::setAllocationLimit(0);

    QImage image("Resources/Textures/world.topo.bathy.200411.3x21600x10800.jpg");
    image.mirror();
    mEarthTexture = new QOpenGLTexture(image);
    mEarthTexture->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
    mEarthTexture->setMinMagFilters(QOpenGLTexture::Filter::LinearMipMapLinear, QOpenGLTexture::Filter::Linear);

    QImageReader::setAllocationLimit(limit);

    return true;
}

void RendererManager::resize(int w, int h)
{
    mWidth = w;
    mHeight = h;

    if (mMousePositionFBO)
    {
        delete mMousePositionFBO;
        mMousePositionFBO = new QOpenGLFramebufferObject(mWidth, mHeight, mMousePositionFBOFormat);
    }
}

void RendererManager::setCamera(Camera *camera)
{
    mCamera = camera;
}

void RendererManager::mousePressed(QMouseEvent *event)
{
    mPressedButton = event->button();
}

void RendererManager::mouseReleased(QMouseEvent *event)
{
    mPressedButton = Qt::NoButton;
}

void RendererManager::mouseMoved(QMouseEvent *event)
{
    mMousePositionFBO->bind();
    glReadPixels(event->position().x(), mMousePositionFBO->height() - event->position().y(), 1, 1, GL_RGBA, GL_FLOAT, &mMouseWorldPosition);
    mMousePositionFBO->release();
}

void RendererManager::render(float ifps)
{
    mMousePositionFBO->bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mShaderManager->bind(ShaderManager::ShaderType::EarthMousePositionShader);
    mShaderManager->setUniformValue("MVP", mCamera->getVP() * mEarth->transformation());
    mModelsData.value("Earth")->render();
    mShaderManager->release();

    QOpenGLFramebufferObject::bindDefault();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    mShaderManager->bind(ShaderManager::ShaderType::EarthShader);
    mShaderManager->setUniformValue("M", mEarth->transformation());
    mShaderManager->setUniformValue("N", mEarth->transformation().normalMatrix());
    mShaderManager->setUniformValue("VP", mCamera->getVP());

    mShaderManager->setUniformValue("earthAmbient", mEarth->ambient());
    mShaderManager->setUniformValue("earthDiffuse", mEarth->diffuse());
    mShaderManager->setUniformValue("earthSpecular", mEarth->specular());
    mShaderManager->setUniformValue("earthShininess", mEarth->shininess());

    mShaderManager->setUniformValue("cameraPosition", mCamera->position());
    mShaderManager->setUniformValue("sunDirection", mSun->direction());
    mShaderManager->setUniformValue("sunColor", mSun->color());
    mShaderManager->setUniformValue("sunAmbient", mSun->ambient());
    mShaderManager->setUniformValue("sunDiffuse", mSun->diffuse());
    mShaderManager->setUniformValue("sunSpecular", mSun->specular());
    mEarthTexture->bind(0);
    mShaderManager->setSampler("earthTexture", 0, mEarthTexture->textureId());

    mModelsData.value("Earth")->render();
    mShaderManager->release();
}

void RendererManager::drawGUI()
{
    ImGui::SetNextWindowSize(ImVec2(420, 820), ImGuiCond_FirstUseEver);
    ImGui::Begin("Debug");
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Latitude: %.6f, Longitude: %.6f)", mMouseWorldPosition[0], mMouseWorldPosition[1]);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
