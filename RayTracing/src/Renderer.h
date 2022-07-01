#pragma once

#include "Walnut/Image.h"
#include <glm/glm.hpp>

#include <memory>

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Walnut::Image> GetFinalImage() { return m_FinalImage; }

private:
	uint32_t PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	glm::vec3 CameraOrigin = glm::vec3(0.0f);
};

