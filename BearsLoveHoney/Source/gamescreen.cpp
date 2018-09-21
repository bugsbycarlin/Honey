/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include "gamescreen.h"

using namespace std;
using namespace Honey;

GameScreen::GameScreen(string left_bear, string right_bear, string middle_bear, string player_color) {
  this->left_bear_string = left_bear;
  this->right_bear_string = right_bear;
  this->middle_bear_string = middle_bear;
  this->player_color = player_color;
}

void GameScreen::loop() {
  logic();
  render();
}

void GameScreen::initialize() {
  initializeLogic();
  initializeAssets();
}

void GameScreen::initializeAssets() {

  // Load images
  graphics.addImages( "Art/", {
    "honey",
  });

  left_bear = new Sprite(
    left_bear_string,
    (position) {
      80,
      300
    },
    "#ffffff", 1.0, 0.0, 1.0
  );

  right_bear = new Sprite(
    right_bear_string,
    (position) {
      880,
      300
    },
    "#ffffff", 1.0, 0.0, 1.0
  );

  middle_bear = new Sprite(
    middle_bear_string,
    (position) {
      480,
      300
    },
    "#ffffff", 1.0, 0.0, 1.0
  );

  honeycomb = new Sprite(
    "honey",
    (position) {
      480,
      400
    },
    "#ffffff", 1.0, 0.0, 0.3
  );

  player_score_box = new Textbox(
    "Fonts/jennifer.ttf",
    hot_config.getInt("layout", "font_size"),
    "You: 0",
    (position) {430, 50},
    player_color
  );

  computer_score_box = new Textbox(
    "Fonts/jennifer.ttf",
    hot_config.getInt("layout", "font_size"),
    "Dem: 0",
    (position) {430, 520},
    "#000000"
  );

  // Make star sprite
  star_sprite = new Sprite(
    "star",
    origin,
    "#ffffff", 0.0, 0.0, 0.15
  );
}

void GameScreen::initializeLogic() {
  player_score = 0;
  computer_score = 0;

  last_busy = false;

  timing.mark("start_time");
  effects.makeOscillation("left_bear_movement", 250, 4);
  effects.start("left_bear_movement");
  effects.makeOscillation("right_bear_movement", 250, 3);
  effects.start("right_bear_movement");

  // Star field locations
  int screen_width = hot_config.getInt("layout", "screen_width");
  int screen_height = hot_config.getInt("layout", "screen_height");
  for (int i = 0; i < star_field.size(); i++) {
    position p {
      .x = math_utils.randomInt(screen_width / 4.0, screen_width),
      .y = math_utils.randomInt(screen_height / 4.0, screen_height)
    };
    star_field[i] = p;
  }
}

void GameScreen::logic() {
  input.processInput();

  if (input.keyPressed("quit") > 0) {
    screenmanager.setQuit();
  }

  if (input.threeQuickKey("escape")) {
    screenmanager.setQuit();
  }

  if (input.keyDown("up")) {
    middle_bear_velocity -= 0.35;
  }

  if (input.keyDown("down")) {
    middle_bear_velocity += 0.35;
  }

  position p = middle_bear->getPosition();
  middle_bear->setPosition((position) {p.x, (int) (p.y + middle_bear_velocity)});

  middle_bear_velocity *= 0.96;

  left_bear->setPosition((position) {80, (int) (300 - effects.oscillation("left_bear_movement"))});
  right_bear->setPosition((position) {880, (int) (300 + effects.oscillation("right_bear_movement"))});

  if (!timing.locked("next_honey")) {
    honeycomb->setOpacity(1.0);
    honeycomb->setPosition(left_bear->getPosition());
    effects.makeTween("throw_honey", 0, 1, 0.7);
    effects.start("throw_honey");

    float next_honey = math_utils.randomInt(4,10) / 4.0;
    timing.lock("next_honey", next_honey);
  }

  position honey_pos = honeycomb->getPosition();
  if (!effects.busy("throw_honey")) {
    if (last_busy && honeycomb->getPosition().x > 800) {
      last_busy = false;
      computer_score += 1;
      computer_score_box->setText("Dem: " + to_string(computer_score));
    }
    star_sprite->setPosition(origin);
    honeycomb->setOpacity(0.0);
  } else {
    position p1 = left_bear->getPosition();
    position p2 = right_bear->getPosition();
    float blend = effects.tween("throw_honey", effects.RAMPDOWN);
    position p3 = {(int) (blend * p2.x + (1 - blend) * p1.x), (int) (blend * p2.y + (1 - blend) * p1.y)};
    honeycomb->setPosition(p3);

    if(math_utils.distance(p3, middle_bear->getPosition()) < 60) {
      effects.stop("throw_honey");
      player_score += 1;
      player_score_box->setText("You: " + to_string(player_score));

      position star_travel;
      if (middle_bear_string != "emo_bear") {
        sound.playSound("choose_" + to_string(math_utils.randomInt(1, 5)), 1);
        star_travel.x = 300;
        star_travel.y = 0;
      } else {
        sound.playSound("bob", 1);
        star_travel.x = 0;
        star_travel.y = 300;
      }

      // Add star field animations
      star_sprite->setPosition(star_travel, 0.5);
      star_sprite->fadeInOut(0.5);
      star_sprite->setColor(player_color);
    }

    last_busy = true;
  }
}

void GameScreen::render() {
  // Clear the screen to a soft white color
  graphics.clearScreen(hot_config.getString("layout", "screen_color"));

  // Switch to 2D drawing mode
  graphics.draw2D();

  player_score_box->draw();
  computer_score_box->draw();

  for (int i = 0; i < star_field.size(); i++) {
    star_sprite->draw(star_field[i]);
  }

  left_bear->draw();
  right_bear->draw();
  middle_bear->draw();
  honeycomb->draw();

  // Put everything we've drawn on screen
  graphics.updateDisplay();
}

GameScreen::~GameScreen() {
  delete player_score_box;
  delete computer_score_box;
  delete left_bear;
  delete right_bear;
  delete middle_bear;
  delete honeycomb;
  delete star_sprite;
}