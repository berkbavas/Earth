#ifndef EARTH_H
#define EARTH_H

#include "Model.h"

#include <QOpenGLTexture>

class Earth : public Model
{
private:
    explicit Earth(QObject *parent = nullptr);

public:
    static Earth *instance();

    void drawGUI() override;
};

#endif // EARTH_H
