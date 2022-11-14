#include "door.h"

Door::Door()
{

        mVertices.push_back(Vertex{-0.1,-0.5,0.5,0,1,0});
        mVertices.push_back(Vertex{-0.1,0.3,0.5,0,1,0});
        mVertices.push_back(Vertex{0.1,-0.5,0.5,0,1,0});

        mVertices.push_back(Vertex{-0.1,0.3,0.5,0,1,0});
        mVertices.push_back(Vertex{0.1,-0.5,0.5,0,1,0});
        mVertices.push_back(Vertex{0.1,0.3,0.5,0,1,0});

    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
       mMatrix.setToIdentity();
}

Door::~Door()
{

}

void Door::init(GLint matrixUniform)
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

void Door::draw()
{

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}



void Door::open()
{
    if(!bOpen){
        mRotation.rotateY(90);
        mPosition.translate(-0.6,0,0.4);
        mMatrix = mPosition*mRotation;
        bOpen = true;
    } else {
        mRotation.rotateY(-90);
        mPosition.translate(0.6,0,-0.4);
        mMatrix = mPosition*mRotation;
        bOpen = false;
    }
}
