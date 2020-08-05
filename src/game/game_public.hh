#pragma once

#include "common.hh"
#include "intermediate/intermediate.hh"

namespace DOGI::Game {
	
	// ================================
	// CORE
	// ================================
	
	void init();
	void term();
	Intermediate::FramePtr frame(Intermediate::Input const &, double impulse);

}
