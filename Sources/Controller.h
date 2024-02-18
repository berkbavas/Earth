#pragma once

#include "DummyCamera.h"
#include "Mouse.h"
#include "Model.h"
#include "Helper.h"
#include "ShaderManager.h"
#include "Sun.h"

#include <QObject>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObjectFormat>

#include <imgui.h>
#include <QtImGui.h>

namespace Earth
{
    class Window;

    class Controller : public QObject, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        explicit Controller(QObject* parent = nullptr);

        void Init();
        void MouseDoubleClicked(QMouseEvent* event);
        void MousePressed(QMouseEvent* event);
        void MouseReleased(QMouseEvent* event);
        void MouseMoved(QMouseEvent* event);
        void WheelMoved(QWheelEvent* event);
        void KeyPressed(QKeyEvent* event);
        void KeyReleased(QKeyEvent* event);
        void Resize(int w, int h);
        void Render(float ifps);
        void SetWindow(Window* newWindow);

    private:
        void LoadModels();

    private:
        ShaderManager* mShaderManager;
        Window* mWindow;

        Sun* mSun;
        Model* mEarth;
        DummyCamera* mCamera;

        Mouse mMouse;
        int mZoomLevel;

        bool mUpdate;
        float mDistance;
        float mTilt;

        QMap<QString, ModelData*> mModelsData;
        QOpenGLTexture* mEarthTexture;

        int mWidth;
        int mHeight;

        // Mouse
        QOpenGLFramebufferObjectFormat mMousePositionFBOFormat;
        QOpenGLFramebufferObject* mMousePositionFBO;
        QVector4D mMouseWorldPosition;
        Qt::MouseButton mPressedButton;
    };
}
