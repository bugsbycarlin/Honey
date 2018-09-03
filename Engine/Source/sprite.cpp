/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "sprite.h"

using namespace std;

namespace Honey {
  Sprite::Sprite(string label, position pos, string color, float opacity, float rotation, float scale) {
    this->label = label;
    this->pos = pos;
    this->color = color;
    this->opacity = opacity;
    this->rotation = rotation;
    this->scale = scale;
  }

  string Sprite::getLabel() {
    return label;
  }

  void Sprite::setLabel(string label) {
    this->label = label;
  }

  position Sprite::getPosition() {
    return pos;
  }

  void Sprite::setPosition(position pos) {
    this->pos = pos;
  }

  void Sprite::setPosition(position pos, float animation_duration) {
    effects.makeTween(label + "_sprite_x", this->pos.x - pos.x, 0, animation_duration);
    effects.makeTween(label + "_sprite_y", this->pos.y - pos.y, 0, animation_duration);
    effects.start(label + "_sprite_x");
    effects.start(label + "_sprite_y");
    setPosition(pos);
  }

  void Sprite::shakePosition(int shake_width, float animation_duration) {
    effects.makeShake(label + "_sprite_shake", shake_width, animation_duration);
    effects.start(label + "_sprite_shake");
  }

  string Sprite::getColor() {
    return color;
  }

  void Sprite::setColor(string color) {
    this->color = color;
  }

  void Sprite::setColor(string color, float animation_duration) {
    floatColor old_color = graphics.parseFloatColor(this->color);
    floatColor new_color = graphics.parseFloatColor(color);
    effects.makeTween(label + "_sprite_r", old_color.r, new_color.r, animation_duration);
    effects.makeTween(label + "_sprite_g", old_color.g, new_color.g, animation_duration);
    effects.makeTween(label + "_sprite_b", old_color.b, new_color.b, animation_duration);
    effects.start(label + "_sprite_r");
    effects.start(label + "_sprite_g");
    effects.start(label + "_sprite_b");
    setColor(color);
  }

  float Sprite::getOpacity() {
    return opacity;
  }

  void Sprite::setOpacity(float opacity) {
    this->opacity = opacity;
  }

  void Sprite::setOpacity(float opacity, float animation_duration) {
    effects.makeTween(label + "_sprite_opacity", this->opacity, opacity, animation_duration);
    effects.start(label + "_sprite_opacity");
    setOpacity(opacity);
  }

  void Sprite::fadeInOut(float animation_duration) {
    effects.makeTween(label + "_sprite_fade", 0, 1, animation_duration);
    effects.start(label + "_sprite_fade");
  }

  float Sprite::getRotation() {
    return rotation;
  }

  void Sprite::setRotation(float rotation) {
    this->rotation = rotation;
  }

  void Sprite::setRotation(float rotation, float animation_duration) {
    effects.makeTween(label + "_sprite_rotation", this->rotation, rotation, animation_duration);
    effects.start(label + "_sprite_rotation");
    setRotation(rotation);
  }

  float Sprite::getScale() {
    return scale;
  }

  void Sprite::setScale(float scale) {
    this->scale = scale;
  }

  void Sprite::setScale(float scale, float animation_duration) {
    effects.makeTween(label + "_sprite_scale", this->scale, scale, animation_duration);
    effects.start(label + "_sprite_scale");
    setScale(scale);
  }

  void Sprite::draw() {
    draw(this->pos);
  }

  void Sprite::draw(position pos) {
    int x = pos.x;
    int y = pos.y;
    if (effects.busy(label + "_sprite_x")) {
      x += effects.tween(label + "_sprite_x", effects.SIGMOID);
      y += effects.tween(label + "_sprite_y", effects.SIGMOID);
    }
    if (effects.busy(label + "_sprite_shake")) {
      x += effects.shake(label + "_sprite_shake");
      y += effects.shake(label + "_sprite_shake");
    }

    float op = opacity;
    if (effects.busy(label + "_sprite_opacity")) {
      op = effects.tween(label + "_sprite_opacity", effects.LINEAR);
    } else if (effects.busy(label + "_sprite_fade")) {
      op = effects.tween(label + "_sprite_fade", effects.SINEWAVE);
    }

    if (effects.busy(label + "_sprite_r")) {
      float r = effects.tween(label + "_sprite_r", effects.SIGMOID);
      float g = effects.tween(label + "_sprite_g", effects.SIGMOID);
      float b = effects.tween(label + "_sprite_b", effects.SIGMOID);
      graphics.setColor(r, g, b, op);
    } else {
      graphics.setColor(color, op);
    }

    float ro = rotation;
    if (effects.busy(label + "_sprite_rotation")) {
      ro = effects.tween(label + "_sprite_rotation", effects.LINEAR);
    }

    float sc = scale;
    if (effects.busy(label + "_sprite_scale")) {
      sc = effects.tween(label + "_sprite_scale", effects.SIGMOID);
    }

    //graphics.drawImage(label, x, y, true, ro, sc);
    graphics.drawImage(label, x, y, true, ro, sc);
  }

  Sprite::~Sprite() {
  }
}
