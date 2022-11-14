#ifndef TERRAIN_H
#define TERRAIN_H


#include "visualobject.h"

class Terrain : public VisualObject

{
public:
    Terrain();
    void init(GLint matrixUniform) override;
    void draw() override;
    void setPlane();
    gsl::Vector3D BarycCoordinates(const gsl::Vector2D& p1, const gsl::Vector2D& p2, const gsl::Vector2D& p3, const gsl::Vector2D& player);
    float getPositionOnTerrain(const float x, const float z);
private:
    int m_width, m_height;
    unsigned char* m_localBuffer;
    class Logger *mLogger{nullptr};
};

#endif // TERRAIN_H
