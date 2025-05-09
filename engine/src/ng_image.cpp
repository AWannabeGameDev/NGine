#include "ngine/renderer/glm_clip_control.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include <stdexcept>

#include "ngine/renderer/image.hpp"

ng::Image::Image(std::string_view path, ImageShrinkFilter minFilter, ImageEnlargeFilter magFilter, ImageWrap wrap,
				 unsigned int mipmapLevels) :
	_textureRef {std::make_shared<_GPUTexture>()}
{
	stbi_set_flip_vertically_on_load(true);
	int width, height;
	unsigned char* data {stbi_load(path.data(), &width, &height, nullptr, 4)};
	_width = (unsigned int)width;
	_height = (unsigned int)height;

	if(data == nullptr)
	{
		throw std::runtime_error {"Couldn't load image."};
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &_textureRef->textureId);

	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_MAG_FILTER, (GLint)magFilter);
	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_WRAP_S, (GLint)wrap);
	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_WRAP_T, (GLint)wrap);
	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_WRAP_R, (GLint)wrap);

	if(minFilter > ImageShrinkFilter::PIXEL_LINEAR_NO_MIPMAP) // mipmap specified
	{
		glTextureStorage2D(_textureRef->textureId, mipmapLevels, GL_RGBA32F, _width, _height);
		glTextureSubImage2D(_textureRef->textureId, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(_textureRef->textureId);
	}
	else
	{
		glTextureStorage2D(_textureRef->textureId, 1, GL_RGBA32F, _width, _height);
		glTextureSubImage2D(_textureRef->textureId, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	stbi_image_free(data);

	_textureRef->textureHandle = glGetTextureHandleARB(_textureRef->textureId);
	glMakeTextureHandleResidentARB(_textureRef->textureHandle);

	if(_textureRef->textureHandle == 0)
	{
		throw std::runtime_error {"Couldn't obtain texture handle for image."};
	}
}

ng::Image::Image(const glm::vec4& color) :
	_width {1}, _height {1},
	_textureRef {std::make_shared<_GPUTexture>()}
{
	glCreateTextures(GL_TEXTURE_2D, 1, &_textureRef->textureId);

	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(_textureRef->textureId, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTextureStorage2D(_textureRef->textureId, 1, GL_RGBA32F, _width, _height);
	glTextureSubImage2D(_textureRef->textureId, 0, 0, 0, _width, _height, GL_RGBA, GL_FLOAT, glm::value_ptr(color));

	_textureRef->textureHandle = glGetTextureHandleARB(_textureRef->textureId);
	glMakeTextureHandleResidentARB(_textureRef->textureHandle);

	if(_textureRef->textureHandle == 0)
	{
		throw std::runtime_error {"Couldn't obtain texture handle for image."};
	}
}

unsigned int ng::Image::width() const
{
	return _width;
}

unsigned int ng::Image::height() const
{
	return _height;
}

ng::Image::_GPUTexture::~_GPUTexture()
{
	glMakeTextureHandleNonResidentARB(textureHandle);
	glDeleteTextures(1, &textureId);
}