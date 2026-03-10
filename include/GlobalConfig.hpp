#pragma once
#include <filesystem>
struct TextConfig {

	std::filesystem::path font_location = "";

};

inline TextConfig g_textConfig;