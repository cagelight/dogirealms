#pragma once

#include "common.hh"
#include "window.hh"
#include "game/game_public.hh"

namespace DOGI {

	struct Renderer {
		
		virtual void initialize(Window::GLLoadFunc) = 0;
		virtual void resize(uint16_t width, uint16_t height) = 0;
		virtual void frame(Intermediate::FramePtr) = 0;
		
	};
	
	using RendererPtr = std::shared_ptr<Renderer>;

}
