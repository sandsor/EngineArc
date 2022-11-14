#ifndef QuadTree_H
#define QuadTree_H

#include <iostream>
#include <vector>
#include <list>
#include "visualobject.h"


typedef std::pair<float, float> Point2D;// Erstatter Vector2D klasse

// Template klasse, laget for ID og T
// ID er ikke implementert ennå
// Bruker Point2D - se typedef ovenfor - i stedet for Vector2D
class QuadTree : public VisualObject
{
private:
    // Punkter som definerer hjørnene til objektet
    // i samme rekkefølge som pekerne til subtrær
    Point2D m_a;
    Point2D m_b;
    Point2D m_c;
    Point2D m_d;
    // Pekere til subtrær er nødvendig i en dynamisk
    // QuadTree implementering. Vi kunne ha lagt det
    // i array også
    QuadTree* m_sv;
    QuadTree* m_so;
    QuadTree* m_no;
    QuadTree* m_nv;

    // Hvert quad lagrer sine egne objekter
    // (T kan være objekter, pekere eller referanser)
    // Legger opp til mulig søk på id
//    std::list<std::pair>m_objects;

    bool isLeaf() const;

    // Hjelpevariabel for å lagre alle T-ene
    // for senere iterering
    std::vector<VisualObject*> m_all_objects;// skulle vært static

public:
    QuadTree();
    QuadTree(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4);
    void init(GLint matrixUniform) override;
    void draw()override;
    void subDivide(int n);
    void construct();
    QuadTree* insert(const Point2D& point, VisualObject* object);
    QuadTree* find(const Point2D& p);
    const QuadTree* find(const Point2D& p) const;

    std::vector<VisualObject*> m_sub_objects;// skulle vært static
};







#endif // QuadTree_H
