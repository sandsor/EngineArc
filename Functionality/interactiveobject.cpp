#include "interactiveobject.h"

InteractiveObject::InteractiveObject() //: mx{0.0f}, my{0.0f}, mz{0.0f}
{
    mVertices.push_back(Vertex{-0.1f,-0.1f,-0.1f,1.0f,0.0f,0.0f});
    mVertices.push_back(Vertex{-0.1,-0.1,0.1,1.0,0.0,0.0});
    mVertices.push_back(Vertex{-0.1,0.1,0.1,1.0,0.0,0.0});

    mVertices.push_back(Vertex{0.1,0.1,-0.1,0,1,0});
    mVertices.push_back(Vertex{-0.1,-0.1,-0.1,0,1,0});
    mVertices.push_back(Vertex{-0.1,0.1,-0.1,0,1,0});

    mVertices.push_back(Vertex{0.1,-0.1,0.1,0,1,0});
    mVertices.push_back(Vertex{-0.1,-0.1,-0.1,0,1,0});
    mVertices.push_back(Vertex{0.1,-0.1,-0.1,0,1,0});

    mVertices.push_back(Vertex{0.1,0.1,-0.1,0,1,0});
    mVertices.push_back(Vertex{0.1,-0.1,-0.1,0,1,0});
    mVertices.push_back(Vertex{-0.1,-0.1,-0.1,0,1,0});

    mVertices.push_back(Vertex{-0.1,-0.1,-0.1,1,0,0});
    mVertices.push_back(Vertex{-0.1,0.1,0.1,1,0,0});
    mVertices.push_back(Vertex{-0.1,0.1,-0.1,1,0,0});

    mVertices.push_back(Vertex{0.1,-0.1,0.1,0,0,1});
    mVertices.push_back(Vertex{-0.1,-0.1,0.1,0,0,1});
    mVertices.push_back(Vertex{-0.1,-0.1,-0.1,0,0,1});

    mVertices.push_back(Vertex{-0.1,0.1,0.1,0,1,1});
    mVertices.push_back(Vertex{-0.1,-0.1,0.1,0,1,1});
    mVertices.push_back(Vertex{0.1,-0.1,0.1,0,1,1});

    mVertices.push_back(Vertex{0.1,0.1,0.1,1,0,1});
    mVertices.push_back(Vertex{0.1,-0.1,-0.1,1,0,1});
    mVertices.push_back(Vertex{0.1,0.1,-0.1,1,0,1});

    mVertices.push_back(Vertex{0.1,-0.1,-0.1,1,0,1});
    mVertices.push_back(Vertex{0.1,0.1,0.1,1,0,1});
    mVertices.push_back(Vertex{0.1,-0.1,0.1,1,0,1});

    mVertices.push_back(Vertex{0.1,0.1,0.1,1,0,1});
    mVertices.push_back(Vertex{0.1,0.1,-0.1,1,0,1});
    mVertices.push_back(Vertex{-0.1,0.1,-0.1,1,0,1});

    mVertices.push_back(Vertex{0.1,0.1,0.1,0,1,1});
    mVertices.push_back(Vertex{-0.1,0.1,-0.1,0,1,1});
    mVertices.push_back(Vertex{-0.1,0.1,0.1,0,1,1});

    mVertices.push_back(Vertex{0.1,0.1,0.1,0,1,1});
    mVertices.push_back(Vertex{-0.1,0.1,0.1,0,1,1});
    mVertices.push_back(Vertex{0.1,-0.1,0.1,0,1,1});

    mMatrix.setToIdentity();
//    mx = 1.5f;
//    mz = 1.5f;
//    my = curve->function(mx,my) + 1.2f;

//    mPosition(0,3) = mx;
//    mPosition(1,3) = my;
//    mPosition(2,3) = mz;

//    mMatrix = mPosition;
}

InteractiveObject::~InteractiveObject()
{

}

void InteractiveObject::init(GLint matrixUniform)
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

void InteractiveObject::draw()
{
    glBindVertexArray( mVAO );
   // glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void InteractiveObject::move(float x, float y, float z)
{
//    mx += x;
//    my += y;
//    mz += z;

////    mMatrix.translate(x,y,z);

//        mPosition(0,3) = mx;
//        mPosition(1,3) = curve->function(mx,mz) + 0.5f;
//        mPosition(2,3) = mz;



//    mMatrix = mPosition;

}

void InteractiveObject::MoveY(float y)
{
    mMatrix.translate(0,y,0);
    mMatrix = mPosition;
}

void InteractiveObject::collision(VisualObject* gameObject)
{
    float objX = gameObject->mMatrix.getPosition().getX();
    float objZ = gameObject->mMatrix.getPosition().getZ();

    float playerX = mMatrix.getPosition().getX();
    float playerZ = mMatrix.getPosition().getZ();

        if (objX - 0.2 < playerX && objX + 0.3 > playerX && objZ - 0.2 < playerZ && objZ + 0.35 > playerZ)
        {
                if(gameObject->m_name == "enemy")
                    qDebug() << "Hit";
                if(gameObject->m_name == "pickup")
                    gameObject->bDraw = false;
        }
}


