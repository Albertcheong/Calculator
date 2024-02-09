#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// define custom WINDOWS MESSAGES
#define WM_APP_INITIALIZE (WM_APP + 1)

// define macros
#define BUTTON_SPACING    10
#define MAX_CHAR_DISPLAY  34

#define BUTTON_CE          1
#define BUTTON_C           2
#define BUTTON_BACKSPACE   3
#define BUTTON_DIV         4
#define BUTTON_SEVEN       5
#define BUTTON_EIGHT       6
#define BUTTON_NINE        7
#define BUTTON_MUL         8
#define BUTTON_FOUR        9
#define BUTTON_FIVE       10
#define BUTTON_SIX        11
#define BUTTON_SUB        12
#define BUTTON_ONE        13
#define BUTTON_TWO        14
#define BUTTON_THREE      15
#define BUTTON_ADD        16
#define BUTTON_NEGATE     17
#define BUTTON_ZERO       18
#define BUTTON_FL_POINT   19
#define BUTTON_EQUAL      20

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
			L"0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,

			10, 10, 280, 45,

			m_hwnd,
			NULL,
			m_hInstance,
			NULL
		);

		int nButtonWidth  = 60;
		int nButtonHeight = 40;
		int nFieldWidth   =  4;
		int nFieldHeight  =  5;

		m_buttons = {
			{BUTTON_CE,        L"CE"    },
			{BUTTON_C,         L"C"     },
			{BUTTON_BACKSPACE, L"\u232B"},
			{BUTTON_DIV,       L"/"     },
			{BUTTON_SEVEN,     L"7"     },
			{BUTTON_EIGHT,     L"8"     },
			{BUTTON_NINE,      L"9"     },
			{BUTTON_MUL,       L"x"     },
			{BUTTON_FOUR,      L"4"     },
			{BUTTON_FIVE,      L"5"     },
			{BUTTON_SIX,       L"6"     },
			{BUTTON_SUB,       L"-"     },
			{BUTTON_ONE,       L"1"     },
			{BUTTON_TWO,       L"2"     },
			{BUTTON_THREE,     L"3"     },
			{BUTTON_ADD,       L"+"     },
			{BUTTON_NEGATE,    L"+/-"   },
			{BUTTON_ZERO,      L"0"     },
			{BUTTON_FL_POINT,  L"."     },
			{BUTTON_EQUAL,     L"="     }
		};
		
		for (int y = 0; y < nFieldHeight; y++)
			for (int x = 0; x < nFieldWidth; x++)
			{
				auto& buttonInfo = m_buttons[x + nFieldWidth * y];
				CreateWindowEx(
					NULL,
					L"Button",
					buttonInfo.label.c_str(),
					WS_CHILD | WS_VISIBLE | BS_CENTER,

					BUTTON_SPACING + x * (nButtonWidth + BUTTON_SPACING), 100 + y * (nButtonHeight + BUTTON_SPACING),
					nButtonWidth, nButtonHeight, m_hwnd, reinterpret_cast<HMENU>(buttonInfo.id), m_hInstance, NULL
				);
			}
	}

	void handleButtonClick(WPARAM ButtonID)
	{
		switch (ButtonID)
		{
			case BUTTON_CE:
				SetWindowText(m_hEdit, L"0");
				break;

			case BUTTON_C:
				SendMessage(m_hEdit, EM_SETSEL, 0, -1);
				SendMessage(m_hEdit, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L""));
				SetWindowText(m_hEdit, L"0");
				break;

			case BUTTON_BACKSPACE:
				{
					std::size_t size = GetWindowTextLength(m_hEdit);
					if (size > 1)
					{
						SendMessage(m_hEdit, EM_SETSEL, size - 1, size);
						SendMessage(m_hEdit, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L""));
					}
					else if (size == 1)
					{
						SetWindowText(m_hEdit, L"0");
					}
				}
				break;

			case BUTTON_DIV:
				break;

			case BUTTON_SEVEN:
				appendEdit(L'7');
				break;

			case BUTTON_EIGHT:
				appendEdit(L'8');
				break;

			case BUTTON_NINE:
				appendEdit(L'9');
				break;

			case BUTTON_MUL:
				break;

			case BUTTON_FOUR:
				appendEdit(L'4');
				break;

			case BUTTON_FIVE:
				appendEdit(L'5');
				break;

			case BUTTON_SIX:
				appendEdit(L'6');
				break;

			case BUTTON_SUB:
				break;

			case BUTTON_ONE:
				appendEdit(L'1');
				break;

			case BUTTON_TWO:
				appendEdit(L'2');
				break;

			case BUTTON_THREE:
				appendEdit(L'3');
				break;

			case BUTTON_ADD:
				break;

			case BUTTON_NEGATE:
				{
					std::size_t size = GetWindowTextLength(m_hEdit);
					if (size > 0)
					{
						std::wstring text = getEditControlText();
						if (text[0] != L'0')
						{
							if (text[0] == L'-')
							{
								text = text.substr(1);
							}
							else
							{
								text = L'-' + text;
							}
						}

						SetWindowText(m_hEdit, text.c_str());
					}
				}
				break;

			case BUTTON_ZERO:
				appendEdit(L'0');
				break;

			case BUTTON_FL_POINT:
				appendEdit(L'.');
				break;

			case BUTTON_EQUAL:
				break;
		}
	}

	void appendEdit(wchar_t buttonText)
	{
		//std::size_t size = GetWindowTextLength(m_hEdit);
		//auto buffer = std::make_unique<wchar_t[]>(size + 2);
		//GetWindowText(m_hEdit, buffer.get(), size + 1);
		//buffer[size] = buttonText;
		//buffer[size + 1] = 0;
		//SetWindowText(m_hEdit, buffer.get());

		if (GetWindowTextLength(m_hEdit) < MAX_CHAR_DISPLAY)
		{
			std::wstring newText;
			if (getEditControlText()[0] == L'0' && GetWindowTextLength(m_hEdit) == 1 && buttonText != L'.')
			{
				newText = buttonText;
			}
			else
			{
				newText = getEditControlText() + buttonText;
			}

			SetWindowText(m_hEdit, newText.c_str());
		}
	}

	std::wstring getEditControlText()
	{
		const int kBufferSize = MAX_CHAR_DISPLAY;
		wchar_t szBuffer[kBufferSize];
		GetWindowText(m_hEdit, szBuffer, kBufferSize);
		return std::wstring(szBuffer);
	}

	PCWSTR getClassName() const override { return L"Simple Calculator"; }

	private:
	HWND m_hEdit;

	struct Button
	{
		int id;
		std::wstring label;
	};
	std::vector<Button> m_buttons;
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

		case WM_COMMAND:
			pApplication->handleButtonClick(wParam);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
