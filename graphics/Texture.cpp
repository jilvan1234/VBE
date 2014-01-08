#include "Texture.hpp"

unsigned int Texture::lastSlot = 0;

Texture::Texture(): handle(0), slot(0), size(0, 0){
	lastSlot = lastSlot%(GL_MAX_TEXTURE_UNITS-GL_TEXTURE0-2);
	slot = lastSlot++;
	glGenTextures(1, &handle);
}

Texture::~Texture(){
	glDeleteTextures(1, (GLuint*) &handle);
}

Texture* Texture::createEmpty(unsigned int sizeX, unsigned int sizeY, Texture::InternalFormat format, int slot) {
	Texture* t = new Texture();
	t->loadEmpty(sizeX, sizeY, format, slot);
	return t;
}

Texture* Texture::createFromFile(const std::string& filePath, Texture::SourceFormat sourceFormat, Texture::SourceType sourceType, Texture::InternalFormat internalFormat, bool mipmap, int slot) {
	Texture* t = new Texture();
	t->loadFromFile(filePath, sourceFormat, sourceType, internalFormat, mipmap, slot);
	return t;
}

Texture* Texture::createFromRaw(const void* pixels, unsigned int sizeX, unsigned int sizeY, Texture::SourceFormat sourceFormat, Texture::SourceType sourceType, Texture::InternalFormat internalFormat, bool mipmap, int slot) {
	Texture* t = new Texture();
	t->loadFromRaw(pixels, sizeX, sizeY, sourceFormat, sourceType, internalFormat, mipmap, slot);
	return t;
}

void Texture::loadEmpty(unsigned int sizeX, unsigned int sizeY, Texture::InternalFormat format, int slot) {
	SourceFormat sFormat = RGBA;
	SourceType sType = UNSIGNED_BYTE;
	switch (format) {
		case R8_SNORM: case R8I: case R8UI: case R16: case R16F: case R16I: case R16UI: case R16_SNORM: case R32F: case R32I: case R32UI: case R8: sFormat = RED; break;
		case RG8_SNORM: case RG8I: case RG8UI: case RG16: case RG16F: case RG16I: case RG16UI: case RG16_SNORM: case RG32F: case RG32I: case RG32UI: case RG8: sFormat = RG; break;
		case RGB4: case RGB5: case RGB8_SNORM: case RGB8I: case RGB8UI: case RGB10: case RGB12: case RGB16_SNORM: case RGB16F: case RGB16I: case RGB16UI: case RGB32F: case RGB32I: case RGB32UI: case RGB8: sFormat = RGB; break;
		case RGBA2: case RGBA4: case RGBA8_SNORM: case RGBA12: case RGBA16: case RGBA16F: case RGBA32F: case RGBA8I: case RGBA8UI: case RGBA16I: case RGBA16UI: case RGBA32I: case RGBA32UI: case RGBA8: sFormat = RGBA; break;
		case DEPTH_COMPONENT16: case DEPTH_COMPONENT24: case DEPTH_COMPONENT32F: case DEPTH24_STENCIL8: case DEPTH32F_STENCIL8: case DEPTH_COMPONENT32: sFormat = DEPTH_COMPONENT; sType = UNSIGNED_INT; break;
	}
	loadFromRaw(nullptr, sizeX, sizeY, sFormat, sType, format, false, slot);
}

void Texture::loadFromFile(const std::string& filePath, Texture::SourceFormat sourceFormat, Texture::SourceType sourceType, Texture::InternalFormat internalFormat, bool mipmap, int slot) {
	//load image
	VBE_DLOG("* Loading new texture from path " << filePath);
	sf::Image image;
	if (!image.loadFromFile(filePath)) {
		VBE_LOG("#ERROR " << filePath << " didn't load" );
	}
	loadFromRaw(image.getPixelsPtr(), image.getSize().x, image.getSize().y, sourceFormat, sourceType, internalFormat, mipmap, slot);
}

void Texture::loadFromRaw(const void* pixels, unsigned int sizeX, unsigned int sizeY, Texture::SourceFormat sourceFormat, Texture::SourceType sourceType, Texture::InternalFormat internalFormat, bool mipmap, int slot) {
	if(slot != -1)
		setSlot(slot);
	format = internalFormat;
	size = vec2i(sizeX, sizeY);
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, sizeX, sizeY, 0, sourceFormat, sourceType, (GLvoid*) pixels);
	if(mipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
		setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	}
	else
		setFilter(GL_LINEAR, GL_LINEAR);

	setWrap(GL_REPEAT);
}

void Texture::resize(unsigned int sizeX, unsigned int sizeY) {
	loadEmpty(sizeX, sizeY, format, slot);
}

void Texture::bind() const {
	VBE_ASSERT(handle !=0, "Trying to bind nullptr texture into slot " << slot);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::setFilter(GLenum min, GLenum mag) {
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture::setWrap(GLenum wrap) const {
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap);
}

void Texture::setComparison(GLenum func, GLenum mode) {
	VBE_ASSERT(format == DEPTH_COMPONENT16 || format == DEPTH_COMPONENT24 || format == DEPTH_COMPONENT32 || format == DEPTH_COMPONENT32F || format == DEPTH24_STENCIL8 || format == DEPTH32F_STENCIL8, "Can't set comparison for a non-depth, non_stencil texture");
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, func);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, mode);
}

void Texture::setSlot(unsigned int newSlot) {
	VBE_ASSERT(newSlot < GL_MAX_TEXTURE_UNITS, "Trying to use impossible texture slot " << newSlot << ". Maximum is " << GL_MAX_TEXTURE_UNITS);
	slot = newSlot;
}

unsigned int Texture::getSlot() const {
	return slot;
}

GLuint Texture::getHandle() const {
	return handle;
}

int Texture::getWidth() const {
	return size.x;
}
int Texture::getHeight() const {
	return size.y;
}
