#ifndef APP_HPP
#define APP_HPP

#include "Draw/Framebuffer.hpp"
#include "Render/Object.hpp"

#include <windows.h>
#include <memory>

class App
{
public:
	App();

	int run(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int iCmdShow);

private:
	static LRESULT CALLBACK wndProcStub(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static App *sInstance;

	LRESULT CALLBACK wndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	void postFramebuffer();

	void draw();

	HWND mHWnd;
	HDC mBackDC;
	Draw::Framebuffer mFramebuffer;
	std::unique_ptr<Render::Object> mObject;
};

#endif