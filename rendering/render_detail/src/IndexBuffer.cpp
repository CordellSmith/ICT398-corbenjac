//
//  IndexBuffer.cpp
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#include "../IndexBuffer.h"
#include "../RenderCommon.h"

namespace rendering {
	IndexBuffer::IndexBuffer() { 
		m_ID = 0;
		m_IndexCount = 0;
	}

	IndexBuffer::IndexBuffer(const void * data, unsigned int count) : m_IndexCount(count) {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_ID);
	}

	void IndexBuffer::SetBufferData(const void * data, unsigned int count) {
		m_IndexCount = count;
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	void IndexBuffer::Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void IndexBuffer::Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::Draw() {
		//Bind();

		glDrawElements(GL_TRIANGLE_STRIP, m_IndexCount, GL_UNSIGNED_INT, (void *)0);
			
		//Unbind();
	}

	void IndexBuffer::DebugDraw() {
		//Bind();

		glDrawElements(GL_LINE_STRIP, m_IndexCount, GL_UNSIGNED_INT, (void *)0);

		//Unbind();
	}
};		/* rendering */