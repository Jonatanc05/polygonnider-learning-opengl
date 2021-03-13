#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>
#include <assert.h>
#include "Game.hpp"

#if _DEBUG
static void APIENTRY debugCallback(GLenum source, GLenum type, unsigned id,
		GLenum severity, GLsizei length, const char *msg, const void *userParam) {
	std::cout << "[OPENGL ERROR]: ";
	for(unsigned i = 0; i < length; i++) {
		std::cout << msg[i];
	}
	std::cout << std::endl;
	if (type == GL_DEBUG_TYPE_ERROR)
		raise(SIGABRT);
}
#endif

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

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	window = glfwCreateWindow(640, 480, "Polygonnider", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << glGetString(GL_VERSION) << std::endl;
#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugCallback, nullptr);
	unsigned unusedIds = 0;
	glDebugMessageControl(
		 // DONT_CARE = don't filter, register me for all events
		GL_DONT_CARE,
		GL_DONT_CARE,
		GL_DONT_CARE,
		0,
		&unusedIds,
		true
	);
#endif

	unsigned int bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *) 0);

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		0, 1, 3
	};
	unsigned int idxBufferId;
	glGenBuffers(1, &idxBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9 * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);

	std::string vsSource, fsSource;
	parseShader("res/shaders/Basic.shader", &vsSource, &fsSource);
	unsigned int program = createShader(vsSource, fsSource);
	glUseProgram(program);

	int location = glGetUniformLocation(program, "u_color");
	assert(location != -1);

	Game* game = Game::getInstance();
	glfwSetKeyCallback(window, Game::OnKeyAction);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		game->update();
		int size;
		float* vertexes = game->getVertexes(&size);
		float* colors = game->getColor();

		glBufferData(GL_ARRAY_BUFFER, size, vertexes, GL_DYNAMIC_DRAW);

		glUniform4fv(location, 1, colors);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteProgram(program);
	glfwTerminate();
	return 0;
}
