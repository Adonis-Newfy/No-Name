///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2021FALL
//  File Name:       EntityManager.h
// 
//  Student Name:    Nicholas Anstey
//  Student User:    nmcanstey
//  Student Email:   nmcanstey@mun.ca
//  Student ID:      201824968
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
                                      
#pragma once

#include "Common.h"
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
    EntityVec                           m_entities;             // all entities
    EntityVec                           m_entitiesToAdd;        // entities to add next update
    std::map<std::string, EntityVec>    m_entityMap;            // map from entity tag to vectors
    size_t                              m_totalEntities = 0;    // total entities created
                                      
    // helper function to avoid repeated code
    void removeDeadEntities(EntityVec& vec);

public:

    EntityManager();

    void update();

    std::shared_ptr<Entity> addEntity(const std::string & tag);

    const EntityVec & getEntities();
    const EntityVec & getEntities(const std::string & tag);
};

// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
