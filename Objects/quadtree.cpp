#include "quadtree.h"

QuadTree::QuadTree() : m_sv{ nullptr }, m_so{ nullptr }, m_nv{ nullptr }, m_no{ nullptr }
{
    construct();
}


QuadTree::QuadTree(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4)
    : m_a{ v1 }, m_b{ v2 }, m_c{ v3 }, m_d{ v4 }, m_sv{ nullptr }, m_so{ nullptr }, m_nv{ nullptr }, m_no{ nullptr }
{
    construct();
}


void QuadTree::construct()
{
    mVertices.push_back(Vertex{m_a.first, 0, m_a.second, 0, 0, 1});
    mVertices.push_back(Vertex{m_b.first, 0, m_b.second, 0, 0, 1});
    mVertices.push_back(Vertex{m_c.first, 0, m_c.second, 0, 0, 1});
    mVertices.push_back(Vertex{m_d.first, 0, m_d.second, 0, 0, 1});
       mMatrix.setToIdentity();
}



void QuadTree::init(GLint matrixUniform)
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

    if (m_sv)
        m_sv->init(matrixUniform);
    if (m_so)
        m_so->init(matrixUniform);
    if (m_no)
        m_no->init(matrixUniform);
    if (m_nv)
        m_nv->init(matrixUniform);
}

void QuadTree::draw()
{
    glBindVertexArray(mVAO);
        //glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
        glDrawArrays(GL_LINE_LOOP, 0, mVertices.size());

        if (m_sv)
            m_sv->draw();
        if (m_so)
            m_so->draw();
        if (m_no)
            m_no->draw();
        if (m_nv)
            m_nv->draw();
}



    QuadTree* QuadTree::insert(const Point2D& point, VisualObject* gameObject)
    {
        if (isLeaf()) {
            m_sub_objects.push_back(gameObject);
            qDebug() << "insert" << gameObject->getPosition2D();
            qDebug() << qPrintable(QString::fromStdString(gameObject->m_name));
            return this;
        }
        else {
            Point2D m = Point2D{ (m_a.first + m_c.first) / 2,(m_a.second + m_c.second) / 2 };
            if (point.second < m.second) {
                if (point.first < m.first)
                    m_sv->insert(point, gameObject);
                else
                    m_so->insert(point, gameObject);
            } else {
                if (point.first < m.first)
                    m_nv->insert(point, gameObject);
                else
                    m_no->insert(point, gameObject);
            }
        }
        return nullptr;
    }


bool QuadTree::isLeaf() const
{
    return m_sv == nullptr && m_no == nullptr && m_nv == nullptr && m_so == nullptr;
}

const QuadTree *QuadTree::find(const Point2D& p) const
{
    if(isLeaf())
        return this;
    else {
        Point2D m = Point2D{(m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2};

        if(p.second < m.second)
        {
            if(p.first < m.first)
                m_sv->find(p);
            else
                m_so->find(p);
        }
        else {
            if (p.first < m.first)
                m_nv->find(p);
            else
                m_no->find(p);
        }
    }
}

QuadTree *QuadTree::find(const Point2D& p)
{
    if(isLeaf())
        return this;
    else {
        Point2D m = Point2D{(m_a.first + m_c.first)/2, (m_a.second + m_c.second)/2};

        if(p.second < m.second)
        {
            if(p.first < m.first)
                m_sv->find(p);
            else
                m_so->find(p);
        }
        else {
            if (p.first < m.first)
                m_nv->find(p);
            else
                m_no->find(p);
        }
    }
}


void QuadTree::subDivide(int n)
{
    if(n > 0){
        Point2D v1 = Point2D { (m_a.first + m_b.first)/2,(m_a.second + m_b.second)/2};
        Point2D v2 = Point2D { (m_b.first + m_c.first)/2,(m_b.second + m_c.second)/2};
        Point2D v3 = Point2D { (m_c.first + m_d.first)/2,(m_c.second + m_d.second)/2};
        Point2D v4 = Point2D { (m_d.first + m_a.first)/2,(m_d.second + m_a.second)/2};
        Point2D m = Point2D { (m_a.first + m_c.first)/2,(m_a.second + m_c.second)/2};

        m_sv = new QuadTree(m_a, v1, m, v4);
        m_sv->subDivide(n-1);
        m_so = new QuadTree(v1, m_b, v2, m);
        m_so->subDivide(n-1);
        m_no = new QuadTree(m, v2, m_c, v3);
        m_no->subDivide(n-1);
        m_nv = new QuadTree(v4, m, v3, m_d);
        m_nv->subDivide(n-1);
    }
}
