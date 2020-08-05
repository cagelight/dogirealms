#pragma once

#include "m1_public.hh"
#include "glad/glad.hh"

#include <string_view>

#define ATTRIB_POSITION 0
#define ATTRIB_UV 1

namespace DOGI::M1 {

	struct Mesh;
	struct Shader;
	struct ShaderProgram;
	
	using MeshPtr = std::shared_ptr<Mesh>;
	using ShaderPtr = std::shared_ptr<Shader>;
	using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
	
	struct BasicMesh;
	
	// SHADER
	struct MainProgram;
	
	struct RendM1;
}

// ================================================================
// MESH
// ================================================================

struct DOGI::M1::Mesh {
	
	virtual ~Mesh() = default;
	virtual void draw() = 0;
	
};

struct DOGI::M1::BasicMesh : public Mesh {
	
	struct Vertex {
		Vec3GL pos;
		Vec2GL uv;
	};
	
	BasicMesh(std::span<Vertex const> pos);
	~BasicMesh();
	
	virtual void draw();
	
protected:
	
	GLenum m_draw_mode = GL_TRIANGLES;
	GLsizei m_element_count = 0;
	GLuint m_vao = 0, m_vbo = 0;
};

// ================================================================
// SHADER
// ================================================================

struct DOGI::M1::Shader final {
	friend ShaderProgram;
	
	enum struct Type : GLenum {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
	};
	
	Shader(Type type);
	~Shader();
	
	void source(char const * src);
	
	struct CompileResult {
		bool success;
		std::string log;
	};
	
	[[nodiscard]]
	CompileResult compile();
	
private:
	
	GLuint m_handle = 0;
};

struct DOGI::M1::ShaderProgram final {
	
	ShaderProgram();
	~ShaderProgram();
	
	void attach(ShaderPtr);
	
	struct LinkResult {
		bool success;
		std::string log;
	};
	
	[[nodiscard]]
	LinkResult link();
	
	void use();
	
private:
	
	GLuint m_handle = 0;
};

struct DOGI::M1::MainProgram {
	
	MainProgram();
	
	inline void use() { m_program->use(); }
	
	void mvp(Mat4 const &);
	void color(Vec3 const &);
	
private:
	
	ShaderProgramPtr m_program;
};

// ================================================================
// CORE
// ================================================================

struct DOGI::M1::RendM1 : public DOGI::Renderer {
	
	void initialize(Window::GLLoadFunc) override;
	void resize(uint16_t width, uint16_t height) override;
	void frame(Intermediate::FramePtr) override;
	
	void initialize_shaders();
	std::unique_ptr<MainProgram> program_main;
	
	void initialize_meshes();
	MeshPtr unit_cube;
	
	Mat4 projection_matrix;
};
