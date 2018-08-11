//
//  VertexBufferLayout.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXBUFFERLAYOUT_H_
#define ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXBUFFERLAYOUT_H_

#include <vector>
#include "RenderCommon.h"

namespace rendering {
	/**
	* @brief Abstraction of a vertex buffer element
	* @author Anna
	*/
	struct VertexBufferElement {
		unsigned int type;
		unsigned int count;
		unsigned int normalized;

		static unsigned int GetTypeSize(unsigned int type) {
			switch (type) {
			case GL_FLOAT:          return sizeof(GLfloat);
			case GL_UNSIGNED_INT:   return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:  return sizeof(GLbyte);
			}
			return 0;
		}
	};

	/**
	* @brief Abstraction of a vertex buffer layout
	* @author Anna
	*/
	class VertexBufferLayout {
	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;	/**< Distance between elements */

	public:
		/**
		* @brief Constructor
		*/
		VertexBufferLayout() : m_Stride(0) {}

		/**
		* @brief Templated function for adding an element to the buffer layout
		* @param unsigned int count the number of elements to push
		*/
		template<typename T>
		void Push(unsigned int count) { __debugbreak(); }

		/**
		 * @brief Get the elements in the layout
		 * @return std::vector<VertexBufferElement>
		 */
		const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
		
		/**
		 * @brief Getter
		 * @return unsigned int stride
		 */
		unsigned int GetStride() const { return m_Stride; }
	};

	/**
	 * @brief Specialised function for adding floats to the buffer layout
	 * @param unsigned int count the number of floats to push
	 */
	template<>
	inline void VertexBufferLayout::Push<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetTypeSize(GL_FLOAT) * count;
	}

	/**
	 * @brief Specialised function for adding unsigned ints to the buffer layout
	 * @param unsigned int count the number of unsigned ints to push
	 */
	template<>
	inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetTypeSize(GL_UNSIGNED_INT) * count;
	}

	/**
	* @brief Specialised function for adding unsigned char to the buffer layout
	* @param unsigned int count the number of unsigned char to push
	*/
	template<>
	inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetTypeSize(GL_UNSIGNED_BYTE) * count;
	}
}; /* rendering */
#endif /* ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXBUFFERLAYOUT_H_ */