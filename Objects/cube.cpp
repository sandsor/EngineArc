#include "cube.h"

Cube::Cube()
{

        mVertices.push_back(Vertex{-0.5,-0.5,-0.5,1,0,0}); //Triangle 1 , Side 1
        mVertices.push_back(Vertex{-0.5,-0.5,0.5,1,0,0});
        mVertices.push_back(Vertex{-0.5,0.5,0.5,1,0,0});

        mVertices.push_back(Vertex{0.5,0.5,-0.5,1,0,0}); //Triangle 2
        mVertices.push_back(Vertex{-0.5,-0.5,-0.5,1,0,0});
        mVertices.push_back(Vertex{-0.5,0.5,-0.5,1,0,0});

        mVertices.push_back(Vertex{0.5,-0.5,0.5,0,1,0}); //Triangle 3 , Side 2
        mVertices.push_back(Vertex{-0.5,-0.5,-0.5,0,1,0});
        mVertices.push_back(Vertex{0.5,-0.5,-0.5,0,1,0});

        mVertices.push_back(Vertex{0.5,0.5,-0.5,0,1,0}); //Triangle 4
        mVertices.push_back(Vertex{0.5,-0.5,-0.5,0,1,0});
        mVertices.push_back(Vertex{-0.5,-0.5,-0.5,0,1,0});

        mVertices.push_back(Vertex{-0.5,-0.5,-0.5,0,0,1}); //Triangle 5 , Side 3
        mVertices.push_back(Vertex{-0.5,0.5,0.5,0,0,1});
        mVertices.push_back(Vertex{-0.5,0.5,-0.5,0,0,1});

        mVertices.push_back(Vertex{0.5,-0.5,0.5,0,0,1}); //Triangle 6
        mVertices.push_back(Vertex{-0.5,-0.5,0.5,0,0,1});
        mVertices.push_back(Vertex{-0.5,-0.5,-0.5,0,0,1});

        mVertices.push_back(Vertex{-0.5,0.5,0.5,1,1,0}); //Triangle 7 , Side 4
        mVertices.push_back(Vertex{-0.5,-0.5,0.5,1,1,0});
        mVertices.push_back(Vertex{0.5,-0.5,0.5,1,1,0});

        mVertices.push_back(Vertex{0.5,0.5,0.5,1,1,0}); //Triangle 8
        mVertices.push_back(Vertex{0.5,-0.5,-0.5,1,1,0});
        mVertices.push_back(Vertex{0.5,0.5,-0.5,1,1,0});

        mVertices.push_back(Vertex{0.5,-0.5,-0.5,1,0,1}); //Triangle 9 , Side 5
        mVertices.push_back(Vertex{0.5,0.5,0.5,1,0,1});
        mVertices.push_back(Vertex{0.5,-0.5,0.5,1,0,1});

        mVertices.push_back(Vertex{0.5,0.5,0.5,1,0,1}); //Triangle 10
        mVertices.push_back(Vertex{0.5,0.5,-0.5,1,0,1});
        mVertices.push_back(Vertex{-0.5,0.5,-0.5,1,0,1});

        mVertices.push_back(Vertex{0.5,0.5,0.5,0,1,1}); //Triangle 11 , Side 6
        mVertices.push_back(Vertex{-0.5,0.5,-0.5,0,1,1});
        mVertices.push_back(Vertex{-0.5,0.5,0.5,0,1,1});

        mVertices.push_back(Vertex{0.5,0.5,0.5,0,1,1}); //Triangle 12
        mVertices.push_back(Vertex{-0.5,0.5,0.5,0,1,1});
        mVertices.push_back(Vertex{0.5,-0.5,0.5,0,1,1});
        mMatrix.setToIdentity();


}

Cube::~Cube()
{

}

void Cube::init(GLint matrixUniform) {
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
void Cube::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Cube::ChangeColor(float r, float g, float b)
{
    for (int i = 0; i < mVertices.size(); i++)
    {
        Vertex tempVertex = mVertices[i];
        tempVertex.m_normal[0] = r;
        tempVertex.m_normal[1] = g;
        tempVertex.m_normal[2] = b;
        mVertices[i] = tempVertex;

    }
}

void Cube::move(float x, float y, float z)
{
    mPosition.translate(x, y, z);
}
