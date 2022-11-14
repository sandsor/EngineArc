#ifndef VISUALCURVE_H
#define VISUALCURVE_H

#include "visualobject.h"

class VisualCurve : public VisualObject
{
public:
    VisualCurve(int t);
    ~VisualCurve() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void construct0();
    void construct1();

    float func1(float x);
    float func2(float x);
};

#endif // VISUALCURVE_H
