#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "vsflate.h"
#include "trianglesurface.h"


class RollingBall : public OctahedronBall
{
public:
//    RollingBall(int n, VSflate* ts);
      RollingBall(int n, TriangleSurface* ts);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;
//    void setSurface(VisualObject* surface) { Triangle = surface; }
       // void setSurface(VisualObject* surface) { Vsflate = surface; }

    gsl::Vector3D BarycCoordinates(const gsl::Vector2D& p1, const gsl::Vector2D& p2, const gsl::Vector2D& p3, const gsl::Vector2D& player);
    gsl::Vector3D FinnAkselerasjonsvektor(const gsl::Vector3D& normalvector);
protected:
    VisualObject* ts;
        //VisualObject* Vsflate;
private:
    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    int allScale = 10;
    gsl::Vector3D mPos{-20.2,5,0};
    gsl::Vector3D v = {0,0,0};
    int oldIndex = 0;
    gsl::Vector3D oldNormalVector = {0,0,0};
    gsl::Vector3D normalvector = {0,0,0};
        float gr = 0.001;
    std::vector<float> MinMax;
    //std::vector<Vertex>& vertices = dynamic_cast<VSflate*>(Vsflate)->getVertices(); //henter vertecies ifra renderwindow(vsflate object)

    std::vector<Vertex>& vertices = dynamic_cast<TriangleSurface*>(ts)->getVertices(); //henter vertecies ifra renderwindow(vsflate object)

};

#endif // ROLLINGBALL_H
