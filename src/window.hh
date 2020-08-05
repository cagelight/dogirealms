#pragma once

#include "common.hh"
#include "game/game_public.hh"

namespace DOGI {
	
	struct Window {
		
		virtual ~Window() = default;
		
		virtual void open(uint16_t width, uint16_t height) = 0;
		virtual void close() = 0;
		
		using GLLoadFunc = void * (*) ( char const * );
		[[nodiscard]]
		virtual GLLoadFunc initialize_gl() = 0;
		
		struct EventData {
			bool close_requested = false;
			
			bool size_changed = false;
			uint16_t width, height;
			
			Intermediate::Input input;
		};		
		
		[[nodiscard]]
		virtual EventData input_frame() = 0;
		
		virtual void finish_frame() = 0;
	};
	
	using WindowPtr = std::shared_ptr<Window>;
	
}
