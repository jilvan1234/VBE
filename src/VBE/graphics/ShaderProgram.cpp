#include "ShaderProgram.hpp"
#include "Uniform.hpp"
#include "Shader.hpp"

GLuint ShaderProgram::current(0);

ShaderProgram::ShaderProgram() : programHandle(0) {
}

ShaderProgram::~ShaderProgram() {
	if(programHandle != 0)
		GL_ASSERT(glDeleteProgram(programHandle));
	for(std::map<std::string, Uniform*>::iterator it = uniforms.begin(); it != uniforms.end(); ++it)
		delete it->second;
}

ShaderProgram* ShaderProgram::loadFromString(const std::string& vertSource, const std::string& fragSource) {
	ShaderProgram* p = new ShaderProgram();
	Shader* vertex = Shader::loadShader(vertSource, GL_VERTEX_SHADER);
	Shader* fragment = Shader::loadShader(fragSource, GL_FRAGMENT_SHADER);
	VBE_DLOG("* Creating new shaderProgram");

	p->programHandle = glCreateProgram();

	vertex->attach(p->programHandle);
	fragment->attach(p->programHandle);
	p->link();
	p->retrieveProgramInfo();
	delete vertex;
	delete fragment;
	return p;
}


ShaderProgram* ShaderProgram::loadFromFile(const std::string& vp_filename, const std::string& fp_filename) {
	return loadFromString(readFileIntoString(vp_filename),
						  readFileIntoString(fp_filename));
}

#ifndef VBE_GLES2

ShaderProgram*ShaderProgram::loadFromString(const std::string& vertSource, const std::string& geomSource, const std::string& fragSource) {
	ShaderProgram* p = new ShaderProgram();
	Shader* vertex = Shader::loadShader(vertSource, GL_VERTEX_SHADER);
	Shader* geometry = Shader::loadShader(geomSource, GL_GEOMETRY_SHADER);
	Shader* fragment = Shader::loadShader(fragSource, GL_FRAGMENT_SHADER);
	VBE_DLOG("* Creating new shaderProgram");

	p->programHandle = glCreateProgram();
	VBE_ASSERT(glGetError() == GL_NO_ERROR, "Failed to create program");

	vertex->attach(p->programHandle);
	geometry->attach(p->programHandle);
	fragment->attach(p->programHandle);
	p->link();
	p->retrieveProgramInfo();
	delete vertex;
	delete geometry;
	delete fragment;
	return p;
}

ShaderProgram* ShaderProgram::loadFromString(const std::string& vertSource, const std::string& tescSource, const std::string& teseSource, const std::string& geomSource, const std::string& fragSource) {
	ShaderProgram* p = new ShaderProgram();
	Shader* vertex   = Shader::loadShader(vertSource, GL_VERTEX_SHADER);
	Shader* tessctrl = Shader::loadShader(tescSource, GL_TESS_CONTROL_SHADER);
	Shader* tesseval = Shader::loadShader(teseSource, GL_TESS_EVALUATION_SHADER);
	Shader* geometry = Shader::loadShader(geomSource, GL_GEOMETRY_SHADER);
	Shader* fragment = Shader::loadShader(fragSource, GL_FRAGMENT_SHADER);
	VBE_DLOG("* Creating new shaderProgram");

	p->programHandle = glCreateProgram();
	VBE_ASSERT(glGetError() == GL_NO_ERROR, "Failed to create program");

	vertex->attach(p->programHandle);
	tessctrl->attach(p->programHandle);
	tesseval->attach(p->programHandle);
	geometry->attach(p->programHandle);
	fragment->attach(p->programHandle);
	p->link();
	p->retrieveProgramInfo();

	delete vertex;
	delete tessctrl;
	delete tesseval;
	delete geometry;
	delete fragment;

	return p;
}

ShaderProgram* ShaderProgram::loadFromFile(const std::string& vp_filename, const std::string& gp_filename, const std::string& fp_filename) {
	return loadFromString(readFileIntoString(vp_filename),
						  readFileIntoString(gp_filename),
						  readFileIntoString(fp_filename));
}

ShaderProgram* ShaderProgram::loadFromFile(const std::string& vp_filename, const std::string& tc_filename, const std::string& te_filename, const std::string& gp_filename, const std::string& fp_filename) {
	return loadFromString(readFileIntoString(vp_filename),
						  readFileIntoString(tc_filename),
						  readFileIntoString(te_filename),
						  readFileIntoString(gp_filename),
						  readFileIntoString(fp_filename));
}
#endif

void ShaderProgram::printInfoLog() {
	VBE_ASSERT(programHandle != 0, "Trying to query nullptr program");
	int length = 0;
	GL_ASSERT(glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &length));
	if (length > 1) {
		GLchar* infoLog = new GLchar[length];
		GL_ASSERT(glGetProgramInfoLog(programHandle, length, nullptr, infoLog));
		VBE_LOG(infoLog );
		delete[] infoLog;
	}
}

void ShaderProgram::use() const {
	VBE_ASSERT(programHandle != 0, "Trying to use nullptr program");
	if(current != programHandle) {
		current = programHandle;
		GL_ASSERT(glUseProgram(programHandle));
	}
	for(std::map<std::string, Uniform*>::const_iterator it = uniforms.begin(); it != uniforms.end(); ++it)
		it->second->ready();
}

bool ShaderProgram::hasUniform(const std::string &name) const {
	return uniforms.find(name) != uniforms.end();
}

Uniform* ShaderProgram::uniform(const std::string &name) const {
	VBE_ASSERT(uniforms.find(name) != uniforms.end(), "Trying to retrieve unexisting uniform " << name);
	VBE_ASSERT(programHandle != 0, "Trying to retrieve uniform from nullptr program");
	return uniforms.at(name);
}

void ShaderProgram::link() {
	GL_ASSERT(glLinkProgram(programHandle));
	//CHECK FOR LINK SUCCESS
	GLint success;
	GL_ASSERT(glGetProgramiv(programHandle, GL_LINK_STATUS, &success));
	if (success != GL_TRUE) {
		printInfoLog();
		VBE_ASSERT(false, "Linking program failed" );
	}
	VBE_DLOG( " - Linked program successfully. PROGRAMID: " << programHandle );
}

void ShaderProgram::retrieveProgramInfo() {
	//RETRIEVE ATTRIBUTE DATA
	GLint activeAttributes;
	GL_ASSERT(glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES, &activeAttributes));
	if (activeAttributes > 0) {
		GLint length;
		GL_ASSERT(glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length));
		if (length > 0) {
			GLchar* attribName = new GLchar[length + 1];
			GLint attribSize;
			GLenum attribType;
			GLint attribLocation;
			for (int i = 0; i < activeAttributes; ++i) {
				// Query attribute info.
				GL_ASSERT(glGetActiveAttrib(programHandle, i, length, nullptr, &attribSize, &attribType, attribName));
				attribName[length] = '\0';

				// Query the pre-assigned attribute location.
				attribLocation = glGetAttribLocation(programHandle, attribName);

				// Assign the vertex attribute mapping for the effect.
				attributes[attribName] = attribLocation;
			}
			delete[] attribName;
		}
	}

	//RETRIEVE UNIFORM INFO
	GLint activeUniforms;
	GL_ASSERT(glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS, &activeUniforms));
	if (activeUniforms > 0) {
		GLint length;
		GL_ASSERT(glGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length));
		if (length > 0) {
			GLchar* uniformName = new GLchar[length + 1];
			GLint uniformSize;
			GLenum uniformType;
			GLint uniformLocation;
			for (int i = 0; i < activeUniforms; ++i) {
				// Query uniform info.
				GL_ASSERT(glGetActiveUniform(programHandle, i, length, nullptr, &uniformSize, &uniformType, uniformName));
				uniformName[length] = '\0';  // nullptr terminate
				if (uniformSize > 1 && length > 3) {
					// This is an array uniform. I'm stripping array indexers off it since GL does not
					// seem to be consistent across different drivers/implementations in how it returns
					// array uniforms. On some systems it will return "u_matrixArray", while on others
					// it will return "u_matrixArray[0]".
					char* c = strrchr(uniformName, '[');
					if (c) *c = '\0';
				}

				// Query the pre-assigned uniform location.
				uniformLocation = glGetUniformLocation(programHandle, uniformName);
				VBE_ASSERT(glGetError() == GL_NO_ERROR, "Failed to get uniform location");
				Uniform* uniform = new Uniform(uniformSize, uniformType, uniformLocation);

				uniforms[uniformName] = uniform;
			}
			delete[] uniformName;
		}
	}

	//PRINT ATTRIBUTE INFO
	VBE_DLOG(" - Printing attribute info:" );
	for(std::map<std::string, GLint>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
		VBE_DLOG("  - Name: " << it->first);
		VBE_DLOG( "    Location: " << it->second);
	}

	//PRINT UNIFORM INFO
	VBE_DLOG(" - Printing uniform info:" );
	for(std::map<std::string, Uniform*>::iterator it = uniforms.begin(); it != uniforms.end(); ++it) {
		VBE_DLOG("  - Name: " << it->first);
		it->second->log();
	}
}

std::string ShaderProgram::readFileIntoString(const std::string& filename){
	std::ifstream is;
	is.open(filename, std::ios::in);
	VBE_ASSERT(!is.fail(), "Failed to get the contents from " << filename );
	// get length of file
	is.seekg(0, std::ios::end);
	int length = (int) is.tellg();
	is.seekg(0, std::ios::beg);

	// allocate memory:
	std::string s(length, 0);

	// read data as a block:
	is.read(&s[0], length);
	is.close();

	return s;
}