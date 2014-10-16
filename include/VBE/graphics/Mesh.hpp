#ifndef MESH_HPP
#define MESH_HPP

#include <map>
#include <string>

#include <VBE/config.hpp>
#include <VBE/graphics/OpenGL.hpp>
#include <VBE/graphics/Vertex.hpp>
#include <VBE/utils/NonCopyable.hpp>

class ShaderBinding;
class ShaderProgram;
class Mesh : public NonCopyable {
	public:
		enum PrimitiveType {
			TRIANGLES = GL_TRIANGLES,
			TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
			TRIANGLE_FAN = GL_TRIANGLE_FAN,
			LINES = GL_LINES,
			LINE_STRIP = GL_LINE_STRIP,
			LINE_LOOP = GL_LINE_LOOP,
			POINTS = GL_POINTS,
#ifndef VBE_GLES2
			PATCHES = GL_PATCHES
#endif
		};

		enum BufferType {
			STATIC = GL_STATIC_DRAW,
			DYNAMIC = GL_DYNAMIC_DRAW,
			STREAM = GL_STREAM_DRAW
		};

		static Mesh* loadFromFile(const std::string filepath, Mesh::BufferType bufferType = STATIC);
		static Mesh* loadEmpty(Vertex::Format format, Mesh::BufferType bufferType = STATIC, bool indexed = false);

		~Mesh();

		void draw(const ShaderProgram* program, unsigned int firstVertex, unsigned int vCount);

		const Vertex::Format& getVertexFormat() const;
		unsigned int getVertexCount() const;
		unsigned int getVertexSize() const;
		BufferType getType() const;
		GLuint getVertexBuffer() const;
		GLuint getIndexBuffer() const;
		PrimitiveType getPrimitiveType() const;
		bool isIndexed() const;

		void setPrimitiveType(Mesh::PrimitiveType type);
		void setVertexData(const void* vertexData, unsigned int newVertexCount);
		void setVertexIndices(const unsigned int* indexData, unsigned int newIndexCount);

	private:
		Mesh(Vertex::Format format, BufferType bufferType = STATIC, bool indexed = false);

		std::map<GLuint, const ShaderBinding*> bindingsCache;
		const Vertex::Format vertexFormat;
		unsigned int vertexCount;
		unsigned int indexCount;
		GLuint vertexBuffer;
		GLuint indexBuffer;
		PrimitiveType primitiveType;
		BufferType bufferType;
		bool indexed;
};

#endif // MESH_HPP