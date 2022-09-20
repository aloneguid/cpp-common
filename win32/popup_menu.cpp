#include "popup_menu.h"
#include "../str.h"

namespace win32 {
    popup_menu::popup_menu(HWND h_wnd_owner) : h_wnd_owner{ h_wnd_owner } {
        h_menu = ::CreatePopupMenu();
    }

    popup_menu::~popup_menu() {
        ::DestroyMenu(h_menu);
    }

    void popup_menu::add(const std::string& id, const std::string& title) {
        auto loword_wparam = next_id++;
        loword_wparam_to_id[loword_wparam] = id;
        ::InsertMenu(h_menu, -1, MF_BYPOSITION, loword_wparam, str::to_wstr(title).c_str());
        count += 1;
    }

    void popup_menu::separator() {
        ::InsertMenu(h_menu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
    }

    void popup_menu::clear() {
        if (count == 0) return;

        ::DestroyMenu(h_menu);
        h_menu = ::CreatePopupMenu();
        count = 0;
        next_id = 0;
        loword_wparam_to_id.clear();
    }

    std::string popup_menu::id_from_loword_wparam(int loword_wparam) {
        return loword_wparam_to_id[loword_wparam];
    }

    void popup_menu::show() {

        if (count == 0) return;

        ::SetForegroundWindow(h_wnd_owner);

        POINT pt;
        ::GetCursorPos(&pt);
        ::TrackPopupMenu(h_menu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, h_wnd_owner, NULL);
    }
}