#include "m1.hh"

using namespace DOGI;

// ================================================================
// SHADER
// ================================================================

M1::Shader::Shader(Type type) {
	m_handle = glCreateShader(static_cast<GLenum>(type));
}

M1::Shader::~Shader() {
	if (m_handle) glDeleteShader(m_handle);
}

void M1::Shader::source(char const * src) {
	glShaderSource(m_handle, 1, &src, nullptr);
}

M1::Shader::CompileResult M1::Shader::compile() {
	CompileResult res {};
	
	GLint status, log_length;
	glCompileShader(m_handle);
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
	glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &log_length);
	res.log.resize(log_length);
	glGetShaderInfoLog(m_handle, log_length, nullptr, res.log.data());
	res.success = status == GL_TRUE;
	
	return res;
}

// ================================================================
// SHADER PROGRAM
// ================================================================

M1::ShaderProgram::ShaderProgram() {
	m_handle = glCreateProgram();
}

M1::ShaderProgram::~ShaderProgram() {
	if (m_handle) glDeleteProgram(m_handle);
}

void M1::ShaderProgram::attach(ShaderPtr shader) {
	glAttachShader(m_handle, shader->m_handle);
}

M1::ShaderProgram::LinkResult M1::ShaderProgram::link() {
	LinkResult res {};
	
	GLint status, log_length;
	glLinkProgram(m_handle);
	glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &log_length);
	res.log.resize(log_length);
	glGetProgramInfoLog(m_handle, log_length, nullptr, res.log.data());
	res.success = status == GL_TRUE;
	
	return res;
}

void M1::ShaderProgram::use() {
	glUseProgram(m_handle);
}

// ================================================================
// M1
// ================================================================

#define UNIFORM_MVP 0
#define UNIFORM_COLOR 1

#define STRFYI(v) #v
#define STRFY(v) STRFYI(v)

static constexpr char vert_src [] = R"(
#version 450 core

// ATTRIB
layout (location = )" STRFY(ATTRIB_POSITION) R"() in vec3 pos;
layout (location = )" STRFY(ATTRIB_UV) R"() in vec2 uv;

// UNIFORM
layout (location = )" STRFY(UNIFORM_MVP) R"() uniform mat4 mvp;

// TO FRAGMENT SHADER
out vec2 frag_uv;

void main() {
	frag_uv = uv;
	gl_Position = mvp * vec4(pos, 1);
})";

static constexpr char frag_src [] = R"(
#version 450 core

// UNIFORM
layout (location = )" STRFY(UNIFORM_COLOR) R"() uniform vec4 vcolor;

// FROM VERTEX SHADER
in vec2 frag_uv;

// TO FRAMEBUFFER
layout (location = 0) out vec4 color;

void main() {
	
	float val1 = min((frag_uv.x - 0.05) * 20, (frag_uv.y - 0.05) * 20);
	float val2 = min(((1 - frag_uv.x) - 0.05) * 20, ((1 - frag_uv.y) - 0.05) * 20);
	float val = clamp(min(val1, val2), 0.25f, 1);
	color = vec4(val * vcolor[0], val * vcolor[1], val * vcolor[2], 1);
})";

M1::MainProgram::MainProgram() {
	ShaderPtr shader_main_vert, shader_main_frag;
	
	shader_main_vert = std::make_shared<Shader>(Shader::Type::VERTEX);
	shader_main_frag = std::make_shared<Shader>(Shader::Type::FRAGMENT);
	m_program = std::make_shared<ShaderProgram>();
	
	shader_main_vert->source(vert_src);
	shader_main_frag->source(frag_src);
	
	auto vert_comp = shader_main_vert->compile();
	if (!vert_comp.success) {
		LOGE << "MAIN VERTEX SHADER COMPILE FAILURE: " << vert_comp.log;
		std::terminate();
	}
	
	auto frag_comp = shader_main_frag->compile();
	if (!frag_comp.success) {
		LOGE << "MAIN FRAGMENT SHADER COMPILE FAILURE: "  << frag_comp.log;
		std::terminate();
	}
	
	m_program->attach(shader_main_vert);
	m_program->attach(shader_main_frag);
	
	auto prog_link = m_program->link();
	if (!prog_link.success) {
		LOGE << "MAIN SHADER PROGRAM LINK FAILURE: "  << prog_link.log;
		std::terminate();
	}
}

void M1::MainProgram::mvp(Mat4 const & value) {
	Mat4GL mat { value };
	glUniformMatrix4fv(UNIFORM_MVP, 1, GL_FALSE, &mat[0][0]);
}

void M1::MainProgram::color(Vec3 const & value) {
	glUniform4f(UNIFORM_COLOR, value[0], value[1], value[2], 1);
}

void M1::RendM1::initialize_shaders() {
	program_main = std::make_unique<MainProgram>();
}
