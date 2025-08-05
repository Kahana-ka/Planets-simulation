//
// Created by kahana on 8/1/25.
//

#ifndef SYS_UTILS_H
#define SYS_UTILS_H



#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

namespace sys_utils {
    std::string exec(const char* cmd);

    long count_files_lines(std::string_view file_path);

    void delete_files(std::string_view file_path);

    void make_vieo(std::string_view file_path,std::string_view out_name,int framerate);

}
#endif //SYS_UTILS_H
