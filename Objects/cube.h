#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Cube : public VisualObject
{
//private:
//   std::vector<Vertex> mVertices;
//   //QOpenGLContext *mContext;

public:
    Cube();
    ~Cube() override;
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // CUBE_H