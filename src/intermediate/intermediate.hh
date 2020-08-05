#pragma once

#include "common.hh"

namespace DOGI::Intermediate {

	// ================================
	// CONCEPTS
	// ================================

	struct Input {
		
		struct {
			Vec3 axrot {0, 0, 0};
		} direct;
		
		struct {
			Vec3 axrot {0, 0, 0};
			Vec3 axmov {0, 0, 0};
		} impulsed;
	};

	struct BasicCube {
		Vec3 color;
		Vec3 position;
		Quat rotation;
	};

	// ================================
	// FRAME
	// ================================

	struct Frame {
		Quat cam_rotation;
		Vec3 cam_position;
		
		std::vector<BasicCube> cubes;
	};
	
	using FramePtr = std::shared_ptr<Frame>;

}
