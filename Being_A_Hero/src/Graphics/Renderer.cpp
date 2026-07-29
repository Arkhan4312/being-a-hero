
#include <glad/glad.h>
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

static const char* vertexSrc = R"(
	#version 330 core
	layout(location = 0) in vec2 aPos;
	layout(location = 1) in vec2 aTexCoord;
	layout(location = 2) in vec4 aColor;

	uniform mat4 uProjection;

	out vec2 vTexCoord;
	out vec4 vColor;

	void main() {
		gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
		vTexCoord = aTexCoord;
		vColor = aColor;
	}
)";

static const char* fragmentSrc = R"(
	#version 330 core
	in vec2 vTexCoord;
	in vec4 vColor;

	uniform sampler2D uTexture;

	out vec4 FragColor;

	void main() {
		FragColor = texture(uTexture,vTexCoord) * vColor;
	}
)";

Renderer::Renderer() = default;
Renderer::~Renderer() {
	shutdown();
}

bool Renderer::init(int width, int height, const char* title)
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window_) {
		std::cerr << "Failed to create window\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window_);
	gladLoadGL();
	glfwSwapInterval(1);

	default_shader_ = std::make_unique<Shader>(vertexSrc, fragmentSrc);
	sprite_batch_ = std::make_unique<SpriteBatch>();
	white_texture_ = Texture2D::createSolidColor(255, 255, 255, 255);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}

void Renderer::shutdown()
{
	white_texture_.reset();
	sprite_batch_.reset();
	default_shader_.reset();
	if (window_) {
		glfwDestroyWindow(window_);
		glfwTerminate();
		window_ = nullptr;
	}
}

bool Renderer::windowShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void Renderer::pollEvents()
{
	glfwPollEvents();
}

void Renderer::swapBuffers()
{
	glfwSwapBuffers(window_);
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::beginFrame(const Camera2D& camera)
{
	clear();
	default_shader_->bind();
	default_shader_->setUniformMat4("uProjection", camera.getProjectionMatrix());
	default_shader_->setUniformInt("uTexture", 0);
	sprite_batch_->begin(camera.getProjectionMatrix(), *white_texture_);
}

void Renderer::endFrame()
{
	sprite_batch_->end();
	swapBuffers();
}

SpriteBatch& Renderer::getSpriteBatch()
{
	return *sprite_batch_;
}

Texture2D& Renderer::getWhiteTexture()
{
	return *white_texture_;
}

GLFWwindow* Renderer::getWindow()
{
	return window_;
}
