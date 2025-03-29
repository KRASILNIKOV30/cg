#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <GL/glu.h>
#include <glm/vec3.hpp>

// Размеры лабиринта
const int WIDTH = 16;
const int HEIGHT = 16;

// Текстуры стен (просто цвета для простоты)
struct Colors
{
	glm::vec3 floor = { 0.3f, 0.3f, 0.3f };
	glm::vec3 ceiling = { 0.2f, 0.2f, 0.4f };
	glm::vec3 wall1 = { 0.8f, 0.2f, 0.2f };
	glm::vec3 wall2 = { 0.2f, 0.8f, 0.2f };
} colors;

// Лабиринт (1 - стена, 0 - проход)
std::vector<std::vector<int>> maze = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1 },
	{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
	{ 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

// Позиция и направление игрока
glm::vec3 playerPos = { 1.5f, 0.5f, 1.5f };
float playerYaw = 0.0f; // Поворот вокруг Y
const float moveSpeed = 0.05f;
const float rotSpeed = 0.03f;

void drawWall(float x1, float y1, float z1, float x2, float y2, float z2, glm::vec3 color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z1);
	glEnd();
}

void drawFloor()
{
	glColor3f(colors.floor.r, colors.floor.g, colors.floor.b);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(WIDTH, 0, 0);
	glVertex3f(WIDTH, 0, HEIGHT);
	glVertex3f(0, 0, HEIGHT);
	glEnd();
}

void drawCeiling()
{
	glColor3f(colors.ceiling.r, colors.ceiling.g, colors.ceiling.b);
	glBegin(GL_QUADS);
	glVertex3f(0, 1, 0);
	glVertex3f(WIDTH, 1, 0);
	glVertex3f(WIDTH, 1, HEIGHT);
	glVertex3f(0, 1, HEIGHT);
	glEnd();
}

void renderMaze()
{
	drawFloor();
	drawCeiling();

	for (int x = 0; x < WIDTH; x++)
	{
		for (int z = 0; z < HEIGHT; z++)
		{
			if (maze[x][z] == 1)
			{
				glm::vec3 color = (x + z) % 2 ? colors.wall1 : colors.wall2;

				// Проверяем соседей, чтобы не рисовать скрытые грани
				if (x == 0 || maze[x - 1][z] == 0)
					drawWall(x, 0, z, x, 1, z, color);
				if (x == WIDTH - 1 || maze[x + 1][z] == 0)
					drawWall(x + 1, 0, z, x + 1, 1, z, color);
				if (z == 0 || maze[x][z - 1] == 0)
					drawWall(x, 0, z, x + 1, 0, z, color);
				if (z == HEIGHT - 1 || maze[x][z + 1] == 0)
					drawWall(x, 0, z + 1, x + 1, 0, z + 1, color);
			}
		}
	}
}

bool canMoveTo(float x, float z)
{
	int ix = static_cast<int>(x);
	int iz = static_cast<int>(z);

	if (ix < 0 || ix >= WIDTH || iz < 0 || iz >= HEIGHT)
		return false;

	return maze[ix][iz] == 0;
}

void processInput(GLFWwindow* window)
{
	// Вращение
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		playerYaw += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		playerYaw -= rotSpeed;

	// Движение
	float moveX = 0.0f, moveZ = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveX = sinf(playerYaw) * moveSpeed;
		moveZ = cosf(playerYaw) * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveX = -sinf(playerYaw) * moveSpeed;
		moveZ = -cosf(playerYaw) * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveX = cosf(playerYaw) * moveSpeed;
		moveZ = -sinf(playerYaw) * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveX = -cosf(playerYaw) * moveSpeed;
		moveZ = sinf(playerYaw) * moveSpeed;
	}

	// Проверка столкновений
	if (canMoveTo(playerPos.x + moveX, playerPos.z))
		playerPos.x += moveX;
	if (canMoveTo(playerPos.x, playerPos.z + moveZ))
		playerPos.z += moveZ;
}

void setupCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Позиция и направление камеры
	float lookX = playerPos.x + sinf(playerYaw);
	float lookZ = playerPos.z + cosf(playerYaw);

	gluLookAt(playerPos.x, playerPos.y, playerPos.z,
		lookX, playerPos.y, lookZ,
		0.0f, 1.0f, 0.0f);
}

int main()
{
	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(800, 600, "3D Maze", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);
		setupCamera();
		renderMaze();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}