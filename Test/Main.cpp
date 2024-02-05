#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <memory>
#include <thread>

// define custom WINDOWS MESSAGES
#define WM_APP_INITIALIZE (WM_APP + 1)

// define macros
#define BORDER_OFFSET     10

LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);

class BaseWindow
{
	public:
	BaseWindow() : m_hInstance(GetModuleHandle(nullptr)), m_hwnd(nullptr), m_clientArea{} {}
	~BaseWindow() {}

	void createBaseWindow(int nWindowWidth, int nWindowHeight)
	{
		WNDCLASSEX wcex{};
		wcex.cbSize        = sizeof(WNDCLASSEX);
		wcex.style         = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc   = windowProc;
		wcex.cbClsExtra    = NULL;
		wcex.cbWndExtra    = NULL;
		wcex.hInstance     = m_hInstance;
		wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		wcex.lpszMenuName  = NULL;
		wcex.lpszClassName = getClassName();
		wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&wcex);

		m_clientArea = { 0, 0, nWindowWidth, nWindowHeight };
		AdjustWindowRectEx(&m_clientArea, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

		m_hwnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,
			getClassName(),
			L"Calculator",
			WS_OVERLAPPEDWINDOW,

			CW_USEDEFAULT, CW_USEDEFAULT, m_clientArea.right - m_clientArea.left, m_clientArea.bottom - m_clientArea.top,

			NULL,
			NULL,
			m_hInstance,
			NULL
		);
	}

	HWND getHwnd() const { return m_hwnd; }

	protected:
	HWND      m_hwnd;
	HINSTANCE m_hInstance;
	RECT      m_clientArea;

	virtual PCWSTR getClassName() const = 0;
};

class Calculator : public BaseWindow
{
	public:
	Calculator() : m_hEdit(nullptr) {}
	~Calculator() {}

	void init()
	{
		m_hEdit = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"Edit",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,

			10, 10, 280, 45,

			m_hwnd,
			NULL,
			m_hInstance,
			NULL
		);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int nWidth = 80, nHeight = 60;
				HWND hButton = CreateWindowEx(
					NULL,
					L"Button",
					L"",
					WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CENTER | BS_TEXT,

					BORDER_OFFSET + j * (BORDER_OFFSET + nWidth), 80 + i * (BORDER_OFFSET + nHeight), nWidth, nHeight,

					m_hwnd,
					NULL,
					m_hInstance,
					NULL
				);

				wchar_t buttonText[10];
				swprintf_s(buttonText, L"%d", j + i * 3 + 1);
				SetWindowText(hButton, buttonText);
			}
		}
	}

	PCWSTR getClassName() const override { return L"Simple Calculator"; }

	private:
	HWND m_hEdit;
};

// global variables
std::unique_ptr<Calculator> pApplication;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_     LPWSTR pCmdline, 
	_In_     int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdline);

	pApplication = std::make_unique<Calculator>();
	pApplication->createBaseWindow(300, 400);

	ShowWindow(pApplication->getHwnd(), nCmdShow);
	UpdateWindow(pApplication->getHwnd());

	MSG msg{};
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
			PostMessage(hwnd, WM_APP_INITIALIZE, NULL, NULL);
			break;

		case WM_APP_INITIALIZE:
			pApplication->init();
			break;

		case BN_CLICKED:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
