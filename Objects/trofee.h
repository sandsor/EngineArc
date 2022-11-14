#ifndef TROFEE_H
#define TROFEE_H

#include "visualobject.h"

class Trofee : public VisualObject
{
public:
    Trofee();
    Trofee(std::string name, float mx, float my, float mz);
    ~Trofee();
    void init(GLint matrixUniform) override;
    void draw() override;
    void readFile();
};

#endif // TROFEE_H
