#pragma once

#include "game_public.hh"

namespace DOGI::Game {

	extern thread_local std::mt19937_64 rng;
	
	struct Camera {
		
		Quat rotation = Quat::axis({0, 1, 1}, 0);
		Vec3 position {0, 0, 0};
		
	};
	
	struct StaticCube {
		Vec3 color;
		Vec3 position;
		Quat rotation;
		
		static StaticCube random();
	};
	
	struct LerpCube {
		
		StaticCube pos_1;
		StaticCube pos_2;
		double velocity;
		
		static LerpCube random();
		Intermediate::BasicCube lerp(double impulse);
		
	private:
		double m_lerp = 0;
	};
	
	struct GameData {
		Camera cam;
		std::vector<LerpCube> lerp_cubes;
	};
	
	extern std::unique_ptr<GameData> data;

}
