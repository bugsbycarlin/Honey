/*

  Honey
  Copyright 2018 - Matthew Carlin

  HotConfig class reads config from a file and can update during runtime.
*/

#ifndef HONEY_HOT_CONFIG_H_
#define HONEY_HOT_CONFIG_H_

#include <string>
#include <stdlib.h>
#include <unordered_map>

#include "tinyxml2.h"

#include "logic.h"

namespace Honey {
  class HotConfig {
   public:
      const float default_update_interval = 2;
      float update_interval;

      const std::string default_path = "config.txt";
      std::string path;

      std::unordered_map<std::string, std::unordered_map<std::string, bool>> bools = {};
      std::unordered_map<std::string, std::unordered_map<std::string, int>> ints = {};
      std::unordered_map<std::string, std::unordered_map<std::string, float>> floats = {};
      std::unordered_map<std::string, std::unordered_map<std::string, std::string>> strings = {};

      enum STATUS {
        SUCCESS,
        FAILURE,
        SLEEPING,
      };

      HotConfig();

      // Read from this location
      void setPath(std::string path);

      // Use this interval
      void setUpdateInterval(float interval);

      // Check the time and update
      int checkAndUpdate();

      // Just update without checking
      int update();

      // Get values by section
      bool getBool(std::string section, std::string name);
      int getInt(std::string section, std::string name);
      float getFloat(std::string section, std::string name);
      std::string getString(std::string section, std::string name);
  };

  extern HotConfig* hot_config;
}
#endif
