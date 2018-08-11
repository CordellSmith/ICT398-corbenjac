//
//  VertexArray.cpp
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#include "../VertexArray.h"
#include "../VertexBufferLayout.h"
#include "../RenderCommon.h"
#include <iostream>

namespace rendering {
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_ID);
		Bind();
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_ID);
	}

	void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
		Bind();
		vb.Bind();
		
		const auto &elements = layout.GetElements();
		unsigned int offset = 0;

		unsigned int i;

		for (i = 0; i < elements.size(); i++)
		{
			const auto & element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset);
			offset += element.count * VertexBufferElement::GetTypeSize(element.type);
		}
	}

	void VertexArray::Bind() const {
		glBindVertexArray(m_ID);
	}

	void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}
};		/* rendering */