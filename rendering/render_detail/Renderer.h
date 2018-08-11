//
//  Renderer.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDERER_H_
#define ZENENGINE_CORE_RENDERING_RENDERER_H_

#include "RenderCommon.h"

namespace rendering {
	class Renderer {
	private:
		//rendering::Shader m_Shader;

	public:
			/**
			 * @brief		Default constructor.
			 */
		Renderer() {
			ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			Clear();
			DepthTest();
			glEnable(GL_CULL_FACE);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		};

			/**
			 * @brief		Clears the colour and depth buffers.
			 */
		void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

		//void Draw() { } "but in the next implementation sh*ts gonna get cool"(tm) -anna

			/**
			 * @brief		Resets the colour of the window to the colour specified by rgba.
			 * @param[in]	r	The red value of the colour.
			 * @param[in]	g	The green value of the colour.
			 * @param[in]	b	The blue value of the colour.
			 * @param[in]	a	The alpha value of the colour.
			 */
		void ClearColor(float r, float g, float b, float a = 1.0f) { glClearColor(r, g, b, a); }

			/**
			 * @brief		Enables depth testing.
			 */
		void DepthTest() {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}
	};
};
#endif /* ZENENGINE_CORE_RENDERING_RENDERER_H_ */