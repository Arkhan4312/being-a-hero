#pragma once
#include "Camera2D.h"
#include "Shader.h"
#include "SpriteBatch.h"
#include "Texture2D.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>



class Renderer {
public:
	Renderer();
	~Renderer();


	bool init(int width, int height, const char* title);
	void shutdown();
	bool windowShouldClose() const;
	void pollEvents();
	void swapBuffers();

	void clear();
	void setClearColor(float r, float g, float b, float a);
	void beginFrame(const Camera2D& camera);
	void endFrame();
	SpriteBatch& getSpriteBatch();
	Texture2D& getWhiteTexture();
	GLFWwindow* getWindow();
private:
	GLFWwindow* window_ = nullptr;
	std::unique_ptr<Shader> default_shader_;
	std::unique_ptr<SpriteBatch> sprite_batch_;
	std::unique_ptr<Texture2D> white_texture_;
};