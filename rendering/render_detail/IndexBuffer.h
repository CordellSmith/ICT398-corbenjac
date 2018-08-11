//
//  IndexBuffer.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDERDETAIL_INDEXBUFFER_H_
#define ZENENGINE_CORE_RENDERING_RENDERDETAIL_INDEXBUFFER_H_

namespace rendering {
	/**
	* @brief Abstraction for an Index Buffer
	* @author Anna
	*/
	class IndexBuffer {
	private:
		unsigned int m_ID;
		unsigned int m_IndexCount;

	public:
		IndexBuffer();
		IndexBuffer(const void * data, unsigned int count);
		~IndexBuffer();

		/**
		 * @brief Fills the buffer with the specified data
		 * @param data
		 * @param count the number of elements in the data structure
		 */
		void SetBufferData(const void * data, unsigned int count);
		
		/**
		 * @brief Makes this Index Buffer the active index buffer
		 */
		void Bind();

		/**
		 * @brief Unbinds all active index buffers
		 */
		void Unbind();

		/**
		 * @brief draws the buffer contents
		 * @pre should be used instead of a vertex buffer if both are present
		 */
		void Draw();

		/**
		* @brief draws the buffer contents in wireframe
		* @pre should be used instead of a vertex buffer if both are present
		*/
		void DebugDraw();

		/**
		 * @brief gets the count of elements
		 * @return index count
		 */
		unsigned int GetIndexCount() const { return m_IndexCount; }

	};	/* IndexBuffer */
};		/* rendering */

#endif	/* ZENENGINE_CORE_RENDERING_RENDERDETAIL_INDEXBUFFER_H_ */

