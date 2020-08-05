#include "game.hh"

using namespace DOGI;

thread_local std::mt19937_64 Game::rng { std::random_device {}() };

std::unique_ptr<Game::GameData> Game::data;

Game::StaticCube Game::StaticCube::random() {
	StaticCube cube {};
	
	std::uniform_real_distribution<double> color_dist {0, 1};
	cube.color = {
		color_dist(rng),
		color_dist(rng),
		color_dist(rng),
	};
	
	std::uniform_real_distribution<double> pos_dist {-20, 20};
	cube.position = {
		pos_dist(rng),
		pos_dist(rng),
		pos_dist(rng),
	};
	
	cube.rotation = Quat::random(rng);
	
	return cube;
}

Game::LerpCube Game::LerpCube::random() {
	LerpCube cube;
	cube.pos_1 = StaticCube::random();
	cube.pos_2 = StaticCube::random();
	
	std::uniform_real_distribution<double> vel_dist {0.02, 0.8};
	cube.velocity = vel_dist(rng);
	
	std::uniform_real_distribution<double> l_dist {0, b::pi_m<double>(2)};
	cube.m_lerp = l_dist(rng);
	
	return cube;
}

Intermediate::BasicCube Game::LerpCube::lerp(double impulse) {
	Intermediate::BasicCube cube;
	m_lerp += velocity * impulse;
	double lerp = std::sin(m_lerp) / 2 + 1;
	cube.color = b::lerp(pos_1.color, pos_2.color, lerp);
	cube.position = b::lerp(pos_1.position, pos_2.position, lerp);
	cube.rotation = Quat::slerp(pos_1.rotation, pos_2.rotation, lerp);
	return cube;
}

void Game::init() {
	data = std::make_unique<GameData>();
	data->cam.position = { 3, 2, 4 };
	
	for (size_t i = 0; i < 1000; i++)
		data->lerp_cubes.push_back(LerpCube::random());
}

void Game::term() {
	data.reset();
}

static constexpr double DIRECT_ROTATION_MULTIPLIER = 0.001;
static constexpr double MOVEMENT_MULTIPLIER = 20;

Intermediate::FramePtr Game::frame(Intermediate::Input const & input, double impulse) {
	Intermediate::FramePtr f = std::make_shared<Intermediate::Frame>();
	
	// ========================
	// LOGIC -- TEMPORARY
	// ========================
	
	auto xrot = Quat::axis( {1, 0, 0},  input.direct.axrot[0] * DIRECT_ROTATION_MULTIPLIER + input.impulsed.axrot[0] );
	auto yrot = Quat::axis( {0, 1, 0},  input.direct.axrot[1] * DIRECT_ROTATION_MULTIPLIER + input.impulsed.axrot[1] );
	auto zrot = Quat::axis( {0, 0, 1}, -input.direct.axrot[2] * DIRECT_ROTATION_MULTIPLIER - input.impulsed.axrot[2] );
	auto trot = xrot * yrot * zrot;
	trot.normalize();
	
	data->cam.rotation = data->cam.rotation * trot;
	data->cam.rotation.normalize();
	
	Vec3 move_adj { input.impulsed.axmov[0], input.impulsed.axmov[1], input.impulsed.axmov[2] };
	move_adj = data->cam.rotation.conjugate() * -move_adj;
	data->cam.position += move_adj * MOVEMENT_MULTIPLIER;
	
	// ========================
	// INTERMEDIATE
	// ========================
	
	f->cubes.reserve(data->lerp_cubes.size());
	for (auto & scube : data->lerp_cubes)
		f->cubes.emplace_back( scube.lerp(impulse) );
	
	f->cam_position = data->cam.position;
	f->cam_rotation = data->cam.rotation;
	
	return f;
}
