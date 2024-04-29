#ifndef SKIA_TUTORIAL_WATER_MARK_UTILS_H
#define SKIA_TUTORIAL_WATER_MARK_UTILS_H

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "skia/file.h"

class WaterMarkUtils {
private:
  template <typename T>
  static bool SplitAndParse(const std::string& str, char delim,
                     std::vector<T>* values) {
    std::istringstream input(str);
    for (std::string line; std::getline(input, line, delim);) {
      std::istringstream to_parse(line);
      T val;
      to_parse >> val;
      if (!to_parse.eof() && !to_parse.good()) {
        return false;
      }
      values->emplace_back(val);
    }
    return true;
  }
  static double MeasureTextWidth(std::string& temp_str);
public:
    static std::vector<std::string> ParseWaterMarkContent(std::string& content);
    static std::vector<std::string> load_system_fonts();
};

#endif  // SKIA_TUTORIAL_WATER_MARK_UTILS_H
