#include <string>
#include <array>

#include "honey.h"

using namespace Honey;
using namespace std;

int test_one(settings s) {
  return boost::get<int>(s["end"]);
}

int main(int argc, char* args[]) {
  printf("Honey Engine, reporting for duty!\n");

  // Load configuration
  if(hot_config->checkAndUpdate() != hot_config->SUCCESS) {
    exit(1);
  }

  settings s = {
    {"label", "star"},
    {"end", 1},
    {"duration", 1.5}
  };
  printf("Settings: %d\n", test_one(s));

  int screen_width = hot_config->getInt("layout", "screen_width");
  int screen_height = hot_config->getInt("layout", "screen_height");
  bool full_screen = hot_config->getBool("layout", "full_screen");

  window->initialize("Honey Engine", screen_width, screen_height, full_screen);
  graphics->initialize();
  sound->initialize();

  // Load images
  graphics->addImage("boss_bear", "Art/boss_bear.png");
  graphics->addImage("explorer_bear", "Art/explorer_bear.png");
  graphics->addImage("grim_bear", "Art/grim_bear.png");
  graphics->addImage("lawn_dart_bear", "Art/lawn_dart_bear.png");
  graphics->addImage("emo_bear", "Art/emo_bear.png");
  graphics->addImage("magnet_bear", "Art/magnet_bear.png");
  graphics->addImage("star", "Art/star.png");
  graphics->addImage("selector", "Art/selector.png");

  // Add music and some sound effects
  sound->addMusic("background_music", "Sound/Nothing_to_Fear.mp3");
  sound->addSound("move_left", "Sound/C_Square1.wav");
  sound->addSound("move_right", "Sound/C_Square2.wav");
  sound->addSound("choose_1", "Sound/Chant1.wav");
  sound->addSound("choose_2", "Sound/Chant2.wav");
  sound->addSound("choose_3", "Sound/Chant3.wav");
  sound->addSound("choose_4", "Sound/Chant4.wav");
  sound->addSound("choose_5", "Sound/Chant5.wav");
  sound->addSound("bob", "Sound/bob.wav");

  // Arrays of bears and bear names and bear colors
  array<string, 6> bears = {"boss_bear", "explorer_bear", "grim_bear", "lawn_dart_bear", "emo_bear", "magnet_bear"};
  array<string, 6> bear_names = {"Bossy", "Colonel", "Grimsby", "Darty", "Emo Bob", "Maggie"};
  array<string, 6> bear_colors = {
    hot_config->getString("layout", "bear_color_0"),
    hot_config->getString("layout", "bear_color_1"),
    hot_config->getString("layout", "bear_color_2"),
    hot_config->getString("layout", "bear_color_3"),
    hot_config->getString("layout", "bear_color_4"),
    hot_config->getString("layout", "bear_color_5"),
  };

  // Star field locations
  array<int, 120> star_field_x;
  array<int, 120> star_field_y;
  for (int i = 0; i < star_field_x.size(); i++) {
    star_field_x[i] = logic->randomInt(-screen_width / 2.0, 1.5 * screen_width);
    star_field_y[i] = logic->randomInt(-screen_height / 2.0, 1.5 * screen_height);
  }

  // Bear layout; we wrap around every 3 bears
  int first_bear_x = hot_config->getInt("layout", "first_bear_x");
  int first_bear_y = hot_config->getInt("layout", "first_bear_y");
  int bear_margin_x = hot_config->getInt("layout", "bear_margin_x");
  int bear_margin_y = hot_config->getInt("layout", "bear_margin_y");
  layouts->makeTileWrapLayout("bearousel", first_bear_x, first_bear_y, bear_margin_x, bear_margin_y, 3);

  // Textbox for the selected bear
  int font_x = hot_config->getInt("layout", "font_x");
  int font_y = hot_config->getInt("layout", "font_y");
  int font_size = hot_config->getInt("layout", "font_size");
  Textbox* bear_name_text = new Textbox(
    "Fonts/jennifer.ttf",
    font_size,
    bear_names[1],
    bear_colors[1], 
    font_x,
    font_y);

  // Variables to track which bear is where and which animation is which.
  int selected_bear = 1;
  int animation_direction = 0;

  // Set action keys
  input->addActionKey("select left", hot_config->getString("input", "select_left_key"));
  input->addActionKey("select right", hot_config->getString("input", "select_right_key"));
  input->addActionKey("choose", hot_config->getString("input", "choose_key"));

  string screen_color = hot_config->getString("layout", "screen_color");
  
  float animation_duration = hot_config->getFloat("animation", "animation_duration");
  float choose_duration = hot_config->getFloat("animation", "choose_duration");
  float lock_duration = hot_config->getFloat("input", "lock_duration");
  float shake_width = hot_config->getFloat("animation", "shake_width");

  float sound_volume = hot_config->getFloat("sound", "sound_volume");
  float music_volume = hot_config->getFloat("sound", "music_volume");

  sound->setSoundVolume(sound_volume);
  sound->setMusicVolume(music_volume);

  // Start playing music
  sound->playMusic("background_music", -1);

  bool quit = false;
  while (!quit) {
    input->processInput();

    // If user slides left, switch the bears, and make animations.
    if (input->actionDown("select left") && !logic->isTimeLocked("movement")) {
      sound->playSound("move_left", 1);
      logic->makeTimeLock("movement", lock_duration);
      selected_bear -= 1;
      selected_bear = (selected_bear + bears.size()) % bears.size();
      bear_name_text->setColor(bear_colors[selected_bear]);
      bear_name_text->setText(bear_names[selected_bear]);
      animation_direction = -1;

      for (int i = 0; i < bears.size(); i++) {
        position p1 = layouts->tileWrap("bearousel", (i + bears.size() - 1) % bears.size());
        position p2 = layouts->tileWrap("bearousel", i);
        effects->makeTween(bears[i] + "x", p1.x, p2.x, animation_duration);
        effects->makeTween(bears[i] + "y", p1.y, p2.y, animation_duration);
      }
    }

    // If the user slides right, switch the bears, and make animations.
    if (input->actionDown("select right") && !logic->isTimeLocked("movement")) {
      sound->playSound("move_right", 1);
      logic->makeTimeLock("movement", lock_duration);
      selected_bear += 1;
      selected_bear = selected_bear % bears.size();
      bear_name_text->setColor(bear_colors[selected_bear]);
      bear_name_text->setText(bear_names[selected_bear]);
      animation_direction = 1;

      for (int i = 0; i < bears.size(); i++) {
        position p1 = layouts->tileWrap("bearousel", (i + 1) % bears.size());
        position p2 = layouts->tileWrap("bearousel", i);
        effects->makeTween(bears[i] + "x", p1.x, p2.x, animation_duration);
        effects->makeTween(bears[i] + "y", p1.y, p2.y, animation_duration);
      }
    }

    // If the users presses the choose button, shake the middle bear.
    if (input->actionDown("choose") && !logic->isTimeLocked("movement")) {
      if (selected_bear != 4) {
        sound->playSound("choose_" + to_string(logic->randomInt(1,5)), 1);
      } else {
        sound->playSound("bob", 1);
      }
      logic->makeTimeLock("movement", lock_duration);
      effects->makeShake("shakey shakey", shake_width, choose_duration);
      effects->makeTween("star_phasing", 0, 1, animation_duration);
      animation_direction = 0;
    }

    if (input->keyPressed("quit") > 0) {
      quit = true;
    }

    if (input->threeQuickKey("escape")) {
      quit = true;
    }

    // Clear the screen to a soft white color
    graphics->clearScreen(screen_color);

    // Switch to 2D drawing mode
    graphics->draw2D();

    // Light up the sky
    if (effects->tween("star_phasing", 0) != 0 && logic->timeSince("star_phasing") < animation_duration) {
      float opacity = effects->tween("star_phasing", effects->SINEWAVE);
      float shooting_star = 300 * effects->tween("star_phasing", effects->LINEAR);
      graphics->setColor(bear_colors[selected_bear], opacity);

      if (selected_bear != 4) {
        for (int i = 0; i < star_field_x.size(); i++) {
          graphics->drawImage("star", star_field_x[i] + shooting_star, star_field_y[i] - shooting_star, true, 0, 0.15);
        }
      } else {
        for (int i = 0; i < star_field_x.size(); i++) {
          graphics->drawImage("star", star_field_x[i], star_field_y[i] + shooting_star, true, 0, 0.15);
        }
      }

      graphics->setColor(1, 1, 1, 1);
    }

    // Draw the bears!
    for (int i = 0; i < bears.size(); i++) {

      // Shift the chosen bears by the carousel selection value
      int bear_num = (selected_bear - 1 + i + bears.size()) % bears.size();
      position p = layouts->tileWrap("bearousel", i);
      if (logic->isTimeLocked("movement")) {
        if (animation_direction != 0) {
          p.x = effects->tween(bears[i] + "x", effects->SIGMOID);
          p.y = effects->tween(bears[i] + "y", effects->SIGMOID);
        } else if (bear_num == selected_bear) {
          p.x += effects->shake("shakey shakey");
          p.y += effects->shake("shakey shakey");
        }
      }
      graphics->drawImage(bears[bear_num], p.x, p.y);
    }

    // Draw the selector
    graphics->setColor(bear_colors[selected_bear], 1);
    position p = layouts->tileWrap("bearousel", 1);
    graphics->drawImage("selector", p.x, p.y);
    graphics->setColor("#FFFFFF", 1);

    // Draw the text box
    bear_name_text->draw();

    // Put everything we've drawn on screen
    graphics->updateDisplay();
  }

  delete bear_name_text;
  QuitHoney();
}