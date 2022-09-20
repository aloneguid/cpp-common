#include "app.h"
#include "../str.h"

using namespace std;

namespace win32 {

    LRESULT WINAPI win32::app::WndProc(
        HWND hWnd, UINT msg,
        WPARAM wParam, LPARAM lParam) {

        // get "this"
        app* app_this = (app*)::GetProp(hWnd, L"this");
        if (app_this && app_this->on_app_message) {
            LRESULT res = app_this->on_app_message(msg, wParam, lParam);

            return res == 0 ? ::DefWindowProc(hWnd, msg, wParam, lParam) : res;
        }

        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }

    app::app(const string& class_name, const string& window_title) {

        wstring w_class_name = str::to_wstr(class_name);

        wc = {
         sizeof(WNDCLASSEX),
         CS_CLASSDC,
         WndProc,
         0L,
         0L,
         GetModuleHandle(nullptr),
         nullptr,
         nullptr,
         nullptr,
         nullptr,
         w_class_name.c_str(),
         nullptr
        };

        if (!::RegisterClassEx(&wc))
            return;

        DWORD dwStyle = WS_OVERLAPPEDWINDOW;
        DWORD dwExStyle = 0;
        hwnd = ::CreateWindowEx(dwExStyle,
            wc.lpszClassName,
            str::to_wstr(window_title).c_str(),
            dwStyle,
            100, 100, 100, 50,
            nullptr, NULL, wc.hInstance,

            // pass "this" pointer to WM_CREATE. Unfortunately it's not available after WM_CREATE has fired.
            this);
        if (!hwnd) return;

        // pass "this" as window prop to retreive it later, because WndProc is a static function
        ::SetProp(hwnd, L"this", this);

        ::ShowWindow(hwnd, SW_HIDE);
    }

    app::~app() {
        ::PostQuitMessage(0);
        ::DestroyWindow(hwnd);
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    }

    void app::run() const {
        MSG msg;
        while (::GetMessage(&msg, nullptr, 0, 0)) {
            ::TranslateMessage(&msg);

            if (on_any_message) {
                on_any_message(msg);
            }

            ::DispatchMessage(&msg);
        }
    }
}