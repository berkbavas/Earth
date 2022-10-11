#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "DummyCamera.h"
#include "RendererManager.h"

#include <QObject>

#include <imgui.h>
#include <QtImGui.h>

class Window;

class Controller : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    void init();
    void mouseDoubleClicked(QMouseEvent *event);
    void mousePressed(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void wheelMoved(QWheelEvent *event);
    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);
    void resize(int w, int h);
    void render(float ifps);
    void setWindow(Window *newWindow);

private:
    QVector<Manager *> mManagers;
    ShaderManager *mShaderManager;
    RendererManager *mRendererManager;

    Window *mWindow;

    Sun *mSun;
    Earth *mEarth;

    DummyCamera *mCamera;

    bool mImGuiWantsMouseCapture;
    bool mImGuiWantsKeyboardCapture;

    bool mSuccess;

    Qt::MouseButton mPressedButton;

    float mX;
    float mY;
    float mZ;
    float mW;
    float mDx;
    float mDy;
    float mDz;
    float mDw;

    bool mUpdate;
    float mDistance;
    float mTilt;
};

#endif // CONTROLLER_H
