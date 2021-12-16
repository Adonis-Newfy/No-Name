///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2021FALL
//  File Name:       Vec2.h
// 
//  Student Name:    Nicholas Anstey
//  Student User:    nmcanstey
//  Student Email:   nmcanstey@mun.ca
//  Student ID:      201824968
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\

#pragma once

class Vec2
{
public:

    float x = 0;
    float y = 0;
                                      
    Vec2();
    Vec2(float xin, float yin);
 
    bool operator == (const Vec2 & rhs) const;
    bool operator != (const Vec2 & rhs) const;
    bool operator <= (const Vec2 & rhs) const;
    bool operator >= (const Vec2 & rhs) const;
                                      
    Vec2 operator + (const Vec2 & rhs) const;
    Vec2 operator - (const Vec2 & rhs) const;
    Vec2 operator / (const float& val) const;
    Vec2 operator * (const float & val) const;
    float operator * (const Vec2& rhs) const;
                                      
    void operator += (const Vec2 & rhs);
    void operator -= (const Vec2 & rhs);
    void operator *= (const float & val);
    void operator /= (const float & val);

    Vec2 abs() const;
    float dist(const Vec2 & rhs) const;
};
                                      
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
