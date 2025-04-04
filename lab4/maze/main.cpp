#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <GL/glu.h>
#include <glm/vec3.hpp>

constexpr int SCREEN_WIDTH = 1400;
constexpr int SCREEN_HEIGHT = 900;

constexpr float NEAR = 0.1f;
constexpr float FAR = 100.0f;

constexpr float MOVE_SPEED = 0.05f;
constexpr float ROT_SPEED = 0.03f;

constexpr int WIDTH = 16;
constexpr int HEIGHT = 16;

struct Colors
{
	glm::vec3 floor = { 0.3f, 0.3f, 0.3f };
	glm::vec3 ceiling = { 0.2f, 0.2f, 0.4f };
	glm::vec3 wall1 = { 0.8f, 0.2f, 0.2f };
	glm::vec3 wall2 = { 0.2f, 0.8f, 0.2f };
} colors;

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

void DrawWall(const float x1, const float z1, const float x2, const float z2, const glm::vec3 color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	glVertex3f(x1, 0, z1);
	glVertex3f(x1, 1, z1);
	glVertex3f(x2, 1, z2);
	glVertex3f(x2, 0, z2);
	glEnd();
}

void DrawFloor()
{
	glColor3f(colors.floor.r, colors.floor.g, colors.floor.b);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(WIDTH, 0, 0);
	glVertex3f(WIDTH, 0, HEIGHT);
	glVertex3f(0, 0, HEIGHT);
	glEnd();
}

void DrawCeiling()
{
	glColor3f(colors.ceiling.r, colors.ceiling.g, colors.ceiling.b);
	glBegin(GL_QUADS);
	glVertex3f(0, 1, 0);
	glVertex3f(WIDTH, 1, 0);
	glVertex3f(WIDTH, 1, HEIGHT);
	glVertex3f(0, 1, HEIGHT);
	glEnd();
}

void RenderMaze()
{
	DrawFloor();
	DrawCeiling();

	for (int x = 0; x < WIDTH; x++)
	{
		for (int z = 0; z < HEIGHT; z++)
		{
			if (maze[x][z] == 1)
			{
				// разный цвет в зависимости от типа кубика
				const glm::vec3 color = (x + z) % 2 ? colors.wall1 : colors.wall2;

				const auto xf = static_cast<float>(x);
				const auto zf = static_cast<float>(z);

				DrawWall(xf, zf, xf, zf + 1, color);
				DrawWall(xf + 1, zf, xf + 1, zf + 1, color);
				DrawWall(xf, zf, xf + 1, zf, color);
				DrawWall(xf, zf + 1, xf + 1, zf + 1, color);
			}
		}
	}
}

bool CanMoveTo(float x, float z)
{
	const auto ix = static_cast<int>(x);
	const auto iz = static_cast<int>(z);

	if (ix < 0 || ix >= WIDTH || iz < 0 || iz >= HEIGHT)
	{
		return false;
	}

	return maze[ix][iz] == 0;
}

void ProcessInput(GLFWwindow* window, glm::vec3& playerPos, float& playerYaw)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		playerYaw += ROT_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		playerYaw -= ROT_SPEED;
	}

	float moveX = 0.0f;
	float moveZ = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveX = sinf(playerYaw) * MOVE_SPEED;
		moveZ = cosf(playerYaw) * MOVE_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveX = -sinf(playerYaw) * MOVE_SPEED;
		moveZ = -cosf(playerYaw) * MOVE_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveX = cosf(playerYaw) * MOVE_SPEED;
		moveZ = -sinf(playerYaw) * MOVE_SPEED;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveX = -cosf(playerYaw) * MOVE_SPEED;
		moveZ = sinf(playerYaw) * MOVE_SPEED;
	}

	if (CanMoveTo(playerPos.x + moveX, playerPos.z))
	{
		playerPos.x += moveX;
	}
	if (CanMoveTo(playerPos.x, playerPos.z + moveZ))
	{
		playerPos.z += moveZ;
	}
}

void setupCamera(const glm::vec3 playerPos, const float playerYaw)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), NEAR, FAR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const auto lookX = playerPos.x + sinf(playerYaw);
	const auto lookZ = playerPos.z + cosf(playerYaw);

	gluLookAt(playerPos.x, playerPos.y, playerPos.z,
		lookX, playerPos.y, lookZ,
		0.0f, 1.0f, 0.0f);
}

int main()
{
	glm::vec3 playerPos = { 1.5f, 0.5f, 1.5f };
	float playerYaw = 0.0f;

	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Maze", nullptr, nullptr);
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

		ProcessInput(window, playerPos, playerYaw);
		setupCamera(playerPos, playerYaw);
		RenderMaze();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}