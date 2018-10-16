#pragma once

#include <vector>
#include "LinearMath\btIDebugDraw.h"
#include "..\Renderer\Shader.h"
#include "..\Controllers\Camera.h"
#include "..\Common\MyMath.h"

class DebugDraw : public btIDebugDraw
{
public:
	DebugDraw() 
	{
		// Debug draw shader init
		m_debugShader = new Shader();

		// Camera object for MVP matrix
		m_camera = new Camera();

		InitDebugDraw();
	}

	~DebugDraw() { }

		/**
		* @brief Initialises the debug draw
		*
		*
		*
		* @return void
		*/
	void InitDebugDraw()
	{
		// Create a debug shader source (vertext and fragment shader)
		ShaderSource debugShaderSource = ParseShaders("Resources/shaders/DebugDraw.shader");

		// Initialise the shader program for the physics engine
		GetDebugShader()->Initialize(debugShaderSource.VertexSource, debugShaderSource.FragmentSource);
	}

	void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix)
	{
		glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix();
		//glm::mat4 modelMatrix = CreateTransformationMatrix(m_debugMesh->GetPosition(), m_debugMesh->GetRotation(), m_debugMesh->GetScale());
		glm::mat4 viewMatrix = CreateViewMatrix(m_camera);

		//GLint modelMatrixId = m_debugShader->GetVariable("model");
		GLint viewMatrixId = m_debugShader->GetVariable("view");
		GLint projectionMatrixId = m_debugShader->GetVariable("projection");

		//m_debugShader->SetMatrix4(modelMatrixId, 1, false, &m_modelMatrix[0][0]);
		m_debugShader->SetMatrix4(viewMatrixId, 1, false, &pViewMatrix[0][0]);
		m_debugShader->SetMatrix4(projectionMatrixId, 1, false, &pProjectionMatrix[0][0]);
	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(m_debugLines[0]) * m_debugLines.size(), &m_debugLines[0], GL_STATIC_DRAW);

		// load the vertex data info
		glVertexAttribPointer(1,  // the handle for the inPos shader attrib should be at pos 0
			3,	// there are 3 values xyz
			GL_FLOAT, // float value
			GL_FALSE, // don't need to be normalised
			4 * sizeof(float),  // how many floats to the next one (btVector3 uses 4 floats)
			(GLvoid*)&m_debugLines[0]  // where do they start as an index); // use 3 values, but add stride each time to get to the next
		);

		glBindVertexArray(0);

		m_debugShader->TurnOn();

		// Bind the VAO
		glBindVertexArray(VAO);

		// Enable the position attribute
		glEnableVertexAttribArray(0);

		// Draw the lines
		glDrawArrays(GL_TRIANGLES, 0, m_debugLines.size());

		// Disable the position attribute
		glDisableVertexAttribArray(0);

		// Unbind the VAO
		glBindVertexArray(0);

		// Drawn once then cleared
		//m_debugLines.clear();

		// Disable shader
		m_debugShader->TurnOff();
	}

	virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) { }
	virtual void reportErrorWarning(const char *) { }
	virtual void draw3dText(const btVector3 &, const char *) { }
	virtual void setDebugMode(int p) { m = p; }
	int getDebugMode(void) const { return 3; }
	int m;

	Shader* GetDebugShader() { return m_debugShader; };

	void SetCamera(Camera* camera) { m_camera = camera; }
	
	unsigned int VBO, VAO;
	
protected:
	std::vector<btVector3> m_debugLines;

	Shader* m_debugShader;

	// Used to alter the scale, position, rotation of the debug draw (lines)
	glm::mat4 m_modelMatrix;

	// Mesh* m_debugMesh;

	Camera* m_camera;
};