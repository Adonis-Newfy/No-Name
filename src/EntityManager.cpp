///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2021FALL
//  File Name:       EntityManager.cpp
// 
//  Student Name:    Nicholas Anstey
//  Student User:    nmcanstey
//  Student Email:   nmcanstey@mun.ca
//  Student ID:      201824968
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\

#include "EntityManager.h"
                                      
EntityManager::EntityManager()
{

}

void EntityManager::update()
{
    // add all the entities that are pending
    for (auto e : m_entitiesToAdd)
    {
        // add it to the vector of all entities
        m_entities.push_back(e);

        // add it to the entity map in the correct place
        // map[key] will create an element at 'key' if it does not already exist
        //          therefore we are not in danger of adding to a vector that doesn't exist
        m_entityMap[e->tag()].push_back(e);
    }
    
    // clear the temporary vector since we have added everything
    m_entitiesToAdd.clear();

    // clean up dead entities in all vectors
    removeDeadEntities(m_entities);
    for (auto & kv : m_entityMap)
    {
        // kv is a key-value pair contained in the map
        //    key   (kv.first):  the tag string
        //    value (kv.second): the vector storing entities
        removeDeadEntities(kv.second);
    }
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    // use std::remove_if to remove dead entities
    // probably not the fastest solution, but it is safe
    vec.erase(std::remove_if(vec.begin(), vec.end(),
        [](const std::shared_ptr<Entity> & entity) { return !entity->isActive(); }), vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
    // create the entity shared pointer
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
                                      
    // add it to the vector of entities that will be added on next update() call
    m_entitiesToAdd.push_back(entity);

    // return the shared pointer to the entity
    return entity;
}

const EntityVec & EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
    // return the vector in the map where all the entities with the same tag live
    return m_entityMap[tag];
}

// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
