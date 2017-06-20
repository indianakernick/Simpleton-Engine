//
//  input manager.cpp
//  Game Engine
//
//  Created by Indi Kernick on 3/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "input manager.hpp"

using namespace Platform;
using namespace Input;

InputManager::InputManager(std::weak_ptr<WindowManager> windowManager)
  : weakWindowManager(windowManager) {}

void InputManager::update() {
  WindowManager::Ptr windowManager = weakWindowManager.lock();
  if (windowManager == nullptr) {
    throw std::logic_error("Window manager destroyed before input manager destroyed");
  }

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_MOUSEBUTTONDOWN:
        sendMouseDown(event.button, windowManager);
        break;
      case SDL_MOUSEBUTTONUP:
        sendMouseUp(event.button, windowManager);
        break;
      case SDL_MOUSEMOTION:
        sendMouseMove(event.motion, windowManager);
        break;
      case SDL_MOUSEWHEEL:
        sendScroll(event.wheel, windowManager);
        break;
      case SDL_KEYDOWN:
        sendKeyDown(event.key, windowManager);
        break;
      case SDL_KEYUP:
        sendKeyUp(event.key, windowManager);
        break;
      case SDL_WINDOWEVENT:
        sendWindowEvent(event.window, windowManager);
        break;
      case SDL_QUIT:
        sendQuit(windowManager);
        break;
    }
  }
}

Key::Type InputManager::fromScancode(int scancode) {
  using namespace Key;
  if (scancode >= SDL_SCANCODE_A && scancode <= SDL_SCANCODE_Z) {
    return scancode - SDL_SCANCODE_A + A;
  } else if (scancode >= SDL_SCANCODE_0 && scancode <= SDL_SCANCODE_9) {
    return scancode - SDL_SCANCODE_0 + NUM_0;
  } else if (scancode >= SDL_SCANCODE_F1 && scancode <= SDL_SCANCODE_F12) {
    return scancode - SDL_SCANCODE_F1 + F1;
  } else if (scancode >= SDL_SCANCODE_RETURN && scancode <= SDL_SCANCODE_BACKSLASH) {
    static const Key::Type KEYS[] = {
      ENTER,
      ESCAPE,
      BACKSPACE,
      TAB,
      SPACE,
      MINUS,
      EQUAL,
      LEFT_BRACKET,
      RIGHT_BRACKET,
      BACK_SLASH,
    };
    return KEYS[scancode - SDL_SCANCODE_RETURN];
  } else if (scancode >= SDL_SCANCODE_SEMICOLON && scancode <= SDL_SCANCODE_CAPSLOCK) {
    static const Key::Type KEYS[] = {
      SEMICOLON,
      QUOTE,
      BACK_QUOTE,
      COMMA,
      DOT,
      SLASH,
      CAPS_LOCK
    };
    return KEYS[scancode - SDL_SCANCODE_SEMICOLON];
  } else if (scancode >= SDL_SCANCODE_LCTRL && scancode <= SDL_SCANCODE_RGUI) {
    static const Key::Type KEYS[] = {
      CONTROL,    //left
      SHIFT,      //left
      ALT,        //left
      LEFT_META,
      CONTROL,    //right
      SHIFT,      //right
      ALT,        //right
      RIGHT_META
    };
    return KEYS[scancode - SDL_SCANCODE_LCTRL];
  } else if (scancode >= SDL_SCANCODE_RIGHT && scancode <= SDL_SCANCODE_UP) {
    static const Key::Type KEYS[] = {
      RIGHT,
      LEFT,
      DOWN,
      UP
    };
    return KEYS[scancode - SDL_SCANCODE_RIGHT];
  }
  
  return UNKNOWN;
}

MButton::Type InputManager::fromIndex(uint8_t mButtonIndex) {
  if (mButtonIndex > 3) {
    return MButton::UNKNOWN;
  } else {
    return mButtonIndex;
  }
}

void InputManager::sendMouseDown(
  const SDL_MouseButtonEvent &event,
  WindowManager::Ptr windowManager
) {
  MButton::Type button = fromIndex(event.which);
  mouseState[button] = true;

  MouseDown::Ptr mouseDown = std::make_shared<MouseDown>();
  mouseDown->window = windowManager->getWindow(event.windowID);
  mouseDown->pos = {event.x, event.y};
  mouseDown->button = button;
  mouseDown->repeat = event.clicks;
  evtMan->emit(mouseDown);
}

void InputManager::sendMouseUp(
  const SDL_MouseButtonEvent &event,
  WindowManager::Ptr windowManager
) {
  MButton::Type button = fromIndex(event.which);
  mouseState[button] = false;

  MouseUp::Ptr mouseUp = std::make_shared<MouseUp>();
  mouseUp->window = windowManager->getWindow(event.windowID);
  mouseUp->pos = {event.x, event.y};
  mouseUp->button = button;
  evtMan->emit(mouseUp);
}

void InputManager::sendMouseMove(
  const SDL_MouseMotionEvent &event,
  WindowManager::Ptr windowManager
) {
  mousePos = {event.x, event.y};

  MouseMove::Ptr mouseMove = std::make_shared<MouseMove>();
  mouseMove->window = windowManager->getWindow(event.windowID);
  mouseMove->pos = mousePos;
  mouseMove->delta = {event.xrel, event.yrel};
  evtMan->emit(mouseMove);
}

void InputManager::sendScroll(
  const SDL_MouseWheelEvent &event,
  WindowManager::Ptr windowManager
) {
  Scroll::Ptr scroll = std::make_shared<Scroll>();
  scroll->window = windowManager->getWindow(event.windowID);
  scroll->pos = mousePos;
  scroll->delta = {event.x, event.y};
  evtMan->emit(scroll);
}

void InputManager::sendKeyDown(
  const SDL_KeyboardEvent &event,
  WindowManager::Ptr windowManager
) {
  Key::Type key = fromScancode(event.keysym.scancode);
  keyState[key] = true;
  Mod::Type mod = getModifiers(keyState);
  
  KeyDown::Ptr keyDown = std::make_shared<KeyDown>();
  keyDown->window = windowManager->getWindow(event.windowID);
  keyDown->key = key;
  keyDown->modifiers = mod;
  keyDown->character = codeToChar(key, mod);
  keyDown->repeat = event.repeat;
  evtMan->emit(keyDown);
}

void InputManager::sendKeyUp(
  const SDL_KeyboardEvent &event,
  WindowManager::Ptr windowManager
) {
  Key::Type key = fromScancode(event.keysym.scancode);
  keyState[key] = true;
  
  KeyUp::Ptr keyUp = std::make_shared<KeyUp>();
  keyUp->window = windowManager->getWindow(event.windowID);
  keyUp->key = key;
  evtMan->emit(keyUp);
}

void InputManager::sendWindowEvent(
  const SDL_WindowEvent &event,
  WindowManager::Ptr windowManager
) {
  if (event.event == SDL_WINDOWEVENT_CLOSE) {
    windowManager->closeWindow(event.windowID);
  }
}

void InputManager::sendQuit(WindowManager::Ptr windowManager) {
  windowManager->closeAllWindows();
  evtMan->emit<Quit>();
}

/*
In case there's a problem

void printWindowEvent(const SDL_WindowEvent &event) {
  std::cout << "Window " << event.windowID << "  ";
  switch (event.event) {
    case SDL_WINDOWEVENT_NONE:
      std::cout << "None\n";
      break;
    case SDL_WINDOWEVENT_SHOWN:
      std::cout << "Shown\n";
      break;
    case SDL_WINDOWEVENT_HIDDEN:
      std::cout << "Hidden\n";
      break;
    case SDL_WINDOWEVENT_EXPOSED:
      std::cout << "Exposed\n";
      break;
    case SDL_WINDOWEVENT_MOVED:
      std::cout << "Moved to " << event.data1 << ", " << event.data2 << '\n';
      break;
    case SDL_WINDOWEVENT_RESIZED:
      std::cout << "Resized to " << event.data1 << "x" << event.data2 << '\n';
      break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      std::cout << "Size changed to " << event.data1 << "x" << event.data2 << '\n';
      break;
    case SDL_WINDOWEVENT_MINIMIZED:
      std::cout << "Minimized\n";
      break;
    case SDL_WINDOWEVENT_MAXIMIZED:
      std::cout << "Maximized\n";
      break;
    case SDL_WINDOWEVENT_RESTORED:
      std::cout << "Restored\n";
      break;
    case SDL_WINDOWEVENT_ENTER:
      std::cout << "Enter\n";
      break;
    case SDL_WINDOWEVENT_LEAVE:
      std::cout << "Leave\n";
      break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
      std::cout << "Focus gained\n";
      break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
      std::cout << "Focus lost\n";
      break;
    case SDL_WINDOWEVENT_CLOSE:
      std::cout << "Close\n";
      break;
    case SDL_WINDOWEVENT_TAKE_FOCUS:
      std::cout << "Take focus\n";
      break;
    case SDL_WINDOWEVENT_HIT_TEST:
      std::cout << "None\n";
      break;
    default:
      assert(false);
  }
}

void printWindowFlags(Uint32 flags) {
  #define CHECK(flag, name) \
  if (flags & SDL_WINDOW_##flag) { \
    std::cout << #name ", "; \
  }
  
  CHECK(FULLSCREEN, fullscreen)
  CHECK(OPENGL, opengl)
  CHECK(SHOWN, shown)
  CHECK(HIDDEN, hidden)
  CHECK(BORDERLESS, borderless)
  CHECK(RESIZABLE, resizable)
  CHECK(MINIMIZED, minimized)
  CHECK(MAXIMIZED, maximized)
  CHECK(INPUT_GRABBED, input grabbed)
  CHECK(INPUT_FOCUS, input focus)
  CHECK(MOUSE_FOCUS, mouse focus)
  CHECK(FULLSCREEN_DESKTOP, fullscreen desktop)
  CHECK(FOREIGN, foreign)
  CHECK(ALLOW_HIGHDPI, allow highdpi)
  CHECK(MOUSE_CAPTURE, mouse capture)
  CHECK(ALWAYS_ON_TOP, always on top)
  CHECK(SKIP_TASKBAR, skip taskbar)
  CHECK(UTILITY, utility)
  CHECK(TOOLTIP, tooltip)
  CHECK(POPUP_MENU, popup menu)
  
  #undef CHECK
  
  std::cout << '\n';
}
*/
