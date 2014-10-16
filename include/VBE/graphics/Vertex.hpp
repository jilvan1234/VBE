#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

#include <VBE/config.hpp>
#include <VBE/graphics/OpenGL.hpp>

namespace Vertex {
	class Attribute {
		public:
			static Attribute& get(int ID);
			static Attribute& get(const std::string &name);

			int ID();
			bool hasName(const std::string &name) const;

			bool operator == (const Attribute& a) const;
			bool operator != (const Attribute& a) const;

			std::set<std::string>::const_iterator begin() { return attrNames.begin();}
			std::set<std::string>::const_iterator end()   { return attrNames.end();  }

			Attribute& addName(const std::string &name);

			enum DefaultId { //Build-in default attributes
				Position = 0,
				Color,
				Normal,
				TexCoord
			};

		private:
			Attribute(int ID);
			int id;

			static void init(); //add default attributes
			static bool isAttrsInit; //added default attributes?
			static std::map<std::string, Attribute*> names; //the names of each attribute and the attribute it points to
			static std::vector<Attribute*> attributes; //all the attributes
			std::set<std::string> attrNames; //al the names this attribute can have
	};

	class Element {
		public:
			enum Type {
				Byte          = GL_BYTE,
				UnsignedByte  = GL_UNSIGNED_BYTE,
				Short         = GL_SHORT,
				UnsignedShort = GL_UNSIGNED_SHORT,
				Int           = GL_INT,
				UnsignedInt   = GL_UNSIGNED_INT,
				Float         = GL_FLOAT,
				Fixed         = GL_FIXED,
#ifndef VBE_GLES2
                Double        = GL_DOUBLE
#endif
			};
			inline bool isIntegerType(Type t) {
				switch(t) {
					case GL_FLOAT:
					case GL_FIXED:
#ifndef VBE_GLES2
					case GL_DOUBLE:
#endif
                        return false;
					default:
                        return true;
				}
			}
			enum Conversion {
				ConvertDefault,
				ConvertToFloat,
				ConvertToFloatNormalized,
				ConvertToInt
			};

			Element(Attribute &attr, Type type, unsigned int size, Conversion conv = ConvertDefault);
			Element(     int attrID, Type type, unsigned int size, Conversion conv = ConvertDefault);
			Element(const Element& element);

			Element& operator=(const Element& e);
			bool operator == (const Element& e) const;
			bool operator != (const Element& e) const;

			Attribute &attr;
			Type type;
			unsigned int size;
			Conversion conv;

		private:
			void calcDefaultConversion();
	};

	class Format {
		public:
			Format(const std::vector<Element> &elements);
			~Format();

			const Element& element(unsigned int index) const;
			unsigned int offset(unsigned int index) const;
			unsigned int elementCount() const;
			unsigned int vertexSize() const;
			bool operator == (const Format& f) const;
			bool operator != (const Format& f) const;

		private:
			std::vector<Element> elements;
			std::vector<unsigned int> offsets;
			unsigned int vertSize;
	};

} // namespace Vertex

#endif // VERTEX_HPP