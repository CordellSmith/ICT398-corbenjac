#pragma once

	/**
	* @class
	* @brief
	*
	* @author Cordell Smith
	* @version 01
	* @date 31/05/2018
	*
	*/
class IRenderer
{
public:
	IRenderer() { }
	~IRenderer() { }

	virtual void Prepare() = 0;
	virtual void Render() = 0;

protected:

};