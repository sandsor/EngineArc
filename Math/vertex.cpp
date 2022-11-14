#include "vertex.h"

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
  os << "(" << v.m_st[0] << ", " << v.m_st[1] << ") ";
  return os;
}
std::istream& operator>> (std::istream& is, Vertex& v) {
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
  is >> dum >> v.m_st[0] >> dum2 >> v.m_st[1] >> dum3;
  return is;
}

Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;

    m_st[0] = s;
    m_st[1] = t;
}

Vertex::Vertex(gsl::Vector3D a, gsl::Vector3D b, gsl::Vector2D c)
{
    m_xyz[0] = a.x;
    m_xyz[1] = a.y;
    m_xyz[2] = a.z;
    m_normal[0] = b.x;
    m_normal[1] = b.y;
    m_normal[2] = b.z;
    m_st[0] = c.x;
    m_st[1] = c.y;
}

gsl::Vector3D Vertex::to3DVec()
{
    gsl::Vector3D temp;
    temp.x = this->m_xyz[0];
    temp.y = this->m_xyz[1];
    temp.z = this->m_xyz[2];

    return temp;
}

std::vector<float> Vertex::findMinMaxXZ(std::vector<Vertex> vertices)
{
    float minX = vertices[0].m_xyz[0];
    float maxX = vertices[0].m_xyz[0];
    float minZ = vertices[0].m_xyz[2];
    float maxZ = vertices[0].m_xyz[2];

    for (int i = 1; i < vertices.size(); i++)
    {
        if(vertices[i].m_xyz[0] < minX)
            minX = vertices[i].m_xyz[0];
        if(vertices[i].m_xyz[0] > maxX)
            maxX = vertices[i].m_xyz[0];
        if(vertices[i].m_xyz[2] < minZ)
            minZ = vertices[i].m_xyz[2];
        if(vertices[i].m_xyz[2] > maxZ)
            maxZ = vertices[i].m_xyz[2];
    }
    return {minX, maxX, minZ, maxZ};
}

