// VisualObject.cpp
#include "visualobject.h"

VisualObject::VisualObject() {

}

VisualObject::VisualObject(std::string name) : m_name(name)
{

}

VisualObject::~VisualObject()
{
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

std::pair<float, float> VisualObject::getPosition2D()
{
    return std::pair<float,float>(mMatrix.getPosition2D().getX(), mMatrix.getPosition2D().getY());
}

std::vector<Vertex> VisualObject::getVertices()
{
    return mVertices;
}

