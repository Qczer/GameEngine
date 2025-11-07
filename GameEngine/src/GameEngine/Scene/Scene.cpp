#include "gepch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "GameEngine/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace GameEngine {

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
		GE_CORE_TRACE("Constructed a transform");
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };

					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto&& [entity, tc, cameraComponent] : view.each())
			{
				if (cameraComponent.Primary)
				{
					mainCamera = &cameraComponent.Camera;
					cameraTransform = tc.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [tc, src] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(tc.GetTransform(), src.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto&& [entity, cameraComponent] : view.each())
		{
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

}