#pragma once

#include "ControlPoint.h"

#include <GLFW/glfw3.h>

class App
{
public:
	static const int WindowWidth = 800;
	static const int WindowHeight = 600;

	App();
	~App();

	[[nodiscard]] float GetPointX(int index) const;
	[[nodiscard]] float GetPointY(int index) const;
	void SetPointPosition(int index, float x, float y);
	void SetPointDragging(int index, bool dragging);

	[[nodiscard]] int GetDraggedIndex() const;
	void SetDraggedIndex(int index);

	bool Initialize();
	void Run();
	void Shutdown();

private:
	bool InitializeGLFW();
	static void SetupScene();
	void Render();

	GLFWwindow* m_window;
	ControlPoint m_points[4];
	int m_draggedPointIndex;
};
