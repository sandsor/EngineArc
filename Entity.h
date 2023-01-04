#pragma once
#include "entt/entt.hpp"
#include "renderwindow.h"
class Entity
{
public:
	//Summary
	//This class creates an entity that has a handle and components described to it

	//constructors
	//parameters: handle is the entitys id and is just a numbre representing the entity
	//parameters: Renderwindow is a refranse to the scene we are rendering in
	Entity() = default;
	Entity(entt::entity handle, RenderWindow* renderWindow);
	Entity(const Entity& other) = default;
	
	//This function adds a component to the entity if it does not already have the component
	template<typename T, typename ... Args>
	T& AddComponent(Args&&...args)
	{
		if(!HasComponent<T>())
		return mRenderWindow->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
	}
	//this function Checks if the current entity has the component and returns it
	template<typename T>
	T& GetComponent()
	{
		if(HasComponent<T>())
		return mRenderWindow->mRegistry.get<T>(mEntityHandle);
	}
	//this funciton checks if the entity has any components and if so returns true
	template<typename T>
	bool HasComponent()
	{
		return mRenderWindow->mRegistry.any_of<T>(mEntityHandle);
	}
	//This function removes the component from the entity if it has that componnet
	template<typename T>
	void RemoveComponent()
	{
		if(HasComponent<T>())
		mRenderWindow->mRegistry.remove<T>(mEntityHandle);
	}
	//operator bool() const { return mEntityHandle != 0; }
private:
	entt::entity mEntityHandle{0};
	RenderWindow* mRenderWindow=nullptr;

};

