/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "screenmanager.h"

using namespace std;

namespace Honey {
  ScreenManager& ScreenManager::instance() {
    static ScreenManager screen_manager_instance;
    return screen_manager_instance;
  }

  ScreenManager::ScreenManager() {
    quit_status = false;
    current_screen = "";
  }

  void ScreenManager::addScreen(string label, unique_ptr<Screen> &screen) {
    screens[label] = std::move(screen);
  }

  void ScreenManager::deleteScreen(string label) {
    screens.erase(label);
  }

  bool ScreenManager::checkScreen(string label) {
    return screens.count(label) == 1;
  }

  void ScreenManager::setCurrentScreen(string label) {
    current_screen = label;
  }

  string ScreenManager::getCurrentScreen() {
    return current_screen;
  }

  void ScreenManager::initialize() {
    if (current_screen != "") {
      screens[current_screen]->initialize();
    }
  }

  void ScreenManager::loop() {
    if (current_screen != "") {
      screens[current_screen]->loop();
    }
  }

  void ScreenManager::setQuit() {
    quit_status = true;
  }

  bool ScreenManager::getQuit() {
    return quit_status;
  }

  ScreenManager::~ScreenManager() {
    screens.clear();
  }
}
