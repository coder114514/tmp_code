#ifndef UNICODE
#define UNICODE
#endif

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "winmm")

#include <windows.h>
#include <windowsx.h>

#include <stdlib.h>
#include <math.h>


const int WIDTH = 900;
const int HEIGHT = 720;

const double PI = acos(-1.0);


struct StateInfo {
	HWND hwnd;
	bool running;
} state_info;


struct Player {
	double x, y, z, angle;
} player;


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	case WM_ERASEBKGND: {
		return 1;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_COMMAND: {
	case 0:
		MessageBoxW(hwnd, L"move forward: up arrow\nmove backward: down error\nturn left: left arrow\nturn right: right arrow\nincrease fov: =\ndecrease fov: -\nfly up: 0\nfly down:9", L"Help", MB_OK);
		return 0;
	}
	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}


double cross(double x1, double y1, double x2, double y2) {
	return x1 * y2 - x2 * y1;
}

bool intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double& x, double& y) {
	double denom = cross(x1 - x2, y1 - y2, x3 - x4, y3 - y4);
	if (denom < 1e-5 && denom > -1e-5) return false;
	double det12 = cross(x1, y1, x2, y2);
	double det34 = cross(x3, y3, x4, y4);
	x = cross(det12, x1 - x2, det34, x3 - x4) / denom;
	y = cross(det12, y1 - y2, det34, y3 - y4) / denom;
	return true;
}


void draw_line(HDC mem_dc, double x1, double y1, double x2, double y2, COLORREF color, int width = 1) {
	HPEN pen = CreatePen(PS_SOLID, width, color);
	HPEN old_pen = (HPEN)SelectObject(mem_dc, pen);
	MoveToEx(mem_dc, (int)x1, (int)y1, nullptr);
	LineTo(mem_dc, (int)x2, (int)y2);
	SelectObject(mem_dc, old_pen);
	DeleteObject(pen);
}

void convas_coord_to_screen_coord(double x, double y, int centerx, int centery, double& sx, double& sy) {
	sx = centerx + x;
	sy = centery - y;
}

void draw_line_w_w2s(int centerx, int centery, HDC mem_dc, double x1, double y1, double x2, double y2, COLORREF color, int width = 1) {
	convas_coord_to_screen_coord(x1, y1, centerx, centery, x1, y1);
	convas_coord_to_screen_coord(x2, y2, centerx, centery, x2, y2);
	draw_line(mem_dc, x1, y1, x2, y2, color, width);
}

void draw_dot(HDC mem_dc, double x, double y, COLORREF color) {
	HBRUSH brush = CreateSolidBrush(color);
	HBRUSH old_brush = (HBRUSH)SelectObject(mem_dc, brush);
	Ellipse(mem_dc, (int)x - 5, (int)y - 5, (int)x + 5, (int)y + 5);
	SelectObject(mem_dc, old_brush);
	DeleteObject(brush);
}


const int x_shift = 35; // screen coord
const double vx1 = 50, vy1 = 125; // world coord
const double vx2 = 50, vy2 = -125; // world coord
const double vx3 = -100, vy3 = 0; // world coord
const double WALL_HEIGHT = 20;
double fov = 90;

void update() {
	if (GetAsyncKeyState(VK_UP)) {
		player.x += cos(player.angle);
		player.y += sin(player.angle);
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		player.x -= cos(player.angle);
		player.y -= sin(player.angle);
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		player.angle += 0.05;
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		player.angle -= 0.05;
	}

	if (GetAsyncKeyState(VK_OEM_PLUS)) {
		if (fov < 179.0) fov += 0.5;
	}
	else if (GetAsyncKeyState(VK_OEM_MINUS)) {
		if (fov > 1.0) fov -= 0.5;
	}

	if (GetAsyncKeyState(0x30)) {
		if (player.z < WALL_HEIGHT) player.z += 1;
	}
	else if (GetAsyncKeyState(0x39)) {
		if (player.z > 0) player.z -= 1;
	}
}

HRGN setup_view(HDC mem_dc, COLORREF color, const RECT& rc) {
	HBRUSH brush = CreateSolidBrush(color);
	FrameRect(mem_dc, &rc, brush);
	DeleteObject(brush);
	HRGN region = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
	SelectClipRgn(mem_dc, region);
	return region;
}

void end_view(HDC mem_dc, HRGN hrgn) {
	SelectClipRgn(mem_dc, nullptr);
	DeleteObject(hrgn);
}

void draw_absolute_view(HDC mem_dc) {
	double spx, spy;
	RECT rc = { x_shift + 20, 100, x_shift + 270, 620 };
	HRGN hrgn = setup_view(mem_dc, RGB(0, 0, 255), rc);
	const int centerx = x_shift + 145;
	const int centery = 360;

	convas_coord_to_screen_coord(player.x, player.y, centerx, centery, spx, spy);
	draw_dot(mem_dc, spx, spy, RGB(255, 255, 255));
	draw_line_w_w2s(centerx, centery, mem_dc, player.x, player.y, player.x + 10 * cos(player.angle), player.y + 10 * sin(player.angle), RGB(255, 255, 255));

	draw_line_w_w2s(centerx, centery, mem_dc, vx1, vy1, vx2, vy2, RGB(0, 255, 255));
	draw_line_w_w2s(centerx, centery, mem_dc, vx3, vy3, vx2, vy2, RGB(255, 114, 255));
	draw_line_w_w2s(centerx, centery, mem_dc, vx1, vy1, vx3, vy3, RGB(114, 114, 255));

	draw_line_w_w2s(centerx, centery, mem_dc, player.x, player.y, 520 * cos(-fov / 360.0 * PI + player.angle), 520 * sin(-fov / 360.0 * PI + player.angle), RGB(114, 114, 114)); // fov
	draw_line_w_w2s(centerx, centery, mem_dc, player.x, player.y, 520 * cos(fov / 360.0 * PI + player.angle), 520 * sin(fov / 360.0 * PI + player.angle), RGB(114, 114, 114)); // fov

	end_view(mem_dc, hrgn);
}

void draw_first_person_view_2D(HDC mem_dc) {
	double spx, spy;
	RECT rc = { x_shift + 290, 100, x_shift + 540, 620 };
	HRGN hrgn = setup_view(mem_dc, RGB(255, 255, 0), rc);
	const int centerx = x_shift + 415;
	const int centery = 360;

	convas_coord_to_screen_coord(0, 0, centerx, centery, spx, spy);
	draw_dot(mem_dc, spx, spy, RGB(255, 255, 255));
	draw_line_w_w2s(centerx, centery, mem_dc, 0, 0, 0, 10, RGB(255, 255, 255));

	double rx1 = vx1 - player.x;
	double ry1 = vy1 - player.y;
	double rx2 = vx2 - player.x;
	double ry2 = vy2 - player.y;
	double rx3 = vx3 - player.x;
	double ry3 = vy3 - player.y;

	double tx1 = rx1 * sin(player.angle) - ry1 * cos(player.angle);
	double ty1 = ry1 * sin(player.angle) + rx1 * cos(player.angle);
	double tx2 = rx2 * sin(player.angle) - ry2 * cos(player.angle);
	double ty2 = ry2 * sin(player.angle) + rx2 * cos(player.angle);
	double tx3 = rx3 * sin(player.angle) - ry3 * cos(player.angle);
	double ty3 = ry3 * sin(player.angle) + rx3 * cos(player.angle);

	draw_line_w_w2s(centerx, centery, mem_dc, tx1, ty1, tx2, ty2, RGB(0, 255, 255));
	draw_line_w_w2s(centerx, centery, mem_dc, tx3, ty3, tx2, ty2, RGB(255, 114, 255));
	draw_line_w_w2s(centerx, centery, mem_dc, tx1, ty1, tx3, ty3, RGB(114, 114, 255));

	draw_line_w_w2s(centerx, centery, mem_dc, 0, 0, 125, 125 / tan(fov / 360.0 * PI), RGB(114, 114, 114)); // fov
	draw_line_w_w2s(centerx, centery, mem_dc, 0, 0, -125, 125 / tan(fov / 360.0 * PI), RGB(114, 114, 114)); // fov

	end_view(mem_dc, hrgn);
}

void draw_poly(HDC mem_dc, COLORREF color, int centerx, int centery, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	HPEN old_pen = (HPEN)SelectObject(mem_dc, pen);
	HBRUSH old_brush = (HBRUSH)SelectObject(mem_dc, brush);

	double sx1, sy1, sx2, sy2, sx3, sy3, sx4, sy4;
	convas_coord_to_screen_coord(x1, y1, centerx, centery, sx1, sy1);
	convas_coord_to_screen_coord(x2, y2, centerx, centery, sx2, sy2);
	convas_coord_to_screen_coord(x3, y3, centerx, centery, sx3, sy3);
	convas_coord_to_screen_coord(x4, y4, centerx, centery, sx4, sy4);

	POINT p[4] = { {(int)sx1,(int)sy1},{(int)sx2,(int)sy2},{(int)sx3,(int)sy3},{(int)sx4,(int)sy4} };
	Polygon(mem_dc, p, 4);

	SelectObject(mem_dc, old_pen);
	SelectObject(mem_dc, old_brush);
	DeleteObject(pen);
	DeleteObject(brush);
}

void draw_a_wall_3D(HDC mem_dc, int centerx, int centery, double vx1, double vy1, double vx2, double vy2, COLORREF color) {
	double rx1 = vx1 - player.x;
	double ry1 = vy1 - player.y;
	double rx2 = vx2 - player.x;
	double ry2 = vy2 - player.y;

	double tx1 = rx1 * sin(player.angle) - ry1 * cos(player.angle);
	double ty1 = ry1 * sin(player.angle) + rx1 * cos(player.angle);
	double tx2 = rx2 * sin(player.angle) - ry2 * cos(player.angle);
	double ty2 = ry2 * sin(player.angle) + rx2 * cos(player.angle);

	if (cross(tx1, ty1, tx2, ty2) > 0) {
		double tmp;
		tmp = tx1, tx1 = tx2, tx2 = tmp;
		tmp = ty1, ty1 = ty2, ty2 = tmp;
	}

	if (ty1 > 0 || ty2 > 0) {
		if (ty1 <= 0) {
			double ix1, iy1;
			if (!intersect(tx1, ty1, tx2, ty2, 0, 0, -sin(fov / 360.0 * PI), cos(fov / 360.0 * PI), ix1, iy1)) {
				goto the_end;
			}
			tx1 = ix1, ty1 = iy1;
		}
		else if (ty2 <= 0) {
			double ix2, iy2;
			if (!intersect(tx1, ty1, tx2, ty2, 0, 0, sin(fov / 360.0 * PI), cos(fov / 360.0 * PI), ix2, iy2)) {
				goto the_end;
			}
			tx2 = ix2, ty2 = iy2;
		}

		double x1, y1a, y1b, x2, y2a, y2b;
		const double ratio = 250.0 / 2 / tan(fov / 360.0 * PI);
		const double ratio1 = ratio / ty1, ratio2 = ratio / ty2;
		x1 = tx1 * ratio1, y1a = (WALL_HEIGHT - player.z) * ratio1, y1b = -player.z * ratio1;
		x2 = tx2 * ratio2, y2a = (WALL_HEIGHT - player.z) * ratio2, y2b = -player.z * ratio2;
		draw_poly(mem_dc, RGB(0, 114, 255), centerx, centery, x1, 260, x1, y1a, x2, y2a, x2, 260);
		draw_poly(mem_dc, RGB(0, 255, 114), centerx, centery, x1, -260, x1, y1b, x2, y2b, x2, -260);
		draw_poly(mem_dc, color, centerx, centery, x1, y1a, x1, y1b, x2, y2b, x2, y2a);
	}
the_end:
	return;
}

void draw_first_person_view_3D(HDC mem_dc) {
	RECT rc = { x_shift + 560, 100, x_shift + 810, 620 };
	HRGN hrgn = setup_view(mem_dc, RGB(0, 255, 0), rc);
	const int centerx = x_shift + 685;
	const int centery = 360;
	draw_a_wall_3D(mem_dc, centerx, centery, vx1, vy1, vx2, vy2, RGB(0, 255, 255));
	draw_a_wall_3D(mem_dc, centerx, centery, vx3, vy3, vx2, vy2, RGB(255, 114, 255));
	draw_a_wall_3D(mem_dc, centerx, centery, vx1, vy1, vx3, vy3, RGB(114, 114, 255));
	end_view(mem_dc, hrgn);
}

void render() {
	//// Begin rendering
	HDC hdc = GetDC(state_info.hwnd);
	HDC mem_dc = CreateCompatibleDC(hdc);
	RECT wnd_rect;
	GetClientRect(state_info.hwnd, &wnd_rect);
	HBITMAP bmp = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
	HBITMAP old_bmp = (HBITMAP)SelectObject(mem_dc, bmp);

	//// Rendering
	FillRect(mem_dc, &wnd_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	draw_absolute_view(mem_dc);
	draw_first_person_view_2D(mem_dc);
	draw_first_person_view_3D(mem_dc);

	//// End rendering
	BitBlt(hdc, 0, 0, WIDTH, HEIGHT, mem_dc, 0, 0, SRCCOPY);
	SelectObject(mem_dc, old_bmp);
	DeleteObject(bmp);
	DeleteObject(mem_dc);
	ReleaseDC(state_info.hwnd, hdc);
}


DWORD WINAPI run(void* arg) {
	UNREFERENCED_PARAMETER(arg);
	double old_time = timeGetTime();
	double accumulator = 0.0;
	while (state_info.running) {
		double delta_time = timeGetTime() - old_time;
		old_time = timeGetTime();
		accumulator += delta_time;
		while (accumulator >= 1000.0 / 62.0) {
			update();
			accumulator -= 1000.0 / 60.0;
			if (accumulator < 0) accumulator = 0;
		}
		render();
	}
	return 0;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	// Register the window class.
	WNDCLASS wc{};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MainWindow";
	wc.style = 0;
	wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	RegisterClassW(&wc);

	HMENU menu = CreateMenu();
	AppendMenuW(menu, MF_STRING, 0, L"&Help");

	// Create the window.
	DWORD style = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_CLIPCHILDREN;
	DWORD ex_style = 0;
	RECT  wnd_rect = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRectEx(&wnd_rect, style, false, ex_style);
	HWND hwnd = ::CreateWindowExW(
		ex_style,                       // Optional window styles
		L"MainWindow",                  // Window class
		L"A Room",                      // Window text
		style,                          // Window style
		CW_USEDEFAULT,                  // X
		CW_USEDEFAULT,                  // Y
		wnd_rect.right - wnd_rect.left, // Width
		wnd_rect.bottom - wnd_rect.top, // Height
		nullptr,                        // Parent window
		menu,                           // Menu
		hInstance,                      // Instance handle
		nullptr                         // Additional application data
	);
	if (!hwnd) return 1;
	::SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
	state_info.hwnd = hwnd;
	state_info.running = true;

	UpdateWindow(hwnd);
	ShowWindow(hwnd, nCmdShow);

	CreateThread(nullptr, 0, run, nullptr, 0, nullptr);

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	state_info.running = false;

	return (int)msg.wParam;
}
