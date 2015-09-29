#include <windows.h>


long CALLBACK WndProc(HWND hWnd, UINT Message, UINT wParam, long lParam);
char szAppName[] = "Skeleton Windows Process";


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, char* szCmd, int iCmdShow)
{
	WNDCLASS wndclass;					// class object for this window
	MSG msg;							// incoming messages are stored here
	HWND hWnd;							// handle to the window object

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);


	RegisterClass(&wndclass);			// pass Windows the wndclass object

	hWnd = CreateWindow(szAppName,		// allocate storage for the window
						"Skeleton Windows Process",
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						HWND_DESKTOP,
						NULL,
						hInstance,
						NULL);

	ShowWindow(hWnd, iCmdShow);			// display the window
	UpdateWindow(hWnd);					// tell the window to paint itself


	while(GetMessage(&msg, NULL, 0, 0))	// get message from queue
	{
		TranslateMessage(&msg);			// translate the message
		DispatchMessage(&msg);			// send message to the appropriate procedure
	}

	return(msg.wParam);
}


long CALLBACK WndProc(HWND hWnd, UINT Message, UINT wParam, long lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;

	switch(Message)
	{
		case WM_PAINT:					// paint itself
			hDC = BeginPaint(hWnd, &ps);	
			
			DrawText(hDC,
					 szAppName,
					 strlen(szAppName),
					 &(ps.rcPaint),
					 DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			EndPaint(hWnd, &ps);
			return 0;

		case WM_LBUTTONDOWN:
		case WM_DESTROY:				// destroy the process
			PostQuitMessage(0);
			return 0;
	}

	return(DefWindowProc(hWnd, Message, wParam, lParam));	// default
}