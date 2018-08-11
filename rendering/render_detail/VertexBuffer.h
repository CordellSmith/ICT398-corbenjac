//
//  VertexBuffer.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXBUFFER_H_
#define ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXBUFFER_H_

/**
 * @brief Abstraction of a Vertex Buffer
 * @author Anna
 */
namespace rendering {
	class VertexBuffer {
	private:
		unsigned int m_ID;
		unsigned int m_Size;

	public:
		/**
		 * @brief Constructor
		 * @post Make sure to fill the buffer before use
		 */
		VertexBuffer();

		/**
		 * @brief Constructor
		 * @param data a pointer to some contiguous data structure
		 * @param size the size of the structure. e.g NumberOfElements * ElementSize
		 */
		VertexBuffer(const void *data, unsigned int size);

		/**
		 * @brief Destructor for a vertex buffer
		 */
		~VertexBuffer();

		/**
		 * @brief Fills the vertex buffer with the given data
		 * @param data a pointer to some contiguous data structure
		 * @param size the size of the structure. e.g NumberOfElements * ElementSize
		 */
		void SetBufferData(const void *data, unsigned int size);
		
		/**
		 * @brief binds this vertex buffer as active
		 */
		void Bind() const;
		
		/**
		 * @brief unbinds the active vertex buffer
		 */
		void Unbind() const;
		
		/**
		 * @brief Draws the vertex buffer contents
		 */
		void Draw();

		/**
		* @brief Draws the vertex buffer contents in wireframe
		*/
		void DebugDraw();

	}; /* VertexBuffer */
}; /* rendering */

#endif /* ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXBUFFER_H_ */