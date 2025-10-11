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

    public:
        enum Model {
            RK3,
            RK4,
            RK5,
        };


        struct Sim {
            double step{0};
            double start_time{0};
            double end_time{0};
            double collision_toll{0};
            Model model;
            std::string save_path;
            bool save_all{false};
            int save_every{0};
        };


    private:
        enum Type_field {
            PLANET,
            SIM,
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


        struct Sim_setted_field {
            bool step{false};
            bool start_time{false};
            bool end_time{false};
            bool collision_toll{false};
            bool model{false};
            bool save_path{false};
            bool save_all{false};
            bool save_every{false};
        };

        enum Sim_field {
            STEP,
            START_TIME,
            END_TIME,
            COLLISION_TOLL,
            MODEL,
            SAVE_PATH,
            SAVE_ALL,
            SAVE_EVERY,
        };




        std::ifstream file;

        Type_field currently_parsing{NONE};

        const char end_context_char {'}'};
        const char start_context_char {'{'};
        const char assignment_char {'='};
        const char comment_char {'#'};
        const char list_char {','};


        const std::unordered_map<std::string,Type_field> type_map { {"planet"s + start_context_char,PLANET},
                                                                    {"sim"s + start_context_char,SIM} };

        const std::unordered_map<std::string, Planet_field> planet_map {{"speed"s,SPEED},
                                                                            {"position"s,POSITION},
                                                                            {"radius"s,RADIUS},
                                                                            {"mass"s,MASS},
                                                                            {"name"s,NAME} };

        const std::unordered_map<std::string,Model> model_map {{"rk3"s,RK3},{"rk4"s,RK4},{"rk5"s,RK5}};
        const std::unordered_map<std::string,Sim_field> sim_map {{"step"s,STEP},
                                                                                    {"start_time"s,START_TIME},
                                                                                    {"end_time"s,END_TIME},
                                                                                {"collision_tollerance"s,COLLISION_TOLL},
                                                                                  {"model"s,MODEL},{"save_path"s,SAVE_PATH},
                                                                                    {"save_all"s,SAVE_ALL},{"save_every",SAVE_EVERY}};

        int current_line{0};

        void parser_dispatcher(Type_field field, std::string_view line);

        void parse_parameters(std::string_view line);
        void parse_planet(std::string_view line);

        void finalize_parse_field(Type_field type);

        phy::Planet planet_in_construction{};
        Planet_setted_field planet_setted_field{};
        Sim_setted_field sim_setted_field{};
        std::vector<phy::Planet> planets;
        std::vector<Sim> sims;
        Sim temp_sim{};
    public:
        void read_input();
        explicit InputReader(const std::string &path);
        std::vector<phy::Planet> get_planets();

   std::vector<Sim> get_sims();

   class InputReaderErr : public std::runtime_error {
       public:
       explicit InputReaderErr(const std::string &what) : std::runtime_error(what){}
   };
};



#endif //PLANETS_SIMULATION_INPUT_READER_H