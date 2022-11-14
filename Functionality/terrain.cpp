#include "terrain.h"
#include "logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Terrain::Terrain()
{
    m_localBuffer = stbi_load("../3Dprog22/Assets/heightmap.bmp", &m_width, &m_height, (int*)NULL, 1);
    setPlane();
}

void Terrain::init(GLint matrixUniform)
{
    mMatrixUniform =matrixUniform;
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
                    3,                  // size / number of elements of data type
                    GL_FLOAT,           // data type
                    GL_FALSE,           // normalize data
                    sizeof(Vertex),  // stride
                    reinterpret_cast<GLvoid*>(0)  );          // array buffer offset
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(2);

        glGenBuffers ( 1, &mEAB );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
}

void Terrain::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void Terrain::setPlane()
{
    mVertices.clear();
    mIndices.clear();

    for ( int z = 0; z < m_height; z++)
        for(int x = 0; x < m_width; x++)
        {
            float y = m_localBuffer[(z * m_width) + x];
            float u = x/m_width-1;
            float v = y/m_height-1;

            mVertices.push_back(Vertex{(float)x,y,(float)z,y/255,y/255,y/255,u,v});
        }

    for(int j = 0; j < m_height-1; j++)
        for(int i = 0; i < m_width-1; i++)
        {
            int it = j * m_width + i;

            Vertex v1 = mVertices[it];
            Vertex v2 = mVertices[it+1];
            Vertex v3 = mVertices[it+1+m_width];
            Vertex v4 = mVertices[it+m_width];

            gsl::Vector3D a;
            a.x = v1.m_xyz[0] - v2.m_xyz[0];
            a.y = v1.m_xyz[1] - v2.m_xyz[1];
            a.z = v1.m_xyz[2] - v2.m_xyz[2];
            gsl::Vector3D b;
            b.x = v3.m_xyz[0] - v2.m_xyz[0];
            b.y = v3.m_xyz[1] - v2.m_xyz[1];
            b.z = v3.m_xyz[2] - v2.m_xyz[2];
            gsl::Vector3D n = n.cross(a,b);
            n.normalize();
            mVertices[it].m_normal[0] = n.getX();
            mVertices[it].m_normal[1] = n.getY();
            mVertices[it].m_normal[2] = n.getZ();
            mVertices[it+1].m_normal[0] = n.getX();
            mVertices[it+1].m_normal[1] = n.getY();
            mVertices[it+1].m_normal[2] = n.getZ();
            mVertices[it+1+m_width].m_normal[0] = n.getX();
            mVertices[it+1+m_width].m_normal[1] = n.getY();
            mVertices[it+1+m_width].m_normal[2] = n.getZ();

            a.x = v1.m_xyz[0] - v4.m_xyz[0];
            a.y = v1.m_xyz[1] - v4.m_xyz[1];
            a.z = v1.m_xyz[2] - v4.m_xyz[2];
            b.x = v3.m_xyz[0] - v4.m_xyz[0];
            b.y = v3.m_xyz[1] - v4.m_xyz[1];
            b.z = v3.m_xyz[2] - v4.m_xyz[2];
            gsl::Vector3D n2 = n2.cross(b,a);
            n2.normalize();
            mVertices[it].m_normal[0] = n2.getX();
            mVertices[it].m_normal[1] = n2.getY();
            mVertices[it].m_normal[2] = n2.getZ();
            mVertices[it+1+m_width].m_normal[0] = n2.getX();
            mVertices[it+1+m_width].m_normal[1] = n2.getY();
            mVertices[it+1+m_width].m_normal[2] = n2.getZ();
            mVertices[it+m_width].m_normal[0] = n2.getX();
            mVertices[it+m_width].m_normal[1] = n2.getY();
            mVertices[it+m_width].m_normal[2] = n2.getZ();

            mIndices.push_back(it);
            mIndices.push_back(it + 1);
            mIndices.push_back(it + 1 + m_width );

            mIndices.push_back(it + 1 + m_width );
            mIndices.push_back(it + m_width );
            mIndices.push_back(it);

        }
    mMatrix.setToIdentity();
}

gsl::Vector3D Terrain::BarycCoordinates(const gsl::Vector2D& p1, const gsl::Vector2D& p2, const gsl::Vector2D& p3, const gsl::Vector2D& player)
{
    gsl::Vector2D p12 = p2-p1;
    gsl::Vector2D p13 = p3-p1;
    gsl::Vector3D n = n.cross(gsl::Vector3D{p13.x, 0, p13.y}, gsl::Vector3D{p12.x, 0, p12.y});

    float areal_123 = n.length(); // dobbelt areal
    gsl::Vector3D baryc; // til retur. Husk
    // u
    gsl::Vector2D p = p2 - player;
    gsl::Vector2D q = p3 - player;
    n = n.cross(gsl::Vector3D{q.x, 0, q.y}, gsl::Vector3D{p.x, 0, p.y});

    baryc.x = n.y/areal_123;
    // v
    p = p3 - player;
    q = p1 - player;

    n = n.cross(gsl::Vector3D{q.x, 0, q.y}, gsl::Vector3D{p.x, 0, p.y});

    baryc.y = n.y/areal_123;

    // w
    p = p1 - player;
    q = p2 - player;
    n = n.cross(gsl::Vector3D{q.x, 0, q.y}, gsl::Vector3D{p.x, 0, p.y});

    baryc.z = n.y/areal_123;

    return baryc;
}


float Terrain::getPositionOnTerrain(const float x, const float z)
{
    float yCoord = 0;

    int newX = /*floor*/(x);
    int newZ = /*floor*/(z);

    qDebug() << "x " << newX << "z " << newZ;
    int it = (newZ * m_width) + newX;

    int a = it;
    int b = it + 1;
    int c = it + 1 + m_width;
    int d = it + m_width;

    gsl::Vector2D p1 = gsl::Vector2D(mVertices[a].m_xyz[0], mVertices[a].m_xyz[2]);
    gsl::Vector2D p2 = gsl::Vector2D(mVertices[b].m_xyz[0], mVertices[b].m_xyz[2]);
    gsl::Vector2D p3 = gsl::Vector2D(mVertices[c].m_xyz[0], mVertices[c].m_xyz[2]);
    gsl::Vector2D p4 = gsl::Vector2D(mVertices[d].m_xyz[0], mVertices[d].m_xyz[2]);
    gsl::Vector2D p = gsl::Vector2D(x, z);

   gsl::Vector3D bary = BarycCoordinates(p1,p2,p3,p);

    if(bary.getX() >= 0 && bary.getX() <= 1 && bary.getY() >= 0 && bary.getY() <= 1 && bary.getZ() >= 0 && bary.getZ() <= 1)
    {
        yCoord += mVertices[a].m_xyz[1] * bary.getX();
        yCoord += mVertices[b].m_xyz[1] * bary.getY();
        yCoord += mVertices[c].m_xyz[1] * bary.getZ();
    } else {
        gsl::Vector3D bary = BarycCoordinates(p1,p3,p4,p);
        yCoord += mVertices[a].m_xyz[1] * bary.getX();
        yCoord += mVertices[c].m_xyz[1] * bary.getY();
        yCoord += mVertices[d].m_xyz[1] * bary.getZ();
    }
    return yCoord;
}
