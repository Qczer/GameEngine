#include "gepch.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "SceneHierarchyPanel.h"
#include "GameEngine/Scene/Components.h"

namespace GameEngine {

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
    {
        SetContext(scene);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

		auto view = m_Context->m_Registry.view<entt::entity>();
		for (auto entityID : view)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		}

        if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {

            if (ImGui::MenuItem("Create Empty Entity"))
                m_Context->CreateEntity("Empty Entity");

            ImGui::EndPopup();
        }

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

        ImGui::End();

		ImGui::Begin("Properties");

        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);

		ImGui::End();
    }

	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
            m_SelectionContext = entity;

        bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{

            if (ImGui::MenuItem("Delete  Entity"))
                entityDeleted = true;

			ImGui::EndPopup();
		}

        if (opened)
        {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
        }

        if (entityDeleted)
        {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity)
                m_SelectionContext = {};
        }
	}

	static void DrawResetButton(const std::string& name, float& value, float resetValue, ImVec4 color, ImVec4 hoveredColor, ImVec2 buttonSize, ImFont* boldFont)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
		ImGui::PushFont(boldFont);
		if (ImGui::Button(name.c_str(), buttonSize))
			value = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

        const std::string result = "##" + name;

		ImGui::SameLine();
		ImGui::DragFloat(result.c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
	}

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.f)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        DrawResetButton("X", values.x, resetValue, { 0.8f, 0.1f, 0.15f, 1.0f }, { 0.9f, 0.2f, 0.2f, 1.0f }, buttonSize, boldFont);
        DrawResetButton("Y", values.y, resetValue, { 0.2f, 0.7f, 0.2f, 1.0f }, { 0.3f, 0.8f, 0.3f, 1.0f }, buttonSize, boldFont);
        DrawResetButton("Z", values.z, resetValue, { 0.1f, 0.25f, 0.8f, 1.0f }, { 0.2f, 0.35f, 0.9f, 1.0f }, buttonSize, boldFont);

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template <typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;

        if (entity.HasComponent<T>())
        {
			auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
            float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
                ImGui::OpenPopup("ComponentSettings");

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open)
            {
				uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                entity.RemoveComponent<T>();
        }
    }

	void SceneHierarchyPanel::DrawComponents(Entity& entity)
	{
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!m_SelectionContext.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			else if (!m_SelectionContext.HasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

        ImGui::PopItemWidth();
        
        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
        {
			DrawVec3Control("Translation", component.Translation);

			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);

			DrawVec3Control("Scale", component.Scale, 1.0f);
        });

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
        {
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &verticalFov))
					camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

				float nearClip = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &nearClip))
					camera.SetPerspectiveNearClip(nearClip);

				float farClip = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &farClip))
					camera.SetPerspectiveFarClip(farClip);
			}
			else
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float nearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &nearClip))
					camera.SetOrthographicNearClip(nearClip);

				float farClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &farClip))
					camera.SetOrthographicFarClip(farClip);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color), 0.5f);
        });
	}

}