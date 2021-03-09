// RayTracerGui.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "RayTracerGui.h"

#include "..\RayTracerLib\World.h"
#include "..\RayTracerLib\Sphere.h"
#include "..\RayTracerLib\Camera.h"

#define MAX_LOADSTRING 100

namespace rt = RayTracer;
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int HSIZE = 320;
int VSIZE = 280;
float FOV = 1.047196666666667f /* pi/3 */;
float ZOOM_DELTA = 0.1;
rt::Point FROM(0, 1.5f, -5);
rt::Point TO(0, 1, 0);
rt::Vector UP(0, 1, 0);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void DrawImage(HDC);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RAYTRACERGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYTRACERGUI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACERGUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RAYTRACERGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, HSIZE, VSIZE, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawImage(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
      switch (wParam)
      {
      case VK_ADD:
        FOV -= ZOOM_DELTA;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
      case VK_SUBTRACT:
        FOV += ZOOM_DELTA;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
      case VK_LEFT:
        MessageBox(NULL, L"Test", L"left button was pressed", NULL);
        break;
      case VK_RIGHT:
        MessageBox(NULL, L"Test", L"right button was pressed", NULL);
        break;
      case VK_UP:
        MessageBox(NULL, L"Test", L"up button was pressed", NULL);
        break;
      case VK_DOWN:
        MessageBox(NULL, L"Test", L"down button was pressed", NULL);
        break;
      case VK_HOME:
        UP = rt::Transform::id().rot_z(3.14159f / 4.0f) * UP;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
      }
      break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void DrawImage(HDC hdc)
{
  rt::World w;

  auto plane_scale = rt::Transform::id().scale(10, 0.1f, 10);
  rt::Material floor_mat(rt::Color(1, 0.9f, 0.9f), 0.1f, 0.9f, 0, 0);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(plane_scale, floor_mat));
//  rt::Sphere floor(plane_scale, floor_mat);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().rot_x(1.570795f /* pi/2 */).rot_y(-0.7853975f /* -pi/4 */).translate(0, 0, 5) * plane_scale, floor_mat));
  //rt::Sphere left_wall(rt::Transform::id().rot_x(1.570795f /* pi/2 */).rot_y(-0.7853975f /* -pi/4 */).translate(0, 0, 5) * plane_scale, floor_mat);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().rot_x(1.570795f /* pi/2 */).rot_y(0.7853975f /* pi/4 */).translate(0, 0, 5) * plane_scale, floor_mat));
  //rt::Sphere right_wall(rt::Transform::id().rot_x(1.570795f /* pi/2 */).rot_y(0.7853975f /* pi/4 */).translate(0, 0, 5) * plane_scale, floor_mat);
  rt::Material middle_mat(rt::Color(0.1f, 1, 0.5f), 0.1f, 0.7f, 0.3f, 200);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().translate(-0.5f, 1, 0.5f), middle_mat));
  //rt::Sphere middle(rt::Transform::id().translate(-0.5f, 1, 0.5f), middle_mat);
  rt::Material right_mat(rt::Color(0.5f, 1, 0.1f), 0.1f, 0.7f, 0.3f, 200);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.5f, 0.5f, 0.5f).translate(1.5f, 0.5f, -0.5f), right_mat));
  //rt::Sphere right(rt::Transform::id().scale(0.5f, 0.5f, 0.5f).translate(1.5f, 0.5f, -0.5f), right_mat);
  rt::Material left_mat(rt::Color(1, 0.8f, 0.1f), 0.1f, 0.7f, 0.3f, 200);
  w.objects().emplace_back(std::make_shared<rt::Sphere>(rt::Transform::id().scale(0.33f, 0.33f, 0.33f).translate(-1.5f, 0.33f, -0.75f), left_mat));
  //rt::Sphere left(rt::Transform::id().scale(0.33f, 0.33f, 0.33f).translate(-1.5f, 0.33f, -0.75f), left_mat);

  w.lights().emplace_back(std::make_shared<rt::Light>(rt::Color(1, 1, 1), rt::Point(-10, 10, -10)));

  rt::Camera cam(HSIZE, VSIZE, FOV, rt::Transform::id().view(FROM, TO, UP));

  auto canvas = cam.render(w);

  // Note: i tried making the render loop here directly, was as slow. Probably better just to switch to opengl
  for (int x = 0; x < HSIZE; ++x) {
    for (int y = 0; y < VSIZE; ++y) {
      SetPixel(hdc, x, y,  RGB( rt::toOutputColor(canvas.pixel(x, y).x), rt::toOutputColor(canvas.pixel(x, y).y), rt::toOutputColor(canvas.pixel(x, y).z)));
    }
  }
}
