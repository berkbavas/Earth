#include "Earth.h"

#include <QImageReader>

Earth::Earth(QObject *parent)
    : Model("Earth", parent)
{
    mAmbient = 1.0f;
}

Earth *Earth::instance()
{
    static Earth instance;
    return &instance;
}

void Earth::drawGUI()
{
    ImGui::SetNextWindowSize(ImVec2(420, 820), ImGuiCond_FirstUseEver);
    ImGui::Begin("Earth");
    Model::drawGUI();
    ImGui::End();
}
