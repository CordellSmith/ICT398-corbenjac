//
//  VertexArray.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXARRAY_H_
#define ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXARRAY_H_

#include "VertexBuffer.h"

namespace rendering {

	class VertexBufferLayout;

	/**
	 * @brief Abstraction for a Vertex Array
	 * @author Anna
	 */
	class VertexArray {
	private:
		unsigned int m_ID;

	public:
		VertexArray();
		~VertexArray();

		/**
		* @brief Registers a Vertex Buffer with a Vertex array
		* @param vb A vertex buffer
		* @param layout the vertex buffer layout
		*/
		void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
		
		/**
		 * @brief binds this vertex array as active
		 */
		void Bind() const;

		/**
		 * Unbinds all active Vertex Arrays
		 */
		void Unbind() const;
	}; /* VertexArray */
}; /* rendering */

#endif /* ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXARRAY_H_ */