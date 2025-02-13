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

    window window::get_foreground() {
        return window{ ::GetForegroundWindow() };
    }

    window window::find_by_class_name(const std::string& class_name) {
        auto wcn = str::to_wstr(class_name);
        HWND hwnd = ::FindWindow(wcn.c_str(), nullptr);
        return window{hwnd};
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

    DWORD window::get_pid() const {
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
    //void window::minimize() { ::ShowWindow(hwnd, SW_SHOWMINIMIZED); }

    LONG window::get_styles() {
        return ::GetWindowLong(hwnd, GWL_STYLE);
    }

    void window::set_topmost(bool topmost) {
        ::SetWindowPos(hwnd,
            topmost ? HWND_TOPMOST : HWND_NOTOPMOST,
            0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
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