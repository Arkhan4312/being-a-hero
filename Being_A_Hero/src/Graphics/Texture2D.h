#pragma once 
#include <string>
#include <memory>
#include <glad/glad.h>
class Texture2D {
public:
	// from BMP
	Texture2D(const std::string& filepath);
	// generate Texture
	Texture2D(int width, int height, GLenum internalFormat, GLenum format, const void* data);

	~Texture2D();

	static std::unique_ptr<Texture2D> createSolidColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	void bind(unsigned int slot = 0) const;
	int geWidth() const;
	int getHeight() const;

private:
	unsigned int renderer_Id_ = 0;
	int width_ = 0;
	int height_ = 0;
};