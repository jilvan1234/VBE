#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include "tools.hpp"
#include "Screen.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

class Environment {
	public:
		class Config {
			public:
				enum GLContextFlag {
					GLDebugContext = SDL_GL_CONTEXT_DEBUG_FLAG,
					GLForwardCompatibleContext = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
					GLRobustAccessContext = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
					GLRobustIsolationContext = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
				};

				enum GLProfileMask {
					GLCoreProfile = SDL_GL_CONTEXT_PROFILE_CORE,
					GLCompatibilityProfile = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
					GLESProfile = SDL_GL_CONTEXT_PROFILE_ES
				};

				Config() :
					windowTitle("VBE_APP"),
					windowHeight(0),
					windowWidth(0),
					windowFlags(Screen::WINDOW_OPENGL|Screen::WINDOW_SHOWN),
					GLMajor(4),
					GLMinor(2),
					GLRedSize(3),
					GLGreenSize(3),
					GLBlueSize(2),
					GLAlphaSize(0),
					GLBufferSize(0),
					GLDepthSize(16),
					GLStencilSize(0),
					GLAccumRedSize(0),
					GLAccumGreenSize(0),
					GLAccumBlueSize(0),
					GLAccumAlphaSize(0),
					GLMultisampleBuffers(0),
					GLMultisampleSamples(0),
					GLContextFlags(0),
					GLProfile(GLCoreProfile),
					GLDoubleBuffer(true),
					GLAcceleratedGraphics(true),
					GLStereo(false),
					GLShareContext(false),
					GLRequestSRGB(false)
				{}

				std::string  windowTitle;
				unsigned int windowHeight;
				unsigned int windowWidth;
				unsigned int windowFlags;
				unsigned int GLMajor;
				unsigned int GLMinor;
				unsigned int GLRedSize;
				unsigned int GLGreenSize;
				unsigned int GLBlueSize;
				unsigned int GLAlphaSize;
				unsigned int GLBufferSize;
				unsigned int GLDepthSize;
				unsigned int GLStencilSize;
				unsigned int GLAccumRedSize;
				unsigned int GLAccumGreenSize;
				unsigned int GLAccumBlueSize;
				unsigned int GLAccumAlphaSize;
				unsigned int GLMultisampleBuffers;
				unsigned int GLMultisampleSamples;
				unsigned int GLContextFlags;
				GLProfileMask GLProfile;
				bool GLDoubleBuffer;
				bool GLAcceleratedGraphics;
				bool GLStereo;
				bool GLShareContext;
				bool GLRequestSRGB;
		};

		static Config& setup();
		static void startUp();
		static void shutDown();

		static void update();

		static Screen* getScreen();
		static Keyboard* getKeyboard() { return keyboard; }
		static Mouse* getMouse() { return mouse; }
	private:
		Environment();
		~Environment();

		static bool running;
		static Screen* screen;
		static Keyboard* keyboard;
		static Mouse* mouse;
		static Config initialConfig;
};

#endif // Environment_HPP
