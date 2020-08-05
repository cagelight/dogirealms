#include "game/game_public.hh"
#include "sdl/window_sdl.hh"
#include "rend_m1/m1_public.hh"

#include <csignal>

std::atomic_bool DOGI::run_sem { true };

static inline void generic_signal_handler(int) {
	if (!DOGI::run_sem) std::terminate();
	DOGI::run_sem.store(false);
}

static constexpr uint16_t DEFAULT_WIDTH = 1600;
static constexpr uint16_t DEFAULT_HEIGHT = 900;
static constexpr double DEFAULT_GAME_FPS = 250; // game logic and input, NOT renderer

int main() {
	
	signal(SIGINT, generic_signal_handler);
	signal(SIGTERM, generic_signal_handler);
	
	LOGI << "Hi :)";
	
	DOGI::Game::init();
	
	DOGI::WindowPtr win = DOGI::SDL2::create_window();
	win->open(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	
	DOGI::Intermediate::FramePtr game_frame;
	std::atomic_bool rend_do_resize { false };
	uint16_t rend_new_width, rend_new_height;
	
	std::thread rthread { [&](){
			
		DOGI::Intermediate::FramePtr last_frame;
		
		DOGI::RendererPtr rend = DOGI::M1::create_renderer();
		rend->initialize(win->initialize_gl());
		rend->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		
		while (DOGI::run_sem) {
			auto frame_temp = game_frame;
			if (frame_temp != last_frame) {
				last_frame = frame_temp;
				if (rend_do_resize) {
					rend->resize(rend_new_width, rend_new_height);
					rend_do_resize = false;
				}
				rend->frame(frame_temp);
				win->finish_frame();
			}
		}
	} };
	
	DOGI::timekeeper_t g_tk { 1.0 / DEFAULT_GAME_FPS };
	double impulse = 0;
	
	while (DOGI::run_sem) {
		impulse = g_tk.mark_and_sleep().seconds();
		
		auto edat = win->input_frame();
		if (edat.close_requested) {
			DOGI::run_sem.store(false);
		}
		if (edat.size_changed) {
			rend_new_width = edat.width;
			rend_new_height = edat.height;
			rend_do_resize = true;
		}
		
		edat.input.impulsed.axrot *= impulse;
		edat.input.impulsed.axmov *= impulse;
		game_frame = DOGI::Game::frame(edat.input, impulse);
	}
	
	rthread.join();
	win->close();
	
	DOGI::Game::term();
	return 0;
}
