#pragma once
#include "entt/entt.hpp"
#include "renderwindow.h"
class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, RenderWindow* renderWindow);
	Entity(const Entity& other) = default;

	template<typename T, typename ... Args>
	T& AddComponent(Args&&...args)
	{
		if(!HasComponent<T>())
		return mRenderWindow->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
	}
	template<typename T>
	T& GetComponent()
	{
		if(HasComponent<T>())
		return mRenderWindow->mRegistry.get<T>(mEntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return mRenderWindow->mRegistry.any_of<T>(mEntityHandle);
	}
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

