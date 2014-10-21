#include <VBE/config.hpp>
#include <VBE/graphics/OpenGL.hpp>
#include <VBE/graphics/Texture.hpp>
#include <VBE/system/Log.hpp>

// static
unsigned int Texture::lastSlot = 0;
int Texture::maxSlots = -1;
const Texture* Texture::current[Texture::TypeCount];

//static
unsigned int Texture::getMaxSlots() {
	if(maxSlots == -1) {
		GL_ASSERT(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSlots));
		VBE_DLOG("* Platform info: Max concurrent texture images: " << maxSlots);
	}
	return maxSlots;
}

// static
void Texture::bind(Texture::Type type, const Texture* tex) {
	const Texture* curr = current[type];
	if(curr == tex) return;

	if(tex == nullptr)
		glBindTexture(typeToGL(type), tex->handle);
	else
		glBindTexture(typeToGL(type), 0);

	current[type] = tex;
}

Texture::Texture(Type type): handle(0), slot(0), type(type) {
	lastSlot = lastSlot%(getMaxSlots()-1);
	slot = lastSlot++;
	GL_ASSERT(glGenTextures(1, &handle));
	VBE_ASSERT(handle != 0, "Failed to create texture");
}

Texture::~Texture(){
	GL_ASSERT(glDeleteTextures(1, (GLuint*) &handle));
}

void Texture::setSlot(unsigned int newSlot) {
	VBE_ASSERT(int(newSlot) < maxSlots, "Trying to use impossible texture slot " << newSlot << ". Maximum is " << maxSlots);
	slot = newSlot;
}

unsigned int Texture::getSlot() const {
	return slot;
}

GLuint Texture::getHandle() const {
	return handle;
}

TextureFormat::Format Texture::getFormat() const {
	return format;
}

Texture::Type Texture::getType() const {
	return type;
}

#ifndef VBE_GLES2
void Texture::setComparison(GLenum func, GLenum mode) {
	VBE_ASSERT(TextureFormat::isDepth(format), "Can't set comparison for a non-depth, non_stencil texture");
	Texture::bind(type, this);
	GL_ASSERT(glTexParameteri(typeToGL(type), GL_TEXTURE_COMPARE_FUNC, func));
	GL_ASSERT(glTexParameteri(typeToGL(type), GL_TEXTURE_COMPARE_MODE, mode));
}
#endif

void Texture::setFilter(GLenum min, GLenum mag) {
	Texture::bind(type, this);
	GL_ASSERT(glTexParameteri(typeToGL(type), GL_TEXTURE_MIN_FILTER, min));
	GL_ASSERT(glTexParameteri(typeToGL(type), GL_TEXTURE_MAG_FILTER, mag));
}

void Texture::setWrap(GLenum wrap) {
	Texture::bind(type, this);
	GL_ASSERT(glTexParameteri(typeToGL(type), GL_TEXTURE_WRAP_S, wrap));
	GL_ASSERT(glTexParameteri(typeToGL(type), GL_TEXTURE_WRAP_T, wrap));
}
