/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "hotconfig.h"

using namespace std;

namespace Honey {
  using namespace tinyxml2;

  HotConfig& HotConfig::instance() {
    static HotConfig hot_config_instance;
    return hot_config_instance;
  }

  HotConfig::HotConfig() {
    path = default_path;
    update_interval = default_update_interval;

    bools = {};
    ints = {};
    floats = {};
    strings = {};
  }

  // Read from this location
  void HotConfig::setPath(string path) {
    this->path = path;
  }

  // Use this interval
  void HotConfig::setUpdateInterval(float interval) {
    update_interval = interval;
    logic.remove("hot_config_update");
    logic.makeTimeLock("hot_config_update", update_interval);
    update();
  }

  // Check the time and update
  int HotConfig::checkAndUpdate() {
    if (!logic.isTimeLocked("hot_config_update")) {
      logic.makeTimeLock("hot_config_update", update_interval);
    } else {
      return SLEEPING;
    }
    return update();
  }

  // Just update without checking
  int HotConfig::update() {
    XMLDocument doc;
    XMLError result = doc.LoadFile(path.c_str());

    if (result != XML_SUCCESS) {
      printf("HotConfig load failed entirely! Failed to load and parse file at path %s\n", path.c_str());
      return FAILURE;
    }

    unordered_map<string, unordered_map<string, bool>> new_bools = {};
    unordered_map<string, unordered_map<string, int>> new_ints = {};
    unordered_map<string, unordered_map<string, float>> new_floats = {};
    unordered_map<string, unordered_map<string, string>> new_strings = {};

    XMLElement* section = doc.FirstChildElement();
    string node_value;
    while (section != nullptr) {
      node_value = section->Value();
      if (node_value != "section") {
        printf("HotConfig load failed on line number %d. Top level nodes must have type 'section'. Found %s instead.\n", section->GetLineNum(), node_value.c_str());
        return FAILURE;
      }

      const char* section_name_chars;
      if (section->QueryStringAttribute("name", &section_name_chars) != XML_SUCCESS) {
        printf("HotConfig load failed on line number %d. Section is missing name.\n", section->GetLineNum());
        return FAILURE;
      }
      string section_name(section_name_chars);
      new_bools[section_name] = {};
      new_ints[section_name] = {};
      new_floats[section_name] = {};
      new_strings[section_name] = {};

      XMLElement* element = section->FirstChildElement();
      while (element != nullptr) {
        node_value = element->Value();
        if (node_value != "param") {
          printf("HotConfig load failed on line number %d. Nodes inside sections must have type 'param'. Found %s instead.\n", element->GetLineNum(), node_value.c_str());
          return FAILURE;
        }

        const char* element_type_chars;
        if (element->QueryStringAttribute("type", &element_type_chars) != XML_SUCCESS) {
          printf("HotConfig load failed on line number %d. Param is missing type.\n", element->GetLineNum());
          return FAILURE;
        }
        string element_type(element_type_chars);

        const char* element_name_chars;
        if (element->QueryStringAttribute("name", &element_name_chars) != XML_SUCCESS) {
          printf("HotConfig load failed on line number %d. Param is missing name.\n", element->GetLineNum());
          return FAILURE;
        }
        string element_name(element_name_chars);

        if (element_type == "bool") {
          string value(element->GetText());
          if (value != "true" && value != "false") {
            printf("HotConfig load failed on line number %d. Failed to parse bool value. Expected true or false, but got %s.\n", element->GetLineNum(), value.c_str());
            return FAILURE;
          } else if (value == "true") {
            new_bools[section_name][element_name] = true;
          } else {
            new_bools[section_name][element_name] = false;
          }
        } else if (element_type == "int") {
          int value;
          int result = element->QueryIntText(&value);
          if (result != XML_SUCCESS) {
            printf("HotConfig load failed on line number %d. Failed to parse int value.\n", element->GetLineNum());
            return FAILURE;
          }
          new_ints[section_name][element_name] = value;
        } else if (element_type == "float") {
          float value;
          int result = element->QueryFloatText(&value);
          if (result != XML_SUCCESS) {
            printf("HotConfig load failed on line number %d. Failed to parse int value.\n", element->GetLineNum());
            return FAILURE;
          }
          new_floats[section_name][element_name] = value;
        } else if (element_type == "string") {
          string value(element->GetText());
          new_strings[section_name][element_name] = value;
        } else {
          printf("HotConfig load failed on line number %d. Unknown type %s. Must be bool, int, float, or string.\n", element->GetLineNum(), element_type_chars);
          return FAILURE;
        }

        element = element->NextSiblingElement();
      }
      section = section->NextSiblingElement();
    }

    bools = new_bools;
    ints = new_ints;
    floats = new_floats;
    strings = new_strings;

    return SUCCESS;
  }

  bool HotConfig::getBool(string section, string name) {
    if (bools.count(section) == 0 || bools[section].count(name) == 0) {
      printf("WARNING: Failed to find bool %s in section %s in config.\n", name.c_str(), section.c_str());
      return false;
    }

    return bools[section][name];
  }

  int HotConfig::getInt(string section, string name) {
    if (ints.count(section) == 0 || ints[section].count(name) == 0) {
      printf("WARNING: Failed to find int %s in section %s in config.\n", name.c_str(), section.c_str());
      return 0;
    }

    return ints[section][name];
  }

  float HotConfig::getFloat(string section, string name) {
    if (floats.count(section) == 0 || floats[section].count(name) == 0) {
      printf("WARNING: Failed to find float %s in section %s in config.\n", name.c_str(), section.c_str());
      return 0;
    }

    return floats[section][name];
  }

  string HotConfig::getString(string section, string name) {
    if (strings.count(section) == 0 || strings[section].count(name) == 0) {
      printf("WARNING: Failed to find string %s in section %s in config.\n", name.c_str(), section.c_str());
      return "";
    }

    return strings[section][name];
  }

  HotConfig::~HotConfig() {
    bools.clear();
    ints.clear();
    floats.clear();
    strings.clear();
  }
}
