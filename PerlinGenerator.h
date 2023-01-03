#pragma once

#include "visualobject.h"
#include "vector3d.h"
#include "FuncBin.h"


class PerlinGenerator : public VisualObject 

{
public:
    PerlinGenerator();
    void init(GLint matrixUniform) override;
    void draw() override;
    void Generate();
    void RandomMove();
    bool RandomPoints = true;
    gsl::Vector3D calculateColor(float mLife, gsl::Vector3D mColorBegin, gsl::Vector3D mColorEnd);
    //gsl::Vector3D BarycCoordinates(const gsl::Vector2D& p1, const gsl::Vector2D& p2, const gsl::Vector2D& p3, const gsl::Vector2D& player);
    //float getPositionOnTerrain(const float x, const float z);
private:

    int m_width = 50;
    int m_height = 50;

    int minHeight = 1;
    int maxheight = 10;

    unsigned char* m_localBuffer;
    class Logger* mLogger{ nullptr };
    gsl::Vector3D ofpoint;
    
};


