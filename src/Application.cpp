#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define VELOCITY 0.001f

float xV1, yV1, xP1, yP1;
float xV2, yV2, xP2, yP2;

void move(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_W: yV1 -=  VELOCITY; break;
			case GLFW_KEY_A: xV1 -= -VELOCITY; break;
			case GLFW_KEY_S: yV1 -= -VELOCITY; break;
			case GLFW_KEY_D: xV1 -=  VELOCITY; break;
			case GLFW_KEY_UP:    yV2 -=  VELOCITY; break;
			case GLFW_KEY_LEFT:  xV2 -= -VELOCITY; break;
			case GLFW_KEY_DOWN:  yV2 -= -VELOCITY; break;
			case GLFW_KEY_RIGHT: xV2 -=  VELOCITY; break;
			default: break;
		}
	} else if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_W: yV1 +=  VELOCITY; break;
			case GLFW_KEY_A: xV1 += -VELOCITY; break;
			case GLFW_KEY_S: yV1 += -VELOCITY; break;
			case GLFW_KEY_D: xV1 +=  VELOCITY; break;
			case GLFW_KEY_UP:    yV2 +=  VELOCITY; break;
			case GLFW_KEY_LEFT:  xV2 += -VELOCITY; break;
			case GLFW_KEY_DOWN:  yV2 += -VELOCITY; break;
			case GLFW_KEY_RIGHT: xV2 +=  VELOCITY; break;
			default: break;
		}
	}
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

	xV1 = yV1 = xV2 = yV2 = 0;
	xP1 = yP1 = -0.5f;
	xP2 = yP2 = 0.5f;
	glfwSetKeyCallback(window, move);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

		xP1 += xV1;
		yP1 += yV1;
		xP2 += xV2;
		yP2 += yV2;

		glBegin(GL_TRIANGLES);
		glVertex2f(xP1, yP1);
		glVertex2f(xP2, yP2);
		glVertex2f(0, 0);
		glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

