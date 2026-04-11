#pragma once

#include <GL/glew.h>
#include <string>

class TextureBuilder
{
public:
	static GLuint CreateTextureFromFile(const std::string& path);
	static GLuint CreateCheckerTexture(
		unsigned char r1,
		unsigned char g1,
		unsigned char b1,
		unsigned char r2,
		unsigned char g2,
		unsigned char b2);
	static GLuint CreateStripeTexture(
		unsigned char r1,
		unsigned char g1,
		unsigned char b1,
		unsigned char r2,
		unsigned char g2,
		unsigned char b2);
	static GLuint CreateRadialTexture(
		unsigned char r1,
		unsigned char g1,
		unsigned char b1,
		unsigned char r2,
		unsigned char g2,
		unsigned char b2);
};
