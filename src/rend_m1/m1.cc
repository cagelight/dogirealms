#include "m1.hh"

using namespace DOGI;

RendererPtr DOGI::M1::create_renderer() {
	return std::make_shared<M1::RendM1>();
}

void M1::RendM1::initialize(Window::GLLoadFunc proc) {
	gladLoadGLLoader(proc);
	glClearColor(0, 0, 0, 1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	initialize_shaders();
	initialize_meshes();
}

void M1::RendM1::resize(uint16_t width, uint16_t height) {
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
	
	float wf = width, hf = height;
	//projection_matrix = mat4_t::ortho(-hf/20, hf/20, -wf/20, wf/20, -10, 10);
	projection_matrix = Mat4::perspective(b::radians<float>(80), wf, hf, 0.125f, 100.0f);
}

void M1::RendM1::frame(Intermediate::FramePtr f) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program_main->use();
	
	Mat4 view { f->cam_rotation };
	view = view * Mat4::translate(f->cam_position);
	
	for (auto const & cube : f->cubes) {
		program_main->color(cube.color);
		
		Mat4 model_matrix = Mat4::translate(cube.position) * Mat4 { cube.rotation };
		program_main->mvp(projection_matrix * view * model_matrix);
		
		unit_cube->draw();
	}
}
