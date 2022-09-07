#include "window.h"
#include "../str.h"

#define MAX_LOADSTRING 1024

using namespace std;

namespace win32 {

    BOOL CALLBACK EnumerateWindowsProc(HWND hwnd, LPARAM lParam) {

        std::vector<window>* container = (std::vector<window>*)lParam;

        container->emplace_back(hwnd);

        return TRUE;
    }

    std::vector<window> window::enumerate() {
        std::vector<window> r;

        ::EnumWindows(EnumerateWindowsProc, (LPARAM)&r);

        return r;
    }

    bool window::is_valid() {
        return ::IsWindow(hwnd);
    }

    std::string window::get_text() {

        WCHAR title[MAX_LOADSTRING];
        ::GetWindowText(hwnd, title, MAX_LOADSTRING);
        return str::to_str(wstring(title));
    }

    bool window::is_visible() {
        return ::IsWindowVisible(hwnd);
    }

    std::shared_ptr<window> window::get_owner() {
        HWND owner = ::GetWindow(hwnd, GW_OWNER);

        if (!owner) return std::shared_ptr<window>();

        return std::make_shared<window>(owner);
    }

    DWORD window::get_pid() {
        DWORD pid;
        if (::GetWindowThreadProcessId(hwnd, &pid)) return pid;
        
        return 0;
    }

    std::string window::get_class_name() {
        WCHAR name[MAX_LOADSTRING];
        ::RealGetWindowClass(hwnd, name, MAX_LOADSTRING);
        return str::to_str(wstring(name));
    }

    void window::restore() { ::ShowWindow(hwnd, SW_RESTORE); }

    void window::minimize() { ::ShowWindow(hwnd, SW_MINIMIZE); }

    LONG window::get_styles() {
        return ::GetWindowLong(hwnd, GWL_STYLE);
    }

    bool window::is_child() {
        LONG styles = get_styles();
        return styles & WS_CHILDWINDOW;
    }

    bool window::is_minimized() {
        LONG styles = get_styles();
        return styles & WS_MINIMIZE;
    }
}