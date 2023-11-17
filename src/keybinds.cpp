#include "keybinds.hpp"

namespace tbyte {
    void initKeys(){
        arc::keybinds.registerKeybind(arc::KEYBIND::ESCAPE, arc:: INPUT_TYPE::KEYBOARD, ARC_KEY_ESC);

        arc::keybinds.registerKeybind(arc::KEYBIND::LEFT_CLICK, arc:: INPUT_TYPE::MOUSE, ARC_MOUSE_LEFT);

        arc::keybinds.registerKeybind(arc::KEYBIND::ENTER, arc:: INPUT_TYPE::KEYBOARD, ARC_KEY_SPACE);
    }
}