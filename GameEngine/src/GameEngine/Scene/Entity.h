#pragma once

#include "Scene.h"

#include <entt.hpp>

namespace GameEngine {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			GE_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...); // std::forward passes the args to entt and it takes care of them
		}

		template<typename T>
		T& GetComponent()
		{
			GE_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			GE_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene;
	};

}