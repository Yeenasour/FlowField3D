#ifndef ENGINE_SHADER
#define ENGINE_SHADER

#include <string>
#include <GL/glew.h>


class Shader
{
private:
	GLuint programID;
	GLuint loadShader(GLuint type, const std::string& source);
public:
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();
	void use() const;
	GLuint getProgramID() const;

	//Uniforms
	void setUniform1f(const std::string& name, float value);
	void setUniform2f(const std::string& name, float val1, float val2);
	void setUniform1i(const std::string& name, int value);
	void setUniform2i(const std::string& name, int val1, int val2);
	void setUniform3f(const std::string& name, float f1, float f2, float f3);
	void setUniform4fv(const std::string&, const GLfloat *mat);
};

#endif