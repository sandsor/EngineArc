#ifndef HOUSE_H
#define HOUSE_H

#include "cube.h"
#include "visualobject.h"

class House : public VisualObject
{
public:
    House();
    ~House();
    void init(GLint matrixUniform) override;
    void draw() override;
    Vertex k1, k2, k3, k4, k5, k6, k7, k8, dk1, dk2, dk3, dk4;
private:
    std::vector<Vertex> mVertices;
    //QOpenGLContext *mContext;
    void side(Vertex m_k1, Vertex m_k2, Vertex m_k3, Vertex m_k4);
};

#endif // HOUSE_H
