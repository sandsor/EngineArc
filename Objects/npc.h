#ifndef NPC_H
#define NPC_H

#include "visualobject.h"
#include "visualcurve.h"

class NPC : public VisualObject
{
public:
    NPC();
    ~NPC();
    void init(GLint matrixUniform) override;
    void draw() override;
    void move();
    void switchGraph();


private:
    Vertex k1, k2, k3, k4, k5, k6, k7, k8;
    void side(Vertex m_k1, Vertex m_k2, Vertex m_k3, Vertex m_k4);
    float mx{-2}, dt{0.015};
    VisualCurve* curve;
    bool bCurrentGraph{false};
};

#endif // NPC_H
