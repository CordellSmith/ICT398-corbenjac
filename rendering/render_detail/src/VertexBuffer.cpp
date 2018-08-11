//
//  VertexBuffer.cpp
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#include "../VertexBuffer.h"
#include "../RenderCommon.h"

namespace rendering {
	VertexBuffer::VertexBuffer() {
		m_ID = 0;
		m_Size = 0;
	}

	VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		m_Size = size;
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_ID);
	}

	void VertexBuffer::SetBufferData(const void *data, unsigned int size) {
		glGenBuffers(1, &m_ID); 
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		m_Size = size;
	}

	void VertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Draw() {
		Bind();

		glDrawArrays(GL_TRIANGLES, 0, m_Size);

		Unbind();
	}

	void VertexBuffer::DebugDraw() {
		Bind();

		glDrawArrays(GL_LINE_LOOP, 0, m_Size);

		Unbind();
	}
};		/* rendering */