///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2021FALL
//  File Name:       Physics.cpp
// 
//  Student Name:    Nicholas Anstey
//  Student User:    nmcanstey
//  Student Email:   nmcanstey@mun.ca
//  Student ID:      201824968
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\

#include "Physics.h"
#include "Components.h"
                                      
Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    

    Vec2 delta = Vec2(abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x), abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y));
    float oX = (a->getComponent<CBoundingBox>().size.x / 2) + (b->getComponent<CBoundingBox>().size.x / 2) - delta.x;
    float oY = (a->getComponent<CBoundingBox>().size.y / 2) + (b->getComponent<CBoundingBox>().size.y / 2) - delta.y;

    Vec2 overlap = Vec2(oX, oY);

    return overlap;
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
   

    Vec2 delta = Vec2(abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x), abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y));
    float oX = (a->getComponent<CBoundingBox>().size.x / 2) + (b->getComponent<CBoundingBox>().size.x / 2) - delta.x;
    float oY = (a->getComponent<CBoundingBox>().size.y / 2) + (b->getComponent<CBoundingBox>().size.y / 2) - delta.y;

    Vec2 overlap = Vec2(oX, oY);

    return overlap;
}

bool Physics::IsInside(const Vec2& pos, std::shared_ptr<Entity> e)
{
    // STUDENT TODO:
    // Implement this function
    // :shrug:

    return false;
}

//To know how works: Raycasting and Line Intersections Lecture
Intersect Physics::LineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
{
   
    Vec2 r = (b - a);
    Vec2 s = (d - c);
    float rxs = (r.x * s.y) - (r.y * s.x);
    Vec2 cma = c - a;
    float t = ((cma.x * s.y) - (cma.y * s.x)) / rxs;
    float u = ((cma.x * r.y) - (cma.y * r.x)) / rxs;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        return { true, Vec2(a.x + (t * r.x), a.y + (t * r.y)) };
    }
    
    return { false, Vec2(0,0) };
    
}

bool Physics::EntityIntersect(const Vec2& a, const Vec2& b, std::shared_ptr<Entity> e)
{
    
                                      
    Vec2 P1 = Vec2(e->getComponent<CTransform>().pos.x - e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CTransform>().pos.y - e->getComponent<CBoundingBox>().halfSize.y);
    Vec2 P2 = Vec2(e->getComponent<CTransform>().pos.x + e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CTransform>().pos.y - e->getComponent<CBoundingBox>().halfSize.y);
    Vec2 P3 = Vec2(e->getComponent<CTransform>().pos.x + e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CTransform>().pos.y + e->getComponent<CBoundingBox>().halfSize.y);
    Vec2 P4 = Vec2(e->getComponent<CTransform>().pos.x - e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CTransform>().pos.y + e->getComponent<CBoundingBox>().halfSize.y);

    if (LineIntersect(a, b, P1, P2).result == true)
    {
        return true;
    }

    else if (LineIntersect(a, b, P2, P3).result == true)
    {
        return true;
    }

    else if (LineIntersect(a, b, P3, P4).result == true)
    {
        return true;
    }

    else if (LineIntersect(a, b, P4, P1).result == true)
    {
        return true;
    }

    return false;

    
}
                                      
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
