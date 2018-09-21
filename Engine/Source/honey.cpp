/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "honey.h"

using namespace std;

namespace Honey {
  Window& window = Window::instance();
  ScreenManager& screenmanager = ScreenManager::instance();
  Timing& timing = Timing::instance();
  MathUtilities& math_utils = MathUtilities::instance();
  HotConfig& hot_config = HotConfig::instance();
  Input& input = Input::instance();
  Effects& effects = Effects::instance();
  Layouts& layouts = Layouts::instance();
  Graphics& graphics = Graphics::instance();
  Sound& sound = Sound::instance();

  void StartHoney(string title, int screen_width, int screen_height, bool full_screen) {
    window.initialize(title, screen_width, screen_height, full_screen);
    graphics.initialize();
    sound.initialize();
  }

  void StartHoney(string title) {
    // Load configuration
    if (hot_config.checkAndUpdate() != hot_config.SUCCESS) {
      exit(1);
    }

    int screen_width = hot_config.getInt("layout", "screen_width");
    int screen_height = hot_config.getInt("layout", "screen_height");
    bool full_screen = hot_config.getBool("layout", "full_screen");

    StartHoney(title, screen_width, screen_height, full_screen);
  }
}