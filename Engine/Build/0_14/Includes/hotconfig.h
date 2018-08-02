/*

  Honey
  Copyright 2018 - Matthew Carlin

  HotConfig class reads config from a file and can update during runtime.
*/

#pragma once

#include <string>
#include <stdlib.h>
#include <unordered_map>

#include "tinyxml2.h"

#include "logic.h"

using namespace std;

namespace Honey {
  class HotConfig {
   public:
      enum STATUS {
        SUCCESS,
        FAILURE,
        SLEEPING,
      };

      HotConfig();

      // Read from this location
      void setPath(string path);

      // Use this interval
      void setUpdateInterval(float interval);

      // Check the time and update
      int checkAndUpdate();

      // Just update without checking
      int update();

      // Get values by section
      bool getBool(string section, string name);
      int getInt(string section, string name);
      float getFloat(string section, string name);
      string getString(string section, string name);

      ~HotConfig();

    private:
      const float default_update_interval = 2;
      float update_interval;

      const string default_path = "config.txt";
      string path;

      unordered_map<string, unordered_map<string, bool>> bools = {};
      unordered_map<string, unordered_map<string, int>> ints = {};
      unordered_map<string, unordered_map<string, float>> floats = {};
      unordered_map<string, unordered_map<string, string>> strings = {};
  };

  extern HotConfig* hot_config;
}
