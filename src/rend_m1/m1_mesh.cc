#include "m1.hh"

using namespace DOGI;

M1::BasicMesh::BasicMesh(std::span<Vertex const> verts) {
	
	m_element_count = verts.size();
	
	static constexpr GLsizei offsetof_pos = 0;
	static constexpr GLsizei sizeof_pos = sizeof(Vertex::pos);
	static constexpr GLsizei offsetof_uv = offsetof_pos + sizeof_pos;
	static constexpr GLsizei sizeof_uv = sizeof(Vertex::uv);
	static constexpr GLsizei sizeof_all = offsetof_uv + sizeof_uv;
	static_assert(sizeof_all == sizeof(Vertex));
	
	glCreateBuffers(1, &m_vbo);
	glNamedBufferData(m_vbo, m_element_count * sizeof_all, verts.data(), GL_STATIC_DRAW);
	
	glCreateVertexArrays(1, &m_vao);
	glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof_all);
	glEnableVertexArrayAttrib(m_vao, ATTRIB_POSITION);
	glEnableVertexArrayAttrib(m_vao, ATTRIB_UV);
	glVertexArrayAttribBinding(m_vao, ATTRIB_POSITION, 0);
	glVertexArrayAttribBinding(m_vao, ATTRIB_UV, 0);
	glVertexArrayAttribFormat(m_vao, ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, offsetof_pos);
	glVertexArrayAttribFormat(m_vao, ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, offsetof_uv);
}

M1::BasicMesh::~BasicMesh() {
	if (m_vbo) glDeleteBuffers(1, &m_vbo);
	if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void M1::BasicMesh::draw() {
	glBindVertexArray(m_vao);
	if (m_element_count) glDrawArrays(m_draw_mode, 0, m_element_count);
}

static constexpr auto cube_pos = std::to_array<GLfloat>({
	0.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	
	0.0, 1.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	
	1.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	1.0, 0.0, 0.0,
	
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 0.0, 0.0,
	
	1.0, 0.0, 1.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	
	1.0, 1.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 0.0, 1.0,
	
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	
	1.0, 1.0, 1.0,
	1.0, 0.0, 1.0,
	0.0, 1.0, 1.0,
	
	0.0, 0.0, 1.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 1.0,
	
	1.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	1.0, 1.0, 0.0,
	
	0.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
});

static constexpr auto cube_uv = std::to_array<GLfloat>({
	0.0, 0.0,
	1.0, 0.0,
	0.0, 1.0,
	
	1.0, 1.0,
	0.0, 1.0,
	1.0, 0.0,
	
	0.0, 0.0,
	1.0, 0.0,
	0.0, 1.0,
	
	1.0, 1.0,
	0.0, 1.0,
	1.0, 0.0,
	
	0.0, 0.0,
	0.0, 1.0,
	1.0, 0.0,
	
	1.0, 1.0,
	1.0, 0.0,
	0.0, 1.0,
	
	1.0, 1.0,
	1.0, 0.0,
	0.0, 1.0,
	
	0.0, 0.0,
	0.0, 1.0,
	1.0, 0.0,
	
	1.0, 1.0,
	1.0, 0.0,
	0.0, 1.0,
	
	0.0, 0.0,
	0.0, 1.0,
	1.0, 0.0,
	
	1.0, 1.0,
	0.0, 1.0,
	1.0, 0.0,
	
	0.0, 0.0,
	1.0, 0.0,
	0.0, 1.0,
});

template <size_t N>
consteval static std::array<M1::BasicMesh::Vertex, N> assemble_primitive(std::span<float const, N * 3> pos, std::span<float const, N * 2> uv) {
	std::array<M1::BasicMesh::Vertex, N> verts;
	for (size_t i = 0; i < N; i++) {
		verts[i].pos = {
			pos[i*3+0],
			pos[i*3+1],
			pos[i*3+2]
		};
		verts[i].uv = {
			uv[i*2+0],
			uv[i*2+1],
		};
	}
	return verts;
}

void M1::RendM1::initialize_meshes() {
	static constexpr auto cube_verts = assemble_primitive<36>( cube_pos, cube_uv );
	
	unit_cube = std::make_shared<BasicMesh>( cube_verts );
}
