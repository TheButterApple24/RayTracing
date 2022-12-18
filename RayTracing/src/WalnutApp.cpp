#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Input/Input.h"
#include "Walnut/Input/KeyCodes.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() : m_Camera(45.0f, 0.1f, 100.0f)
	{
		Material& defaultMat = m_Scene.Materials.emplace_back();
		defaultMat.Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
		Material& orange = m_Scene.Materials.emplace_back();
		orange.Albedo = glm::vec3(1.0f, 0.5f, 0.0f);
		Material& blue = m_Scene.Materials.emplace_back();
		blue.Albedo = { (151.0f / 255.0f), 0.0f, 1.0f };

		{
			Sphere orangeSphere;
			orangeSphere.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			orangeSphere.Radius = 1.0f;
			m_Scene.Spheres.push_back(orangeSphere);
		}

		{
			Sphere sphere;
			sphere.Position = { 0.0f, -101.0f, 0.0f };
			sphere.Radius = 100.0f;
			m_Scene.Spheres.push_back(sphere);
		}
	}

	virtual void OnUpdate(float ts) override
	{
		if (m_Camera.OnUpdate(ts))
			m_Renderer.ResetFrameIndex();
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}

		ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);

		if (ImGui::Button("Reset"))
		{
			m_Renderer.ResetFrameIndex();
		}

		ImGui::Begin("Scene");
		if (ImGui::Button("Create Sphere"))
		{
			Sphere sphere;
			m_Scene.Spheres.push_back(sphere);
		}

		for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
		{
			ImGui::PushID(i);

			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)m_Scene.Materials.size() - 1);
			ImGui::Separator();

			ImGui::PopID();
		}

		for (size_t i = 0; i < m_Scene.Materials.size(); i++)
		{
			ImGui::PushID(i);

			Material& mat = m_Scene.Materials[i];

			ImGui::ColorEdit3("Albedo", glm::value_ptr(mat.Albedo));
			ImGui::DragFloat("Roughness", &mat.Roughness, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &mat.Metallic, 0.05f, 0.0f, 1.0f);
			ImGui::Separator();
			ImGui::PopID();

		}

		ImGui::End();

		glm::vec3 lightDir = m_Renderer.GetLightDirection();
		if (ImGui::DragFloat3("Light Direction", &lightDir[0], 0.1f))
		{
			m_Renderer.SetLightDirection(lightDir);
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
				ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;

	// TEMP VARIABLES
	glm::vec3 m_LightDirection = glm::vec3(-1.0f);
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}