#include "Texture2D.h"
#include <glad/glad.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>

#pragma pack(push,1)
struct BMPHeader {
	uint16_t signature;
	uint32_t fileSize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t dataOffset;
	uint32_t dibSize;
	int32_t  width;
	int32_t  height;
	uint16_t planes;
	uint16_t bpp;
	uint32_t compression;
	uint32_t imageSize;
	int32_t  xPixelsPerMeter;
	int32_t  yPixelsPerMeter;
	uint32_t colorsUsed;
	uint32_t colorsImportant;
};
#pragma pack(pop)


Texture2D::Texture2D(const std::string& filepath)
	: renderer_Id_(0), width_(0), height_(0) {
	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open texture file: " << filepath << std::endl;
		return;
	}

	BMPHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	if (header.signature != 0x4D42) {
		std::cerr << "Not a valid BMP file: " << filepath << std::endl;
		return;
	}

	if (header.compression != 0 || (header.bpp != 24 && header.bpp != 32)) {
		std::cerr << "Unsupported BMP format: only uncompressed 24/32 bpp supported. \n";
		return;
	}

	width_ = header.width;
	height_ = abs(header.height);
	bool flipVertically = header.height > 0;
	
	int bytesPerPixel = header.bpp / 8;
	int rowSize = (width_ * bytesPerPixel + 3) & ~3;
	int dataSize = rowSize * height_;

	std::vector<unsigned char> pixels(dataSize);
	file.seekg(header.dataOffset, std::ios::beg);
	file.read(reinterpret_cast<char*>(pixels.data()), dataSize);

	if (flipVertically) {
		std::vector<unsigned char> flipped(dataSize);
		for (int y = 0;y < height_;++y) {
			int srcRow = (height_ - 1 - y) * rowSize;
			int dstRow = y * rowSize;
			memcpy(flipped.data() + dstRow, pixels.data() + srcRow, rowSize);
		}
		pixels = std::move(flipped);
	}

	GLenum internalFormat = (bytesPerPixel == 4) ? GL_RGBA8 : GL_RGB8;
	GLenum format = (bytesPerPixel == 4) ? GL_BGRA : GL_BGR;

	glGenTextures(1, &renderer_Id_);
	glBindTexture(GL_TEXTURE_2D, renderer_Id_);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width_, height_, 0, format, GL_UNSIGNED_BYTE, pixels.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	std::cout << "Loaded texture: " << filepath << " (" << width_ << "x" << height_ << ")\n";
}

Texture2D::Texture2D(int width, int height, GLenum internalFormat, GLenum format, const void* data)
	: width_(width), height_(height) {
	glGenTextures(1, &renderer_Id_);
	glBindTexture(GL_TEXTURE_2D, renderer_Id_);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture2D::~Texture2D()
{
	if (renderer_Id_) {
		glDeleteTextures(1, &renderer_Id_);
	}
}


std::unique_ptr<Texture2D> Texture2D::createSolidColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	unsigned char data[4] = { r,g,b,a };
	return std::make_unique<Texture2D>(1, 1, GL_RGBA8, GL_RGBA, data);
}

void Texture2D::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, renderer_Id_);
}

int Texture2D::geWidth() const
{
	return width_;
}

int Texture2D::getHeight() const
{
	return height_;
}
