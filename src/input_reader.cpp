//
// Created by kahana on 9/20/25.
//

#include "input_reader.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <boost/type.hpp>

InputReader::InputReader(const std::string &path){
    file.open(path);
    if (!file.is_open())
        throw InputReaderErr{"Errore"};

}


void InputReader::read_input() {

    std::string line;

    while (std::getline(file, line)) {
        current_line++;
        //Remove space
        line.erase(std::ranges::remove_if(line, isspace).begin(), line.end());

        if (line.empty() || line[0] == comment_char)
            continue;

        if (currently_parsing == NONE) {
            if (type_map.contains(line)) {
                currently_parsing = type_map.at(line);
                continue;
            }

            std::cout << "Unknown on line " << current_line << ": " << line << std::endl;
            break;

        }

        if (std::find(line.begin(),line.end(),assignment_char) == line.end() && line[0] != end_context_char) {
            std::cout << "No field fund, missing " << assignment_char << " on line " << current_line << std::endl;
            continue;
        }

        if (line ==  ""s + end_context_char) {
            finalize_parse_field(currently_parsing);
            currently_parsing = NONE;
        }

        try {
            parser_dispatcher(currently_parsing,line);
        } catch (const std::exception &e) {
            std::cout << "Invalid value on line " << current_line << ": " << line << std::endl;
        }


    }

}

void InputReader::parser_dispatcher(const Type_field field,const std::string_view line) {
    try {
        switch (field) {
            case PARAMETERS:
                parse_parameters(line);
                break;
            case PLANET:
                parse_planet(line);
                break;
            case PLOT:
                parse_plot(line);
                break;
            case SAVE:
                parse_save(line);
                break;
            default:
                break;
        }
    }catch (std::exception &e){ throw; }

}

void InputReader::parse_parameters(const std::string_view line){}


void InputReader::parse_planet(const std::string_view line) {
    std::vector<std::string> tokens;
    boost::split(tokens,line,boost::is_any_of(""s+assignment_char));
    if (tokens.size() > 2) {
        std::cout << "Too many " << assignment_char << std::endl;
        return;
    }

    if (planet_map.contains(tokens[0])) {
        const Planet_field field = planet_map.at(tokens[0]);

        try {
            switch (field) {
                case MASS:
                    if (!planet_setted_field.mass) {
                        planet_in_construction.mass = std::stod(tokens[1]);
                        planet_setted_field.mass = true;
                    }else {
                        std::cout << "Field alrady setted" << std::endl;
                    }
                    break;

                case RADIUS:
                    if (!planet_setted_field.radius) {
                        planet_in_construction.radius = std::stod(tokens[1]);
                        planet_setted_field.radius = true;
                    }else {
                        std::cout << "Field alrady setted" << std::endl;
                    }
                    break;

                case SPEED:
                    if (!planet_setted_field.speed) {
                        std::vector<std::string> subtokens;
                        boost::split(subtokens,tokens[1],boost::is_any_of(""s+list_char));

                        if (subtokens.size() == 2) {
                            planet_in_construction.speed.emplace_back(std::stoi(subtokens[0]),std::stod(subtokens[1]),0.);
                        } else if (subtokens.size() == 3) {
                            planet_in_construction.speed.emplace_back(std::stoi(subtokens[0]),std::stod(subtokens[1]),std::stod(subtokens[2]));
                        } else if (subtokens.size() > 3){
                            std::cout << "Too many " << list_char << std::endl;
                        } else {
                            std::cout << "Too few" << std::endl;
                        }
                        planet_setted_field.speed = true;
                    }else {
                        std::cout << "Field alrady setted" << std::endl;
                    }
                    break;

                case POSITION:
                    if (!planet_setted_field.position) {
                        std::vector<std::string> subtokens;
                        boost::split(subtokens,tokens[1],boost::is_any_of(""s+list_char));

                        if (subtokens.size() == 2) {
                            planet_in_construction.position.emplace_back(std::stoi(subtokens[0]),std::stod(subtokens[1]),0.);
                        } else if (subtokens.size() == 3) {
                            planet_in_construction.position.emplace_back(std::stoi(subtokens[0]),std::stod(subtokens[1]),std::stod(subtokens[2]));
                        } else if (subtokens.size() > 3){
                            std::cout << "Too many " << list_char << std::endl;
                        } else {
                            std::cout << "Too few" << std::endl;
                        }
                        planet_setted_field.position = true;
                    }else {
                        std::cout << "Field alrady setted" << std::endl;
                    }
                    break;

                case NAME:
                    if (!planet_setted_field.name) {
                        planet_in_construction.id = tokens[1];
                        planet_setted_field.name = true;
                    }else {
                        std::cout << "Field alrady setted" << std::endl;
                    }
                    break;
            }
        }catch (std::invalid_argument &e) {
             throw;
        }

    } else {
        std::cout << "Field not existing: " << tokens[0] << std::endl;
    }


}

void InputReader::parse_plot(const std::string_view line){}

void InputReader::parse_save(const std::string_view line){}

void InputReader::finalize_parse_field(const Type_field type) {

    switch (type) {
        case PLANET:
            if (planet_setted_field.mass &&
                planet_setted_field.radius &&
                planet_setted_field.name &&
                planet_setted_field.speed &&
                planet_setted_field.position) {
                    planets.emplace_back(planet_in_construction);


            } else {
                std::cout << "Missing fields" << std::endl;
            }

            planet_setted_field.mass = false;
            planet_setted_field.radius = false;
            planet_setted_field.name = false;
            planet_setted_field.speed = false;
            planet_setted_field.position = false;
            planet_in_construction.speed.clear();
            planet_in_construction.position.clear();
            break;
        default: ;
    }

}