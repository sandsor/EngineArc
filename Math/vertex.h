#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include "vector3d.h"
#include "vector2d.h"
#include "gltypes.h"
#include <iostream>
#include <fstream>

class Vertex {
    //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    friend std::ostream& operator<< (std::ostream&, const Vertex&);

    //! Overloaded ostream operator which reads all vertex data from an open textfile stream
    friend std::istream& operator>> (std::istream&, Vertex&);
public:
   Vertex();
   Vertex(float x, float y, float z, float r, float g, float b, float s=0.f, float t=0.f);
   Vertex(gsl::Vector3D a, gsl::Vector3D b, gsl::Vector2D c);


//private:
   float m_xyz[3];       	// position i 3D
   float m_normal[3];    // normal in 3D or rgb colors
   float m_st[2];        	// texture coordinates if used

    gsl::Vector3D to3DVec();
    static std::vector<float> findMinMaxXZ(std::vector<Vertex> vertices);
};
#endif // VERTEX_H
