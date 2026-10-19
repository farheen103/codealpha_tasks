
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ID_NUM1   101
#define ID_NUM2   102
#define ID_ADD    201
#define ID_SUB    202
#define ID_MUL    203
#define ID_DIV    204
#define ID_CLEAR  205
#define ID_EXIT   206
#define ID_RESULT 301

HWND hNum1, hNum2, hResult;
HFONT hFont, hTitleFont, hResultFont;

HBRUSH hBackground, hWhite, hResultBrush;

COLORREF purple = RGB(100, 65, 165);
COLORREF lavender = RGB(242, 237, 255);
COLORREF darkText = RGB(55, 43, 80);
COLORREF pink = RGB(239, 126, 177);
COLORREF blue = RGB(105, 155, 230);
COLORREF green = RGB(65, 160, 120);
COLORREF white = RGB(255, 255, 255);

void DrawButton(const DRAWITEMSTRUCT *dis)
{
    HDC dc = dis->hDC;
    RECT r = dis->rcItem;
    int id = (int)dis->CtlID;
    COLORREF color = purple;

    if (id == ID_ADD) color = purple;
    if (id == ID_SUB) color = pink;
    if (id == ID_MUL) color = blue;
    if (id == ID_DIV) color = green;
    if (id == ID_CLEAR) color = RGB(220, 210, 245);
    if (id == ID_EXIT) color = RGB(90, 75, 110);

    if (dis->itemState & ODS_SELECTED)
        color = RGB(
            GetRValue(color) * 8 / 10,
            GetGValue(color) * 8 / 10,
            GetBValue(color) * 8 / 10
        );

    HBRUSH brush = CreateSolidBrush(color);
    FillRect(dc, &r, brush);
    DeleteObject(brush);

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc,
        (id == ID_CLEAR) ? purple : white);

    HFONT font = hFont;
    HFONT oldFont = (HFONT)SelectObject(dc, font);

    char text[100];
    GetWindowTextA(dis->hwndItem, text, sizeof(text));

    DrawTextA(dc, text, -1, &r,
              DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(dc, oldFont);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
                            WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            hBackground = CreateSolidBrush(lavender);
            hWhite = CreateSolidBrush(white);
            hResultBrush = CreateSolidBrush(RGB(230, 220, 255));

             hFont = CreateFontA(
                19, 0, 0, 0, FW_MEDIUM,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH, "Segoe UI"
            );

             hTitleFont = CreateFontA(
                30, 0, 0, 0, FW_BOLD,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH, "Segoe UI"
            );

             hResultFont = CreateFontA(
                25, 0, 0, 0, FW_BOLD,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH, "Segoe UI"
            );

            HWND title = CreateWindowA(
                "STATIC", "Smart Calculator",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                25, 20, 450, 42,
                hwnd, NULL, NULL, NULL
            );

            HWND subtitle = CreateWindowA(
                "STATIC", "Your everyday math companion",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                25, 62, 450, 28,
                hwnd, NULL, NULL, NULL
            );

            CreateWindowA(
                "STATIC", "FIRST NUMBER",
                WS_VISIBLE | WS_CHILD,
                40, 115, 160, 28,
                hwnd, NULL, NULL, NULL
            );

            hNum1 = CreateWindowA(
                "EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER |
                ES_AUTOHSCROLL,
                40, 143, 430, 38,
                hwnd, (HMENU)ID_NUM1, NULL, NULL
            );

            CreateWindowA(
                "STATIC", "SECOND NUMBER",
                WS_VISIBLE | WS_CHILD,
                40, 195, 180, 28,
                hwnd, NULL, NULL, NULL
            );

            hNum2 = CreateWindowA(
                "EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER |
                ES_AUTOHSCROLL,
                40, 223, 430, 38,
                hwnd, (HMENU)ID_NUM2, NULL, NULL
            );

            CreateWindowA(
                "BUTTON", "Addition  (+)",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                40, 285, 205, 48,
                hwnd, (HMENU)ID_ADD, NULL, NULL
            );

            CreateWindowA(
                "BUTTON", "Subtraction  (-)",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                265, 285, 205, 48,
                hwnd, (HMENU)ID_SUB, NULL, NULL
            );

            CreateWindowA(
                "BUTTON", "Multiplication  (*)",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                40, 345, 205, 48,
                hwnd, (HMENU)ID_MUL, NULL, NULL
            );

            CreateWindowA(
                "BUTTON", "Division  (/)",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                265, 345, 205, 48,
                hwnd, (HMENU)ID_DIV, NULL, NULL
            );

            hResult = CreateWindowA(
                "STATIC", "RESULT: 0",
                WS_VISIBLE | WS_CHILD | SS_CENTER |
                SS_CENTERIMAGE,
                40, 415, 430, 55,
                hwnd, (HMENU)ID_RESULT, NULL, NULL
            );

            CreateWindowA(
                "BUTTON", "Clear",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                105, 490, 135, 42,
                hwnd, (HMENU)ID_CLEAR, NULL, NULL
            );

            CreateWindowA(
                "BUTTON", "Exit",
                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                265, 490, 135, 42,
                hwnd, (HMENU)ID_EXIT, NULL, NULL
            );

            SendMessageA(title, WM_SETFONT,
                         (WPARAM)hTitleFont, TRUE);
            SendMessageA(subtitle, WM_SETFONT,
                         (WPARAM)hFont, TRUE);
            SendMessageA(hNum1, WM_SETFONT,
                         (WPARAM)hFont, TRUE);
            SendMessageA(hNum2, WM_SETFONT,
                         (WPARAM)hFont, TRUE);
            SendMessageA(hResult, WM_SETFONT,
                         (WPARAM)hResultFont, TRUE);

            for (HWND child = GetWindow(hwnd, GW_CHILD);
                 child != NULL;
                 child = GetWindow(child, GW_HWNDNEXT))
            {
                if (child != title && child != subtitle &&
                    child != hNum1 && child != hNum2 &&
                    child != hResult)
                {
                    SendMessageA(child, WM_SETFONT,
                                 (WPARAM)hFont, TRUE);
                }
            }

            return 0;
        }

        case WM_CTLCOLORSTATIC:
        {
            HDC dc = (HDC)wParam;
            HWND control = (HWND)lParam;

            SetBkMode(dc, TRANSPARENT);

            if (control == hResult)
            {
                SetTextColor(dc, purple);
                SetBkMode(dc, OPAQUE);
                SetBkColor(dc, RGB(230, 220, 255));
                return (LRESULT)hResultBrush;
            }

            SetTextColor(dc, darkText);
            return (LRESULT)hBackground;
        }

        case WM_CTLCOLOREDIT:
        {
            HDC dc = (HDC)wParam;
            SetTextColor(dc, darkText);
            SetBkColor(dc, white);
            return (LRESULT)hWhite;
        }

        case WM_DRAWITEM:
            DrawButton((DRAWITEMSTRUCT *)lParam);
            return TRUE;

        case WM_COMMAND:
        {
            int id = LOWORD(wParam);

            if (id == ID_EXIT)
            {
                DestroyWindow(hwnd);
                return 0;
            }

            if (id == ID_CLEAR)
            {
                SetWindowTextA(hNum1, "");
                SetWindowTextA(hNum2, "");
                SetWindowTextA(hResult, "RESULT: 0");
                SetFocus(hNum1);
                return 0;
            }

            if (id == ID_ADD || id == ID_SUB ||
                id == ID_MUL || id == ID_DIV)
            {
                char text1[128], text2[128], output[256];
                char *end1, *end2;
                double a, b, result = 0;

                GetWindowTextA(hNum1, text1, sizeof(text1));
                GetWindowTextA(hNum2, text2, sizeof(text2));

                a = strtod(text1, &end1);
                b = strtod(text2, &end2);

                while (*end1 == ' ' || *end1 == '\t')
                    end1++;
                while (*end2 == ' ' || *end2 == '\t')
                    end2++;

                if (text1[0] == '\0' || *end1 != '\0' ||
                    text2[0] == '\0' || *end2 != '\0')
                {
                    MessageBoxA(hwnd,
                        "Enter valid numbers in both fields.",
                        "Invalid Input",
                        MB_OK | MB_ICONWARNING);
                    return 0;
                }

                if (!isfinite(a) || !isfinite(b))
                {
                    MessageBoxA(hwnd,
                        "Please enter finite numbers.",
                        "Invalid Input",
                        MB_OK | MB_ICONWARNING);
                    return 0;
                }

                switch (id)
                {
                    case ID_ADD: result = a + b; break;
                    case ID_SUB: result = a - b; break;
                    case ID_MUL: result = a * b; break;

                    case ID_DIV:
                        if (b == 0.0)
                        {
                            MessageBoxA(hwnd,
                                "Division by zero is not allowed!",
                                "Calculation Error",
                                MB_OK | MB_ICONERROR);
                            return 0;
                        }
                        result = a / b;
                        break;
                }

                if (!isfinite(result))
                {
                    MessageBoxA(hwnd,
                        "Result is outside the supported range.",
                        "Calculation Error",
                        MB_OK | MB_ICONERROR);
                    return 0;
                }

                snprintf(output, sizeof(output),
                         "RESULT: %.10g", result);

                SetWindowTextA(hResult, output);
                InvalidateRect(hResult, NULL, TRUE);
                UpdateWindow(hResult);

                return 0;
            }

            break;
        }

        case WM_DESTROY:
            if (hFont) DeleteObject(hFont);
            if (hTitleFont) DeleteObject(hTitleFont);
            if (hResultFont) DeleteObject(hResultFont);
            if (hBackground) DeleteObject(hBackground);
            if (hWhite) DeleteObject(hWhite);
            if (hResultBrush) DeleteObject(hResultBrush);

            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    const char CLASS_NAME[] = "SmartCalculatorWindow";

    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassA(&wc))
        return 1;

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Smart Calculator",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
        WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 520, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd)
        return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}
