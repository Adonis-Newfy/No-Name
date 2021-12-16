///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2021FALL
//  File Name:       Entity.h
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
#include "Components.h"

class EntityManager;

typedef std::tuple<
    CTransform,
    CLifeSpan,
    CInput,
    CBoundingBox,
    CAnimation,
    CState,
    CFollowPlayer,
    CPatrol,
    CDamage,
    CHealth,
    CInvincibility,
    CGravity,
    CInventory,
    CWeapons,
    CBuffed,
    CCooldown,
    CLayer
> ComponentTuple;

class Entity
{
    friend class EntityManager;

    bool                m_active    = true;
    std::string         m_tag       = "default";
    size_t              m_id        = 0;
    ComponentTuple      m_components;

    // constructor is private so we can never create
    // entities outside the EntityManager which had friend access
    Entity(const size_t & id, const std::string & tag);

public:
                                      
    void                    destroy();
    size_t                  id()                const;
    bool                    isActive()          const;
    const std::string &     tag()               const;

    template <typename T>
    bool hasComponent() const
    {
        return getComponent<T>().has;
    }

    template <typename T, typename... TArgs>
    T & addComponent(TArgs&&... mArgs)
    {
        auto & component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T & getComponent()
    {
        return std::get<T>(m_components);
    }
                                      
    template<typename T>
    const T& getComponent() const
    {
        return std::get<T>(m_components);
    }

    template<typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }
};

                                      

// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
