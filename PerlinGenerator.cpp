#include "PerlinGenerator.h"

PerlinGenerator::PerlinGenerator()
{
    Generate(); // grid
    mMatrix.setToIdentity(); 
}

void PerlinGenerator::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER,      //what buffer type
        mVertices.size() * sizeof(Vertex),   //how big buffer do we need
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
        reinterpret_cast<GLvoid*>(0));          // array buffer offset
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void PerlinGenerator::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr); // since im drawing the lines manually, i nead to draw indecies and not verticies
}

void PerlinGenerator::Generate() // creates a simple grid able to be manipulated
{

    //gennerating vertex points in a grid system Height * Widht,  values are set in header file.sass
    for (float y = 0; y < m_height; y++) {
        for (float x = 0; x < m_width; x++) {
            mVertices.push_back(Vertex{ x - m_width / 2,  0,y - m_height / 2     , y / 255, y / 255, y / 255           , 0, 0 });
        }
    }

    //connecting lines too create grid. going through all verticies
    for (float y = 0; y < m_height - 1; y++) {
        for (float x = 0; x < m_width - 1; x++) {
            mIndices.push_back(x + y * m_width);
            mIndices.push_back((x + 1) + y * m_width);
            mIndices.push_back(x + (y + 1) * m_width);

            mIndices.push_back((x + 1) + y * m_width);
            mIndices.push_back((x + 1) + (y + 1) * m_width);
            mIndices.push_back(x + (y + 1) * m_width);
        }
    }
    //RandomMove();
    //PerlinMove();
}

void PerlinGenerator::RandomMove()
{
    if (RandomPoints) {
        for (int i = 0; i < mVertices.size(); i++) {
            Vertex newPosition = mVertices.at(i);
            int range = (maxheight - minHeight) + 1;
            newPosition.m_xyz[1] = minHeight + int(range * rand() / (RAND_MAX + 1.0));
            float life = (newPosition.m_xyz[1] - minHeight) / (maxheight - minHeight);
            gsl::Vector3D temp = calculateColor(life, gsl::Vector3D(0, 1, 0), gsl::Vector3D(0, 0, 1));
            newPosition.m_normal[0] = temp.getX();
            newPosition.m_normal[1] = temp.getY();
            newPosition.m_normal[2] = temp.getZ();
          mVertices.at(i) = newPosition;

        }
    }
    //newPosition.m_xyz[1] = rand() % maxheight + minHeight;
}

void PerlinGenerator::PerlinMove()
{
    if (PerlinActive) {

        int i = 0;
        Highestpoint = 0;
        lowestpoint = 0;

        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {

                Vertex newPosition = mVertices.at(i);
                //newPosition.m_xyz[1] = ofNoise(x, y);
                newPosition.m_xyz[1] = ofNoise( ofMap(x, 0, m_width, 0, perlinrange, false),
                    ofMap(y, 0, m_height, 0, perlinrange, false))*maxheight;

                if (newPosition.m_xyz[1]< lowestpoint)
                    lowestpoint = newPosition.m_xyz[1];

                if (newPosition.m_xyz[1] > Highestpoint)
                    Highestpoint = newPosition.m_xyz[1];

                float life = (newPosition.m_xyz[1] - lowestpoint) / (Highestpoint - lowestpoint);
                gsl::Vector3D temp = calculateColor(life, gsl::Vector3D(0, 1, 0), gsl::Vector3D(0, 0, 1));
                newPosition.m_normal[0] = temp.getX();
                newPosition.m_normal[1] = temp.getY();
                newPosition.m_normal[2] = temp.getZ();

                mVertices.at(i) = newPosition;
                i++;
            }
        }
    }
}

gsl::Vector3D PerlinGenerator::calculateColor(float mLife, gsl::Vector3D mColorBegin, gsl::Vector3D mColorEnd) // got help from teammate
{
    gsl::Vector3D color;
    float RedDifference = mColorBegin.getX() - mColorEnd.getX();
    float GreenDifference = mColorBegin.getY() - mColorEnd.getY();
    float BlueDifference = mColorBegin.getZ() - mColorEnd.getZ();
    color.setX(mColorBegin.getX() - (1 - mLife) * RedDifference);
    color.setY(mColorBegin.getY() - (1 - mLife) * GreenDifference);
    color.setZ(mColorBegin.getZ() - (1 - mLife) * BlueDifference);


    return color;
}




