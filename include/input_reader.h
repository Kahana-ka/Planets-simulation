//
// Created by kahana on 9/20/25.
//

#ifndef PLANETS_SIMULATION_INPUT_READER_H
#define PLANETS_SIMULATION_INPUT_READER_H

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

#include "phy.h"

using namespace std::literals;

class InputReader {

        enum Type_field {
            PLOT,
            SAVE,
            PLANET,
            PARAMETERS,
            NONE,
        };

        enum Planet_field {
            SPEED,
            POSITION,
            RADIUS,
            MASS,
            NAME,
        };

        struct Planet_setted_field {
            bool speed{false};
            bool position{false};
            bool radius{false};
            bool mass{false};
            bool name{false};
        };

        std::ifstream file;

        Type_field currently_parsing{NONE};

        const char end_context_char {'}'};
        const char start_context_char {'{'};
        const char assignment_char {'='};
        const char comment_char {'#'};
        const char list_char {','};


        const std::unordered_map<std::string,Type_field> type_map { {"plot"s + start_context_char,PLOT},
                                                                    {"save"s + start_context_char,SAVE},
                                                                    {"planet"s + start_context_char,PLANET},
                                                                    {"param"s + start_context_char,PARAMETERS} };

        const std::unordered_map<std::string, Planet_field> planet_map {{"speed"s,SPEED},
                                                                            {"position"s,POSITION},
                                                                            {"radius"s,RADIUS},
                                                                            {"mass"s,MASS},
                                                                            {"name"s,NAME} };

        const std::vector<std::string> sim_str{"param"};
        const std::vector<std::string> plot_str{"param"};
        const std::vector<std::string> save_str{"param"};
        const std::vector<std::string> param_str{"param"};

        int current_line{0};

        void parser_dispatcher(Type_field field, std::string_view line);

        void parse_parameters(std::string_view line);
        void parse_planet(std::string_view line);
        void parse_plot(std::string_view line);
        void parse_save(std::string_view line);

        void finalize_parse_field(Type_field type);
        std::vector<phy::Planet> planets;
        phy::Planet planet_in_construction{};
        Planet_setted_field planet_setted_field{};

    public:
        void read_input();
        explicit InputReader(const std::string &path);


   class InputReaderErr : public std::runtime_error {
       public:
       explicit InputReaderErr(const std::string &what) : std::runtime_error(what){}
   };
};



#endif //PLANETS_SIMULATION_INPUT_READER_H