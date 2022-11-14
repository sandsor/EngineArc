#include "house.h"

House::House()
{
    //Kanter på vegg
    Vertex k1(-0.5,-0.5,0.5,1,0,0);
    Vertex k2(0.5,-0.5,0.5,1,0,0);
    Vertex k3(-0.5,0.5,0.5,1,0,0);
    Vertex k4(0.5,0.5,0.5,1,0,0);
    Vertex k5(0.5,-0.5,-0.5,1,0,0);
    Vertex k6(0.5,0.5,-0.5,1,0,0);
    Vertex k7(-0.5,-0.5,-0.5,1,0,0);
    Vertex k8(-0.5,0.5,-0.5,1,0,0);

    //Nye kanter på gulv for å få annen farge uten å påvirke veggene
    //1257
    Vertex gk1(-0.5,-0.5,0.5,0,0,1);
    Vertex gk2(0.5,-0.5,0.5,0,0,1);
    Vertex gk3(0.5,-0.5,-0.5,0,0,1);
    Vertex gk4(-0.5,-0.5,-0.5,0,0,1);

    //Kanter på dør
    Vertex kd1(-0.1,-0.5,0.5,1,0,0);
    Vertex kd2(-0.1,0.5,0.5,1,0,0);
    Vertex kd3(0.1,-0.5,0.5,1,0,0);
    Vertex kd4(0.1,0.5,0.5,1,0,0);


    //Kanter over dør
    Vertex kod1(-0.1,0.3,0.5,1,0,0);
    Vertex kod2(0.1,0.3,0.5,1,0,0);

    //side(k1,k2,k3,k4);
    side(k1,kd1,k3,kd2);
    side(kd3,k2,kd4,k4);
    side(kod1, kod2, kd2, kd4);
//    side(kd1,kd4,kd2,kd3);

    side(k2,k5,k4,k6);
    side(k5,k7,k6,k8);
    side(k7,k1,k8,k3);
    //side(k3,k4,k8,k6);
//    side(k7,k5,k1,k2);

    side(gk1, gk2, gk4, gk3);

       mMatrix.setToIdentity();

    //!Fyll ut huset unntatt døra, lag dørklasse
}

House::~House(){}

void House::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void House::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void House::side(Vertex m_k1, Vertex m_k2, Vertex m_k3, Vertex m_k4)
{
    mVertices.push_back(m_k1);
    mVertices.push_back(m_k2);
    mVertices.push_back(m_k3);

    mVertices.push_back(m_k2);
    mVertices.push_back(m_k3);
    mVertices.push_back(m_k4);
}
