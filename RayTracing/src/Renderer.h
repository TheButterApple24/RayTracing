#pragma once

#include "Walnut/Image.h"

#include <memory>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();
	void SetLightDirection(glm::vec3 lightDir) { m_LightDirection = lightDir; }
	glm::vec3 GetLightDirection() { return m_LightDirection; }

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
private:
	glm::vec4 PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	// TEMP VARIABLES
	glm::vec3 m_LightDirection = glm::vec3(-1.0f);
};