#include "Controller.h"
#include "Window.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

Earth::Controller::Controller(QObject* parent)
    : QObject(parent)
    , mZoomLevel(100)
    , mUpdate(false)
    , mDistance(40.0f)
    , mTilt(0.0f)
    , mWidth(1600)
    , mHeight(900)
    , mMousePositionFBO(nullptr)
    , mPressedButton(Qt::NoButton)
{}

void Earth::Controller::Init()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mShaderManager = new ShaderManager;

    mShaderManager->Init();

    mSun = new Sun;
    mEarth = new Model("Earth");

    mMousePositionFBOFormat.setSamples(0);
    mMousePositionFBOFormat.setAttachment(QOpenGLFramebufferObject::Attachment::Depth);
    mMousePositionFBOFormat.setInternalTextureFormat(GL_RGBA32F);

    mMousePositionFBO = new QOpenGLFramebufferObject(mWidth, mHeight, mMousePositionFBOFormat);

    QImage image("Resources/Textures/world.topo.bathy.200411.3x21600x10800.jpg");
    image = image.mirrored();
    mEarthTexture = new QOpenGLTexture(image);
    mEarthTexture->setWrapMode(QOpenGLTexture::WrapMode::Repeat);
    mEarthTexture->setMinMagFilters(QOpenGLTexture::Filter::LinearMipMapLinear, QOpenGLTexture::Filter::Linear);

    mCamera = new DummyCamera;
    mCamera->SetPosition(QVector3D(0, 0, mDistance));
    mCamera->SetVerticalFov(60.0f);
    mCamera->SetZNear(0.01f);
    mCamera->SetZFar(100000.0f);

    mEarth->SetPosition(QVector3D(0, 0, 0));

    LoadModels();
}

void Earth::Controller::Render(float ifps)
{
    if (mUpdate)
    {
        // Earth
        {
            auto rotation = mEarth->Rotation();
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), -mMouse.mDz * ifps * qMax(2.0f, mDistance - 10)) * rotation;
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), -mMouse.mDx * ifps * (mDistance - 10)) * rotation;
            rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -mMouse.mDy * ifps * (mDistance - 10)) * rotation;
            mEarth->SetRotation(rotation);
        }

        // Camera
        {
            mTilt -= mMouse.mDw * ifps * 30;
            mTilt = qBound(0.0f, mTilt, 89.0f);

            auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mTilt);
            auto position = QVector3D(0, 0, 10) + rotation * QVector3D(0, 0, mDistance - 10);

            mCamera->SetRotation(rotation);
            mCamera->SetPosition(position);
        }

        mMouse.mDx = 0.0f;
        mMouse.mDy = 0.0f;
        mMouse.mDz = 0.0f;
        mMouse.mDw = 0.0f;
        mUpdate = false;
    }

    mSun->SetDirection(mCamera->GetViewDirection());

    // Render
    {
        mMousePositionFBO->bind();
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mShaderManager->Bind(ShaderManager::ShaderType::EarthMousePositionShader);
        mShaderManager->SetUniformValue("MVP", mCamera->GetViewProjectionMatrix() * mEarth->Transformation());
        if (mModelsData.contains("Earth"))
            mModelsData.value("Earth")->Render();
        mShaderManager->Release();

        QOpenGLFramebufferObject::bindDefault();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        mShaderManager->Bind(ShaderManager::ShaderType::EarthShader);
        mShaderManager->SetUniformValue("M", mEarth->Transformation());
        mShaderManager->SetUniformValue("N", mEarth->Transformation().normalMatrix());
        mShaderManager->SetUniformValue("VP", mCamera->GetViewProjectionMatrix());
        mShaderManager->SetUniformValue("earthAmbient", mEarth->GetAmbient());
        mShaderManager->SetUniformValue("earthDiffuse", mEarth->GetDiffuse());
        mShaderManager->SetUniformValue("earthSpecular", mEarth->GetSpecular());
        mShaderManager->SetUniformValue("earthShininess", mEarth->GetShininess());
        mShaderManager->SetUniformValue("cameraPosition", mCamera->Position());
        mShaderManager->SetUniformValue("sunDirection", mSun->GetDirection());
        mShaderManager->SetUniformValue("sunColor", mSun->GetColor());
        mShaderManager->SetUniformValue("sunAmbient", mSun->GetAmbient());
        mShaderManager->SetUniformValue("sunDiffuse", mSun->GetDiffuse());
        mShaderManager->SetUniformValue("sunSpecular", mSun->GetSpecular());
        mEarthTexture->bind(0);
        mShaderManager->SetSampler("earthTexture", 0, mEarthTexture->textureId());
        if (mModelsData.contains("Earth"))
            mModelsData.value("Earth")->Render();
        mShaderManager->Release();
    }

    QtImGui::newFrame();
    glViewport(0, 0, mWindow->width(), mWindow->height());
    ImGui::SetNextWindowSize(ImVec2(420, 820), ImGuiCond_FirstUseEver);
    ImGui::Begin("Debug");
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Latitude: %.6f, Longitude: %.6f)", mMouseWorldPosition[0], mMouseWorldPosition[1]);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    QtImGui::render();
}

void Earth::Controller::LoadModels()
{
    qInfo() << "Loading and creating all models...";

    QDir dir("Resources/Models");
    dir.setNameFilters(QStringList("*.obj"));
    auto files = dir.entryList(QDir::Filter::AllEntries);

    for (const auto& file : qAsConst(files))
    {
        QString modelName = file.left(file.lastIndexOf("."));
        ModelData* modelData = Earth::Helper::LoadModelData(dir.path() + "/" + file);
        mModelsData.insert(modelName, modelData);

        if (modelData)
            modelData->Create();
    }

    qInfo() << "All models are loaded.";
}

void Earth::Controller::WheelMoved(QWheelEvent* event)
{
    if (event->angleDelta().y() < 0)
        mDistance = mDistance / 0.95;

    if (event->angleDelta().y() > 0)
        mDistance = mDistance * 0.95;

    mDistance = qBound(10.0f + 2 * mCamera->GetZNear(), mDistance, 1000.0f);

    mUpdate = true;
}

void Earth::Controller::MousePressed(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMouse.mX = event->pos().x();
        mMouse.mY = event->pos().y();
        mMouse.mPressedButton = Qt::LeftButton;
    }

    if (event->button() == Qt::MiddleButton)
    {
        mMouse.mZ = event->pos().y();
        mMouse.mPressedButton = Qt::MiddleButton;
    }

    if (event->button() == Qt::RightButton)
    {
        mMouse.mW = event->pos().y();
        mMouse.mPressedButton = Qt::RightButton;
    }
}

void Earth::Controller::MouseReleased(QMouseEvent* event)
{
    mMouse.mPressedButton = Qt::NoButton;
}

void Earth::Controller::MouseMoved(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (mMouse.mPressedButton == Qt::LeftButton)
    {
        mMouse.mDx += mMouse.mX - event->pos().x();
        mMouse.mDy += mMouse.mY - event->pos().y();

        mMouse.mX = event->pos().x();
        mMouse.mY = event->pos().y();

        mUpdate = true;
    }

    if (mMouse.mPressedButton == Qt::MiddleButton)
    {
        mMouse.mDz += mMouse.mZ - event->pos().y();
        mMouse.mZ = event->pos().y();

        mUpdate = true;
    }

    if (mMouse.mPressedButton == Qt::RightButton)
    {
        mMouse.mDw += mMouse.mW - event->pos().y();
        mMouse.mW = event->pos().y();
        mUpdate = true;
    }

    mMousePositionFBO->bind();
    glReadPixels(event->pos().x(), mMousePositionFBO->height() - event->pos().y(), 1, 1, GL_RGBA, GL_FLOAT, &mMouseWorldPosition);
    mMousePositionFBO->release();
}

void Earth::Controller::KeyPressed(QKeyEvent* event)
{
}

void Earth::Controller::KeyReleased(QKeyEvent* event)
{
}

void Earth::Controller::Resize(int w, int h)
{
    mWindow->makeCurrent();
    mCamera->Resize(w, h);
    mWidth = w;
    mHeight = h;

    if (mMousePositionFBO)
    {
        delete mMousePositionFBO;
        mMousePositionFBO = new QOpenGLFramebufferObject(mWidth, mHeight, mMousePositionFBOFormat);
    }

    mWindow->doneCurrent();
}

void Earth::Controller::MouseDoubleClicked(QMouseEvent*)
{
}

void Earth::Controller::SetWindow(Window* newWindow)
{
    mWindow = newWindow;
}