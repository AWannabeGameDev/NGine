#ifndef NG_IMAGE_HPP
#define NG_IMAGE_HPP

#include <glad/glad.h>
#include <glm/vec4.hpp>

#include <string_view>
#include <memory>

namespace ng
{

enum class ImageShrinkFilter
{
	PIXEL_NEAREST_NO_MIPMAP = GL_NEAREST,
	PIXEL_LINEAR_NO_MIPMAP = GL_LINEAR,
	PIXEL_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	PIXEL_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	PIXEL_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	PIXEL_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

enum class ImageEnlargeFilter
{
	PIXEL_NEAREST = GL_NEAREST,
	PIXEL_LINEAR = GL_LINEAR
};

enum class ImageWrap
{
	REPEAT = GL_REPEAT,
	CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
	CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	MIRRORED_REPEAT = GL_MIRRORED_REPEAT
};

class Image
{
	friend class Renderer;

private :
	class _GPUTexture
	{
	public :
		GLuint textureId;

		~_GPUTexture();
	};

	std::shared_ptr<_GPUTexture> _textureRef;
	unsigned int _width;
	unsigned int _height;

public :
	// `mipmapLevels` is silently ignored if `minFilter` does not specify mipmap-based filtering.
	Image(std::string_view path, ImageShrinkFilter minFilter, ImageEnlargeFilter magFilter, ImageWrap wrap, unsigned int mipmapLevels);
	Image(const glm::vec4& color);

	unsigned int width() const;
	unsigned int height() const;
};

}

#endif