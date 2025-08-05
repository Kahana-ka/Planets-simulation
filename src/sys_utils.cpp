//
// Created by kahana on 8/1/25.
//
#include "sys_utils.h"

#include <algorithm>
#include <format>
#include <fstream>

std::string sys_utils::exec(const char* cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    const std::unique_ptr<FILE, decltype(&pclose) > pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += std::string( buffer.data());
    }

    return result;
}

long sys_utils::count_files_lines(std::string_view file_path) {
    std::ifstream ifs{std::string(file_path)};
    return  std::count_if(std::istreambuf_iterator<char>{ifs}, {}, [](char c) { return c == '\n'; });;
}

void sys_utils::delete_files(std::string_view file_path) {
    exec(std::format("rm {}",file_path).c_str());
}

void sys_utils::make_vieo(std::string_view file_path,std::string_view name,int framerate) {
    exec(std::format("yes | ffmpeg -framerate {} -pattern_type glob -i '{}*{}.png'" \
                                   " -c:v libx264 -pix_fmt yuv420p {}{}.mp4", framerate,file_path,name,file_path,name).c_str());
}