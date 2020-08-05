#include <SDL2/SDL.h>

#include "window_sdl.hh"

using namespace DOGI;

struct SDL2Window final : public Window {
	
	SDL2Window() {
		SDL_Init(SDL_INIT_VIDEO);
	}
	
	~SDL2Window() {
		close();
		SDL_Quit();
	}
	
	void open(uint16_t width, uint16_t height) override {
		window = SDL_CreateWindow("DOGI", 0, 0, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		this->width = width;
		this->height = height;
		
		keystate = SDL_GetKeyboardState(NULL);
	}
	
	void close() override {
		if (gl_ctx) {
			SDL_GL_DeleteContext(gl_ctx);
			gl_ctx = nullptr;
		}
		if (window) {
			SDL_DestroyWindow(window);
			window = nullptr;
		}
	}
	
	GLLoadFunc initialize_gl() override {
		if (!gl_ctx) {
			SDL_GL_LoadLibrary(nullptr);
			gl_ctx = SDL_GL_CreateContext(window);
		}
		return SDL_GL_GetProcAddress;
	}
	
	EventData input_frame() override;
	
	void finish_frame() override {
		SDL_GL_SwapWindow(window);
	}
	
private:
	
	SDL_Window * window = nullptr;
	SDL_GLContext gl_ctx = nullptr;
	Uint8 const * keystate;
	uint16_t width, height;
	
	bool m_in_focus = false;
	bool m_prevent_grab = false;
	
	inline bool should_grab() {
		return m_in_focus && !m_prevent_grab;
	}
};

Window::EventData SDL2Window::input_frame() {
	EventData data {};
	SDL_Event e {};
	while(SDL_PollEvent(&e))  {
		switch (e.type) {
		default: break;
			
		case SDL_QUIT:
			data.close_requested = true;
			break;
			
		case SDL_WINDOWEVENT:
			switch (e.window.event) {
			default:
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				data.size_changed = true;
				this->width = data.width = e.window.data1;
				this->height = data.height = e.window.data2;
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				m_in_focus = true;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				m_in_focus = false;
				break;
			}
			break;
			
		case SDL_MOUSEMOTION:
			if (!should_grab()) break; // rotation input only accepted when grabbed
			data.input.direct.axrot[0] += e.motion.yrel;
			data.input.direct.axrot[1] += e.motion.xrel;
			break;
		}
	}
	
	SDL_SetRelativeMouseMode(should_grab() ? SDL_TRUE : SDL_FALSE);
	
	constexpr auto KEY_XP = SDL_SCANCODE_D;
	constexpr auto KEY_XN = SDL_SCANCODE_A;
	constexpr auto KEY_YP = SDL_SCANCODE_SPACE;
	constexpr auto KEY_YN = SDL_SCANCODE_C;
	constexpr auto KEY_ZP = SDL_SCANCODE_W;
	constexpr auto KEY_ZN = SDL_SCANCODE_S;
	
	if (keystate[KEY_XP]) data.input.impulsed.axmov[0] += 1;
	if (keystate[KEY_XN]) data.input.impulsed.axmov[0] -= 1;
	if (keystate[KEY_YP]) data.input.impulsed.axmov[1] += 1;
	if (keystate[KEY_YN]) data.input.impulsed.axmov[1] -= 1;
	if (keystate[KEY_ZP]) data.input.impulsed.axmov[2] += 1;
	if (keystate[KEY_ZN]) data.input.impulsed.axmov[2] -= 1;
	
	constexpr auto KEY_RoP = SDL_SCANCODE_E;
	constexpr auto KEY_RoN = SDL_SCANCODE_Q;
	constexpr auto KEY_PiP = SDL_SCANCODE_DOWN;
	constexpr auto KEY_PiN = SDL_SCANCODE_UP;
	constexpr auto KEY_YwP = SDL_SCANCODE_RIGHT;
	constexpr auto KEY_YwN = SDL_SCANCODE_LEFT;
	
	if (keystate[KEY_RoP]) data.input.impulsed.axrot[2] += 1;
	if (keystate[KEY_RoN]) data.input.impulsed.axrot[2] -= 1;
	if (keystate[KEY_PiP]) data.input.impulsed.axrot[0] += 1;
	if (keystate[KEY_PiN]) data.input.impulsed.axrot[0] -= 1;
	if (keystate[KEY_YwP]) data.input.impulsed.axrot[1] += 1;
	if (keystate[KEY_YwN]) data.input.impulsed.axrot[1] -= 1;
	
	return data;
}

WindowPtr DOGI::SDL2::create_window() {
	return std::make_shared<SDL2Window>();
}
