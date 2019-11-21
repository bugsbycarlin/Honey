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
  Config& config = Config::instance();
  Config& conf = config;
  Config& hot_config = config;
  Input& input = Input::instance();
  Collisions& collisions = Collisions::instance();
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
    if (config.checkAndUpdate() != config.SUCCESS) {
      exit(1);
    }

    int screen_width = config.getInt("layout", "screen_width");
    int screen_height = config.getInt("layout", "screen_height");
    bool full_screen = config.getBool("layout", "full_screen");

    StartHoney(title, screen_width, screen_height, full_screen);
  }
}