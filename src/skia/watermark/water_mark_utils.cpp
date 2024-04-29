#include "water_mark_utils.h"

#include <iostream>

#include "include/core/SkFont.h"
#include "src/core/SkOSFile.h"

double WaterMarkUtils::MeasureTextWidth(std::string &temp_str) {
  // 创建字体对象
  SkFont font(nullptr, 30.0f);
  // 抗锯齿
  font.setEdging(SkFont::Edging::kAntiAlias);
  std::string text = temp_str;
  // 文本长度
  size_t length = strlen(text.c_str());
  // 文字宽度
  float textWidth =
      font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
  return textWidth;
}

std::vector<std::string>
WaterMarkUtils::ParseWaterMarkContent(std::string &content) {
  std::vector<std::string> results;
  std::list<std::string> handle_content_list;
  std::string origin_content =
      std::regex_replace(content, std::regex(" "), "\u00a0");
  std::cout << "print origin_content text: " << origin_content << std::endl;
  std::string regex_replace_result =
      std::regex_replace(origin_content, std::regex("\u00a0"), " ");
  std::cout << "print regex_replace origin_content text: "
            << regex_replace_result << std::endl;
  const bool split = SplitAndParse(regex_replace_result, ' ', &results);
  if (split) {
    for (const auto &result : results) {
      std::cout << "print result text: " << result << std::endl;
      handle_content_list.push_back(result);
    }
  }
  std::vector<std::string> labels;
  std::string temp_single_line_content_str =
      !handle_content_list.empty() ? handle_content_list.front() : "";
  if (handle_content_list.empty()) {
    labels.push_back(temp_single_line_content_str);
  } else {
    handle_content_list.pop_front();
  }
  const int DEFAULT_LABEL_LENGTH = 300;
  while (!handle_content_list.empty()) {
    std::string temp_str =
        temp_single_line_content_str + " " + handle_content_list.front();
    if (MeasureTextWidth(temp_str) <= DEFAULT_LABEL_LENGTH) {
      temp_single_line_content_str += " " + handle_content_list.front();
      handle_content_list.pop_front();
      if (handle_content_list.empty()) {
        labels.push_back(temp_single_line_content_str);
      }
    } else {
      labels.push_back(temp_single_line_content_str);
      if (handle_content_list.size() == 1) {
        labels.push_back(handle_content_list.front());
        handle_content_list.pop_front();
        break;
      } else {
        temp_single_line_content_str = handle_content_list.front();
        handle_content_list.pop_front();
      }
    }
  }

  std::reverse(labels.begin(), labels.end());

  // Print the labels
  for (const auto &item : labels) {
    std::cout << item << std::endl;
  }

  return labels;
}

std::vector<std::string> WaterMarkUtils::load_system_fonts() {
#define SK_FONT_FILE_PREFIX "resources"
  std::vector<std::string> fonts;
  SkOSFile::Iter iter(SK_FONT_FILE_PREFIX, ".ttf");
  SkString name;
  while (iter.next(&name, false)) {
    std::cout << name.c_str() << std::endl;
    fonts.emplace_back(name.c_str());
  }
  return fonts;
}

/*int main() {
  WaterMarkUtils::load_system_fonts();
  std::string content = "centforever\u00a0wetest\u00a08956";
  WaterMarkUtils::ParseWaterMarkContent(content);
  return 0;
}*/
