#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Game.hpp"

static void parseShader(const std::string& filepath, std::string *out_vss, std::string *out_fss) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	#define getShaderType(line) line.substr(8, 99)

	std::string line;
	std::stringstream ss[2];
	ShaderType currType = ShaderType::NONE;
	while (getline(stream, line)) {

		if (line.substr(0, 7) == "#shader") {

			if (getShaderType(line) == "vertex")
				currType = ShaderType::VERTEX;
			else if (getShaderType(line) == "fragment")
				currType = ShaderType::FRAGMENT;

		} else {
			ss[(int)currType] << line << '\n';
		}

	}

	if (out_vss) *out_vss = ss[0].str();
	if (out_fss) *out_fss = ss[1].str();
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile shader: " << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void) {
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Polygonnider", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << glGetString(GL_VERSION) << std::endl;

	unsigned int bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *) 0);

	std::string vsSource, fsSource;
	parseShader("res/shaders/Basic.shader", &vsSource, &fsSource);
	unsigned int program = createShader(vsSource, fsSource);
	glUseProgram(program);

	Game* game = Game::getInstance();
	glfwSetKeyCallback(window, Game::OnKeyAction);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		game->update();

		int size;
		float* vertexes = game->getVertexes(&size);
		glBufferData(GL_ARRAY_BUFFER, size, vertexes, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteProgram(program);
	glfwTerminate();
	return 0;
}
