#include "..\Renderer\Shader.h"

std::string Shader::LoadShaderFile(std::string filePath)
{
	std::ifstream fin(filePath.c_str());

	if ( !fin )
		return "";

	std::string strLine = "";
	std::string strText = "";

	while ( getline(fin, strLine) )
	{
		strText = strText + "\n" + strLine;
	}
	fin.close();

	return strText;
}

void Shader::Initialize(std::string strVertexFile, std::string strFragmentFile)
{
	std::string vShaderStr, fShaderStr;

	if ( !strVertexFile.length() || !strFragmentFile.length() )
		return;

	if ( m_vertexShaderId || m_fragmentShaderId || m_shaderProgramId )
		Destroy();

	GLenum ErrorCheckValue = glGetError();

	// Assign VS and FS ids
	m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	vShaderStr = strVertexFile.c_str();
	fShaderStr = strFragmentFile.c_str();

	const char *szVShader = vShaderStr.c_str();
	const char *szFShader = fShaderStr.c_str();

	glShaderSource(m_vertexShaderId, 1, &szVShader, nullptr);
	glShaderSource(m_fragmentShaderId, 1, &szFShader, nullptr);
	
	// Compile shaders
	glCompileShader(m_vertexShaderId);
	glCompileShader(m_fragmentShaderId);

	// Create shader program
	m_shaderProgramId = glCreateProgram();

	// Attach shdaers to program
	glAttachShader(m_shaderProgramId, m_vertexShaderId);
	glAttachShader(m_shaderProgramId, m_fragmentShaderId);

	// Link our program with OpenGL
	glLinkProgram(m_shaderProgramId);

	ErrorCheckValue = glGetError();

	if ( ErrorCheckValue != GL_NO_ERROR )
	{
		std::cout << "ERROR: Could not create the shader program with error Id: " << ErrorCheckValue << std::endl;
		exit(-1);
	}
}

GLint Shader::GetVariable(std::string strVariable)
{
	if(!m_shaderProgramId)
		return -1;

	return glGetUniformLocation(m_shaderProgramId, strVariable.c_str());
}

void Shader::Destroy()
{
	if(m_vertexShaderId)
	{
		glDetachShader(m_shaderProgramId, m_vertexShaderId);
		glDeleteShader(m_vertexShaderId);
		m_vertexShaderId = 0;
	}

	if(m_fragmentShaderId)
	{
		glDetachShader(m_shaderProgramId, m_fragmentShaderId);
		glDeleteShader(m_fragmentShaderId);
		m_fragmentShaderId = 0;
	}

	if(m_shaderProgramId)
	{
		glDeleteShader(m_shaderProgramId);
		m_shaderProgramId = 0;
	}
}