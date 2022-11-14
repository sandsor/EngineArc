#include "visualcurve.h"

VisualCurve::VisualCurve(int c)
{

    if(c == 0)
        construct0();
    else if(c == 1)
        construct1();

    mMatrix.setToIdentity();
}

VisualCurve::~VisualCurve(){}

void VisualCurve::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER,      //what buffer type
                  mVertices.size()*sizeof( Vertex ),   //how big buffer do we need
                  mVertices.data(),             //the actual vertices
                  GL_STATIC_DRAW        //should the buffer be updated on the GPU
                  );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
                3,                  // size
                GL_FLOAT,           // data type
                GL_FALSE,           // normalized?
                sizeof(Vertex),  // stride
                reinterpret_cast<GLvoid*>(0  ));          // array buffer offset
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void VisualCurve::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
}

void VisualCurve::construct0()
{
    float y;

    float xmin=-2.5f, xmax=2.5f, h=0.25f;
    for (auto x=xmin; x<xmax; x+=h)
    {
        y = func1(x);
        mVertices.push_back(Vertex{x,y,0,1,1,1});
    }
}

void VisualCurve::construct1()
{
    float y;

    float xmin=-2.5f, xmax=2.5f, h=0.25f;
    for (auto x=xmin; x<xmax; x+=h)
    {
        y = func2(x);
        mVertices.push_back(Vertex{x,y,0,1,1,1});
    }
}

float VisualCurve::func1(float x)
{
    return -0.0341 * x * x + (0.7019* x) + 0.2915;
}
float VisualCurve::func2(float x)
{
    return 0.417 *(x * x * x)-3.75 * (x * x)+10.333 * x-6;
}
