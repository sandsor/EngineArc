#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H


//#include <QOpenGLFunctions_4_1_Core>
//#include <QMatrix4x4>
//#include <vector>
//#include "vertex.h"
#include "visualobject.h"
#include "trianglesurface.h"

class InteractiveObject : public VisualObject
{


public:
    InteractiveObject();
    ~InteractiveObject() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float x, float y, float z)override;
    void collision(VisualObject* gameObject);
    void MoveY(float y);
private:

    TriangleSurface* curve;

    float mx, my, mz; // posisjon

    float distance{0.15};

};


#endif // INTERACTIVEOBJECT_H
