#include "App.hpp"
#include "BptFileLoader.hpp"
#include "BmpFileLoader.hpp"
#include "Geo/Transformation.hpp"

#define CLASSNAME "render"

App *App::sInstance;

App::App()
{
	sInstance = this;
}

int App::run(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = wndProcStub;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASSNAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	mFramebuffer = Draw::Framebuffer(800, 600, 4);

	RECT rect;
	rect.left = 10;
	rect.top = 50;
	rect.right = rect.left + mFramebuffer.width();
	rect.bottom = rect.top + mFramebuffer.height();
	AdjustWindowRect(&rect, WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE, FALSE);
	mHWnd = CreateWindowEx((DWORD)0, CLASSNAME, "Render", WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInst, NULL);

	HDC hDC = GetDC(mHWnd);
	mBackDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, mFramebuffer.width(), mFramebuffer.height());
	ReleaseDC(mHWnd, hDC);
	HGDIOBJ oldBitmap = SelectObject(mBackDC, (HGDIOBJ)hBitmap);
	DeleteObject(oldBitmap);

	std::unique_ptr<Render::Texture> texture = BmpFileLoader::load("bricks.bmp");
	mObject = BptFileLoader::load("teapot.bpt", *texture);
	draw();

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK App::wndProcStub(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return sInstance->wndProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK App::wndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			BitBlt(ps.hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, mBackDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

			char buf[80];
			sprintf_s(buf, sizeof(buf), "Draw Time: %ims", mDrawTime);
			SetBkMode(ps.hdc, TRANSPARENT);
			TextOut(ps.hdc, 0, 0, buf, strlen(buf));

			EndPaint(hWnd, &ps);
			break;
		}

		case WM_LBUTTONUP:
		{
			mFramebuffer.clear(Draw::Color(0.5f, 0.5f, 0.5f));
			mFramebuffer.postMultisampleBuffer();
			postFramebuffer();
			SetTimer(hWnd, 0, 0, NULL);
			return 0;
		}

		case WM_TIMER:
		{
			draw();
			KillTimer(hWnd, wParam);
			return 0;
		}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void App::postFramebuffer()
{
	BITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = mFramebuffer.width();
	bi.bmiHeader.biHeight = mFramebuffer.height();
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;

	HBITMAP hBitmap = (HBITMAP)GetCurrentObject(mBackDC, OBJ_BITMAP);
	SetDIBits(mBackDC, hBitmap, 0, mFramebuffer.height(), mFramebuffer.displayColorBits(), &bi, DIB_RGB_COLORS);
	InvalidateRect(mHWnd, NULL, FALSE);
}

void App::draw()
{
	Render::Config config(mFramebuffer);
	Geo::Matrix m = Geo::Transformation::rotate(100, 0, 0) * Geo::Transformation::translate(0, 2, -23);
	config.setView(m.inverse());
	config.setProjection(Geo::Transformation::perspective(0.25f * float(mFramebuffer.width()) / float(mFramebuffer.height()), 0.25f, 1.0f, 30.0f));
	config.setViewport(Geo::Transformation::viewport(0.0f, 0.0f, float(mFramebuffer.width()), float(mFramebuffer.height()), 0.0f, 1.0f));
	config.setType(Render::Config::Type::Solid);

	mFramebuffer.clear(Draw::Color(0.5f, 0.5f, 0.5f));

	int startTicks = GetTickCount();
	mObject->render(config);
	mDrawTime = GetTickCount() - startTicks;

	mFramebuffer.postMultisampleBuffer();

	postFramebuffer();
}