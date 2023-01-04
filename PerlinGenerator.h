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
    void PerlinMove(float deltaTime);
    gsl::Vector3D calculateColor(float mLife, gsl::Vector3D mColorBegin, gsl::Vector3D mColorEnd);
    //gsl::Vector3D BarycCoordinates(const gsl::Vector2D& p1, const gsl::Vector2D& p2, const gsl::Vector2D& p3, const gsl::Vector2D& player);
    //float getPositionOnTerrain(const float x, const float z); 

public:

    bool RandomPoints = false, PerlinActive = false;
    float minHeight = 1, maxheight = 10;
    float perlinrange = 1;
    float PerlinSpeed = 5;

private:

    int m_width = 20, m_height = 20;
    float lowestpoint = 0, Highestpoint = 0;

    float PerlinTimer = 0;
    
};


