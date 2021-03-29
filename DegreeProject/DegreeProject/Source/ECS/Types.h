#pragma once

#include <set>

class System;
struct Component;

// Constants
const size_t MAX_ENTITY_COUNT = 5000;
const size_t MAX_COMPONENT_COUNT = 32;

// Custom types
using EntityID = size_t;
using SystemTypeID = size_t;
using ComponentTypeID = size_t;
using EntitySignature = std::set<ComponentTypeID>;

// Create an id for a component
inline static const ComponentTypeID getRuntimeComponentTypeID()
{
	static ComponentTypeID typeID = 0u;
	return typeID++;
}

// Create an id for a system
inline static const SystemTypeID getRuntimeSystemTypeID()
{
	static SystemTypeID typeID = 0u;
	return typeID++;
}

// Attach typeid to component class and return it
template<typename T>
inline static const ComponentTypeID componentType() noexcept
{
	static_assert((std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value), "INVALID TEMPLATE TYPE");
	static const ComponentTypeID typeID = getRuntimeComponentTypeID();

	return typeID;
}

// Attach typeid to system class and return it
template<typename T>
inline static const SystemTypeID systemType() noexcept
{
	static_assert((std::is_base_of<System, T>::value && !std::is_same<System, T>::value), "INVALID TEMPLATE TYPE");
	static const SystemTypeID typeID = getRuntimeSystemTypeID();

	return typeID;
}