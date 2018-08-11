//
//  render.h
//  RenderingEngine
//
//  Created by Brianna Whitcher
//

#ifndef ZENENGINE_CORE_RENDERING_RENDER_H_
#define ZENENGINE_CORE_RENDERING_RENDER_H_

#include "render_detail/IndexBuffer.h"
#include "render_detail/RenderCommon.h"
#include "render_detail/Shader.h"
#include "render_detail/TextureBundle.h"
#include "render_detail/VertexArray.h"
#include "render_detail/VertexBuffer.h"
#include "render_detail/VertexBufferLayout.h"
#include "render_detail/VertexLayout.h"

typedef rendering::IndexBuffer			  IndexBuffer;
typedef rendering::Shader				  Shader;
typedef rendering::Texture				  Texture;
typedef rendering::TextureBundle		  TextureBundle;
typedef rendering::VertexBuffer			  VertexBuffer;
typedef rendering::Vertex<TexturedVertex> Vertex;
typedef rendering::VertexBufferLayout	  VertexBufferLayout;
typedef rendering::VertexArray			  VertexArray;

#endif /* ZENENGINE_CORE_RENDERING_RENDER_H_ */