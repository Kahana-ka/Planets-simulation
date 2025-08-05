//
// Created by kahana on 7/29/25.
//

#ifndef GNUPLOTTING_H
#define GNUPLOTTING_H

#include <string_view>
#include <vector>
#include <format>
#include "gnuplot-iostream.h"
#include "sys_utils.h"


namespace Line_attribute {
    enum Line_attribute {
        none,
    };
}

namespace Point_attribute {
    enum Point_attribute {
        none,
    };
}

class Data_info {

public:
    struct Data_label {
        std::string_view name;
        std::string_view units;
    };

    struct Line_style {
        Line_attribute::Line_attribute style{Line_attribute::none};
        std::string_view color; /// in hex
        int size {-1};
    };

    struct Point_style {
        Point_attribute::Point_attribute style{Point_attribute::none};
        int size{-1};
    };

private:
    std::string_view name;
    std::string_view path;

    Line_style line_style {};
    Point_style point_style {};

    //Deve essere ordinato come nel file
    std::vector<Data_label> columns_name;

public:

    Data_info(std::string_view name, std::string_view path);

    //Add
    void add_column_name(std::string_view name,std::string_view units);
    void add_column_name(const std::vector<std::pair<std::string_view,std::string_view>>& names);
    void add_column_name(const std::vector<Data_label>& names);

    void set_line_style(Line_attribute::Line_attribute style, std::string_view color, int size);
    void set_line_style(const Line_style &style);

    void set_point_style(Point_attribute::Point_attribute style, int size);
    void set_point_style(Point_style style);

    const std::string_view & get_name() const;
    const std::string_view & get_path() const;
    const Line_style & get_line_style() const;
    const Point_style & get_point_style() const;

    const Data_label & get_column_names(int index) const;

    size_t get_number_of_columns() const;

    bool has_point_style_data() const;
    bool has_line_style_data() const;

};

class Gnu_plotter {

    static constexpr std::string_view gnu_new_line = ",\\\n";

public:
    struct Canvas_info {
        // Da popolare
        std::string_view legend_position;
    };

    struct Axis_limits {
        int x_min{0};
        int x_max{0};
        int y_min{0};
        int y_max{0};
        int z_min{0};
        int z_max{0};
    };

private:
    // Dati dentro il vettore sono assunti dello stesso tipo
    // Quindi stessi nomi alle colonne etc....
    std::vector<Data_info>  data;
    std::string_view save_path;
    Canvas_info canvas_info;

    //Flag interni
    bool is_subplot{false};
    bool no_save{false};

    std::string set_canvas_cmd();

    std::string line_properties_cmd(const Data_info::Line_style& style);

    std::string point_properties_cmd(const Data_info::Point_style& style);

    std::string set_axis_limit_cmd(const Axis_limits& axis);

    /// TODO and TO DISCOVER
    std::string set_multi_canvas_cmd();

    void gnu_plot_command(std::string_view command);

    std::string save_plot_cmd(std::string_view plot_name);
    std::pair<double,double> resolution;
    void set_is_subplot_flag(bool subplot);

    void gnu_plot_command_w_sync(std::string_view command);

public:

    explicit Gnu_plotter(const std::vector<Data_info>& data);

    Gnu_plotter(const std::vector<Data_info>& data,std::string_view save_path,const std::pair<int,int>& resolution);


    std::string curve_plot(int x_column,int y_column,const Axis_limits& a_limit,std::string_view plot_name="");
    std::string curve_plot(int x_column,int y_column,std::string_view plot_name="");
    std::string curve_plot(int x_column,int y_column,int z_column,const Axis_limits& a_limit,std::string_view plot_name="");
    std::string curve_plot(int x_column,int y_column,int z_column,std::string_view plot_name="");

    std::string multi_curve_plot(const std::vector<std::tuple<int,int>> &columns, const std::pair<int,int> &layout,std::string_view plot_name="");
    std::string multi_curve_plot(const std::vector<std::tuple<int,int,int>> &columns, const std::pair<int,int> &layout,std::string_view plot_name="");
    std::string multi_curve_plot(const std::vector<std::tuple<int,int>>& columns,const std::pair<int,int>& layout,const std::vector<Axis_limits>& a_limit,std::string_view plot_name="");
    std::string multi_curve_plot(const std::vector<std::tuple<int,int,int>>& columns,const std::pair<int,int>& layout,const std::vector<Axis_limits>& a_limit,std::string_view plot_name="");

    std::string animate_curve_plot(int x_column,int y_column,int z_column,std::string_view plot_name,int tail,int frequency,int fps);
    std::string animate_curve_plot(int x_column,int y_column,int z_column,const Axis_limits& a_limit,std::string_view plot_name,int tail,int frequency,int fps);

    std::string animate_curve_plot(int x_column,int y_column,std::string_view plot_name,int tail,int frequency,int fps);
    std::string animate_curve_plot(int x_column,int y_column,const Axis_limits& a_limit,std::string_view plot_name,int tail,int frequency,int fps);


};






#endif //GNUPLOTTING_H
