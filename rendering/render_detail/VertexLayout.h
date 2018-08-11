//
//  VertexBufferLayout.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXLAYOUT_H_
#define ZENENGINE_CORE_RENDERING_RENDRDETAIL_VERTEXLAYOUT_H_

#include "RenderCommon.h"

constexpr unsigned int ColoredVertex  = 0;
constexpr unsigned int TexturedVertex = 1;

namespace rendering {
	/**
	* @brief Abstraction for a Vertex Layout
	* @author Anna
	*/
	template <unsigned int T> struct Vertex;

	/**
	 * @brief Abstraction for a Vertex with a color component
	 * @author Anna
	 */
	template <>
	struct Vertex<ColoredVertex> {
		gem::vec3f Position;
		gem::vec3f Color;
		gem::vec3f Normal;

		Vertex() : Position(gem::vec3f(0.0f, 0.0f, 0.0f)),
			Color(gem::vec3f(0.0f, 0.0f, 0.0f)),
			Normal(gem::vec3f(0.0f, 0.0f, 0.0f)) { }

		Vertex(float x,  float y,  float z,
			   float r,  float g,  float b,
			   float xn, float yn, float zn) :
			Position(gem::vec3f(x, y, z)),
			Color(gem::vec3f(r, g, b)),
			Normal(gem::vec3f(xn, yn, zn)) { }
	};

	/**
	 * @brief Abstraction for a Vertex with a texture component
	 * @author Anna
	 */
	template <>
	struct Vertex<TexturedVertex> {
		gem::vec3f Position;
		gem::vec2f UV;
		gem::vec3f Normal;

		Vertex() : Position(gem::vec3f(0.0f, 0.0f, 0.0f)),
			UV(gem::vec2f(0.0f, 0.0f)),
			Normal(gem::vec3f(0.0f, 0.0f, 0.0f)) { }

		Vertex(gem::vec3f position, gem::vec2f uv, gem::vec3f normal) :
			Position(position), UV(uv), Normal(normal) { }

		Vertex(float x, float y, float z,
			float u, float v,
			float xn, float yn, float zn) :
			Position(gem::vec3f(x, y, z)),
			UV(gem::vec2f(u, v)),
			Normal(gem::vec3f(xn, yn, zn)) { }
	};

}; /* rendering */
#endif /* ZENENGINE_CORE_RENDERING_RENDERDETAIL_VERTEXLAYOUT_H_ */