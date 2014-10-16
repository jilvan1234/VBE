#ifndef Texture_HPP
#define Texture_HPP

#include <VBE/graphics/OpenGL.hpp>
#include <VBE/graphics/TextureFormat.hpp>
#include <VBE/utils/NonCopyable.hpp>

class Texture : public NonCopyable {
	public:
		virtual ~Texture();

		void setSlot(unsigned int newSlot);
		unsigned int getSlot() const;
		GLuint getHandle() const;
		TextureFormat::Format getFormat() const;
	protected:
		Texture();
		GLuint handle;
		unsigned int slot;
		TextureFormat::Format format;
		GLenum texType;
	private:
		static unsigned int lastSlot;
		static int maxSlots;
};

#endif // Texture_HPP
