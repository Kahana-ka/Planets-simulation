//
// Created by kahana on 7/29/25.
//

#include "gnuplotting.h"
#include  <ranges>

/////////////////////////////////////////////////////////////////////////////////////
///                                                                               ///
///                                 GNU PLOTTER                                   ///
///                                                                               ///
/////////////////////////////////////////////////////////////////////////////////////

void Gnu_plotter::gnu_plot_command_w_sync(const std::string_view command) {
    gnu_plot_command(command.data());

    // aspetta che gnuplot finisca
    while (!sys_utils::exec("pidof gnuplot").empty()){}
}

void Gnu_plotter::set_is_subplot_flag(const bool subplot) {
    no_save = subplot;
    is_subplot = subplot;
}

std::string Gnu_plotter::set_canvas_cmd() {
    if (is_subplot)
        return "";
    return "";
}

std::string Gnu_plotter::line_properties_cmd(const Data_info::Line_style& style) {

    std::string command;

    if (style.style != Line_attribute::none) {
        command = std::format("lt {} ",static_cast<int>(style.style));
    }

    if (!style.color.empty()) {
        command = std::format("lc rgb '{}' ",style.color);
    }

    if (style.size > -1) {
        command = std::format("lw {} ",style.size);
    }

    return command;
}

std::string Gnu_plotter::point_properties_cmd(const Data_info::Point_style& style) {

    std::string command;

    if (style.style != Point_attribute::none) {
        command = std::format("pt {} ",static_cast<int>(style.style));
    }

    if (style.size > -1) {
        command = std::format("ps {} ",style.size);
    }

    return command;
}


std::string Gnu_plotter::set_axis_limit_cmd(const Axis_limits& axis) {
    std::string command;

    if (axis.x_max > axis.x_min) {
        command += std::format("set xrange [{}:{}] \n",axis.x_min,axis.x_max);
    }

    if (axis.y_max > axis.y_min) {
        command += std::format("set yrange [{}:{}] \n",axis.y_min,axis.y_max);
    }

    if (axis.z_max > axis.z_min) {
        command += std::format("set zrange [{}:{}] \n",axis.z_min,axis.z_max);
    }

    return command;
}

/// TODO and TO DISCOVER
std::string Gnu_plotter::set_multi_canvas_cmd() {
    if (is_subplot)
        return "";
    return "";
}


void Gnu_plotter::gnu_plot_command(const std::string_view command) {
    Gnuplot gp("gnuplot -persist");
    gp << command;
}

std::string Gnu_plotter::save_plot_cmd(std::string_view plot_name) {
    if (no_save) {
        return "";
    }

    std::string command;
    command += std::format("set term pngcairo size {},{}\n",resolution.first,resolution.second);
    command += std::format("set output '{}{}.png'\n",save_path,plot_name);
    return command;
}

/// ~~~~~~~~~~~~ Parte pubblica ~~~~~~~~~~~~~~~~

Gnu_plotter::Gnu_plotter(const std::vector<Data_info>& data) : Gnu_plotter(data,"",{}){}

Gnu_plotter::Gnu_plotter(const std::vector<Data_info>& data, const std::string_view save_path,const std::pair<int,int>& resolution) {
    this->data = data;
    this->save_path = save_path;
    this->resolution = resolution;
}

std::string Gnu_plotter::curve_plot(const int x_column, const int y_column, const std::string_view plot_name) {
    constexpr Axis_limits a{};
    return curve_plot(x_column,y_column,a,plot_name);
}
std::string Gnu_plotter::curve_plot(const int x_column, const int y_column, const int z_column, const std::string_view plot_name) {
    constexpr Axis_limits a{};
    return curve_plot(x_column,y_column,z_column,a,plot_name);
}

std::string Gnu_plotter::curve_plot(int x_column,int y_column,const Axis_limits& a_limit, const std::string_view plot_name) {

    std::string command;

    if (!save_path.empty() && !plot_name.empty()) {
        command += save_plot_cmd(plot_name);
    }

    command += set_canvas_cmd();
    command += set_axis_limit_cmd(a_limit);
    command += "plot ";

    for (const auto& d : data) {
        command += std::format("'{}' u {}:{} title '{}' ",d.get_path(),x_column,y_column,d.get_name());

        if (d.has_point_style_data()) {
            command += "w lp ";
            command += line_properties_cmd(d.get_line_style());
            command += point_properties_cmd(d.get_point_style());
        }else{
            command += "w l ";
            command += line_properties_cmd(d.get_line_style());
        }
        command += gnu_new_line;
    }

    command = command.substr(0, command.size()-gnu_new_line.size());
    command += "\n";
    //Debug
    std::cout << command << std::endl;
    if (!is_subplot)
        gnu_plot_command(command);

    return command;
}

std::string Gnu_plotter::curve_plot(int x_column,int y_column,int z_column,const Axis_limits& a_limit, const std::string_view plot_name) {
    std::string command;

    if (!save_path.empty() && !plot_name.empty()) {
        command += save_plot_cmd(plot_name);
    }

    command += set_canvas_cmd();
    command += set_axis_limit_cmd(a_limit);
    command += "splot ";

    for (const auto& d : data) {
        command += std::format("'{}' u {}:{}:{} title '{}' ",d.get_path(),x_column,y_column,z_column,d.get_name());

        if (d.has_point_style_data()) {
            command += "w lp ";
            command += line_properties_cmd(d.get_line_style());
            command += point_properties_cmd(d.get_point_style());
        }else{
            command += "w l ";
            command += line_properties_cmd(d.get_line_style());
        }
        command += gnu_new_line;
    }

    command = command.substr(0, command.size()-gnu_new_line.size());
    command += "\n";
    //Debug
    std::cout << command << std::endl;

    if (!is_subplot)
        gnu_plot_command(command);

    return command;
}

std::string Gnu_plotter::multi_curve_plot(const std::vector<std::tuple<int,int>> &columns, const std::pair<int,int> &layout,std::string_view plot_name) {
    return multi_curve_plot(columns,layout,{},plot_name);
}

std::string Gnu_plotter::multi_curve_plot(const std::vector<std::tuple<int,int,int>> &columns, const std::pair<int,int> &layout,std::string_view plot_name) {
    return multi_curve_plot(columns,layout,{},plot_name);
}

std::string Gnu_plotter::multi_curve_plot(const std::vector<std::tuple<int,int>>& columns,const std::pair<int,int>& layout,const std::vector<Axis_limits>& a_limit,std::string_view plot_name) {


    std::string command;

    if (!save_path.empty() && !plot_name.empty()) {
        command += save_plot_cmd(plot_name);
    }

    command += set_multi_canvas_cmd();
    command += std::format("set multiplot layout {},{} rowsfirst\n",layout.first,layout.second);

    bool need_to_limit_axis = false;

    if (!a_limit.empty()) {
        if (columns.size() != a_limit.size()) {
            return "";
        }
        need_to_limit_axis = true;
    }

    auto limit_it = a_limit.begin();
    auto column_it = columns.begin();


    set_is_subplot_flag(true);
    for (;column_it != columns.end();++column_it,limit_it) {
        if (need_to_limit_axis)
            command += set_axis_limit_cmd(*limit_it);

        command += curve_plot(get<0>(*column_it),get<1>(*column_it));
    }
    set_is_subplot_flag(false);

    command += "unset multiplot\n";

    //debug
    std::cout << command << std::endl;
    gnu_plot_command(command);
    return command;
}

std::string Gnu_plotter::multi_curve_plot(const std::vector<std::tuple<int,int,int>>& columns,const std::pair<int,int>& layout,const std::vector<Axis_limits>& a_limit,std::string_view plot_name) {

    std::string command;

    if (!save_path.empty() && !plot_name.empty()) {
        command += save_plot_cmd(plot_name);
    }

    command += set_multi_canvas_cmd();
    command += std::format("set multiplot layout {},{} rowsfirst\n",layout.first,layout.second);

    bool need_to_limit_axis = false;

    if (!a_limit.empty()) {
        if (columns.size() != a_limit.size())
            return "";
        need_to_limit_axis = true;
    }


    auto limit_it = a_limit.begin();
    auto column_it = columns.begin();

    set_is_subplot_flag(true);
    for (;column_it != columns.end();++column_it,limit_it) {
        if (need_to_limit_axis)
            command += set_axis_limit_cmd(*limit_it);

        command += curve_plot(get<0>(*column_it),get<1>(*column_it),get<2>(*column_it));
    }
    set_is_subplot_flag(false);

    command += "unset multiplot\n";

    //debug
    std::cout << command << std::endl;
    gnu_plot_command(command);
    return command;

}


std::string Gnu_plotter::animate_curve_plot_trajectory_3d(const std::string_view plot_name,int tail,int frequency,int fps) {
    constexpr Axis_limits a{};
    return animate_curve_plot_trajectory_3d(a,plot_name,tail,frequency,fps);
}

std::string Gnu_plotter::animate_curve_plot_trajectory_3d(const Axis_limits& a_limit, const std::string_view plot_name,int tail,int frequency,int fps) {

    // e forzato a salvare quindi se non ha i dati esce
    if (save_path.empty() || plot_name.empty()) {
        return "";
    }

    std::string command;

    std::string data_file = "";
    if (data.size() > 1)
        data_file = join_files_position();
    else
        data_file = data[0].get_path();

    const int lines_number = sys_utils::count_files_lines(data_file);

    command += set_canvas_cmd();
    command += set_axis_limit_cmd(a_limit);
    command += std::format("set term pngcairo size {},{}\n",resolution.first,resolution.second);
    command += std::format("do for [i= 1:{}:{}]{{\n",lines_number,frequency);

    set_is_subplot_flag(true); // Non serve basta non passarli il nome ma non si sa mai
    command += std::format("set output sprintf('{}%06.0f_{}.png', i) \n",save_path,plot_name);
    command += "splot ";

    int coll_off = 2;
    for (auto& d : data) {
        if (tail > -1)
            command += std::format("'{}' u {}:{}:{} every ::i-{}::i title '{}' ",data_file,coll_off,coll_off +1,coll_off+2,tail,d.get_name());
        else
            command += std::format("'{}' u {}:{}:{} every ::1::i title '{}' ",data_file,coll_off,coll_off+1,coll_off+2,d.get_name());

        command += "w l ";
        command += line_properties_cmd(d.get_line_style());

        command += gnu_new_line;

        if (d.has_point_style_data()) {
            command += std::format("'{}' u {}:{}:{} every ::i::i title '{}' w p ",data_file,coll_off,coll_off+1,coll_off+2,d.get_name());
            command += line_properties_cmd(d.get_line_style());
            command += point_properties_cmd(d.get_point_style());
            command += gnu_new_line;
        }
        coll_off += 3;
    }
    set_is_subplot_flag(false);
    command = command.substr(0, command.size()-gnu_new_line.size());
    command += "\n}";

    //DEbug
    std::cout << command << std::endl;
    gnu_plot_command_w_sync(command);
    std::cout << "Fino gnuare" << std::endl;
    sys_utils::make_vieo(save_path,plot_name,fps);
    sys_utils::delete_files(std::format("{}*_{}.png",save_path,plot_name));
    return command;
}

std::string Gnu_plotter::animate_curve_plot_trajectory_2d(const std::string_view plot_name, const int tail, const int frequency, const int fps) {
    constexpr Axis_limits a{};
    return animate_curve_plot_trajectory_2d(a,plot_name,tail,frequency,fps);
}

std::string Gnu_plotter::animate_curve_plot_trajectory_2d(const Axis_limits& a_limit, const std::string_view plot_name,int tail,int frequency,int fps) {

    // e forzato a salvare quindi se non ha i dati esce
    if (save_path.empty() || plot_name.empty()) {
        return "";
    }

    std::string command;
    std::string data_file = "";
    if (data.size() > 1)
        data_file = join_files_position();
    else
        data_file = data[0].get_path();

    const int lines_number = static_cast<int>( sys_utils::count_files_lines(data_file));

    command += set_canvas_cmd();
    command += set_axis_limit_cmd(a_limit);
    command += std::format("set term pngcairo size {},{}\n",resolution.first,resolution.second);

    command += std::format("do for [i= 1:{}:{}]{{\n",lines_number,frequency);

    set_is_subplot_flag(true); // Non serve basta non passarli il nome ma non si sa mai
    command += std::format("set output sprintf('{}%06.0f_{}.png', i) \n",save_path,plot_name);
    command += "plot ";

    int coll_off = 2;
    for (auto& d : data) {
        if (tail > -1)
            command += std::format("'{}' u {}:{} every ::i-{}::i title '{}' ",data_file,coll_off ,coll_off + 1 ,tail,d.get_name());
        else
            command += std::format("'{}' u {}:{} every ::1::i title '{}' ",data_file,coll_off ,coll_off + 1 ,d.get_name());

        command += "w l ";
        command += line_properties_cmd(d.get_line_style());

        command += gnu_new_line;

        if (d.has_point_style_data()) {
            command += std::format("'{}' u {}:{} every ::i::i title '{}' w p ",data_file, coll_off , coll_off + 1 ,d.get_name());
            command += line_properties_cmd(d.get_line_style());
            command += point_properties_cmd(d.get_point_style());
            command += gnu_new_line;
        }
        coll_off += 3;
    }
    set_is_subplot_flag(false);
    command = command.substr(0, command.size()-gnu_new_line.size());
    command += "\n}";

    //DEbug
    std::cout << command << std::endl;
    gnu_plot_command_w_sync(command);
    std::cout << "Fino gnuare" << std::endl;
    sys_utils::make_vieo(save_path,plot_name,fps);
    sys_utils::delete_files(std::format("{}*_{}.png",save_path,plot_name));
    return command;
}



// Estremo schifo ma funge

std::string Gnu_plotter::join_files_position() {

    int temp_id = 1;

    if (data.size()<2) {
        return "";
    }

    //jino i primi due file
    std::string join = std::format("join -a1 -a2 -e NaN -o 0 1.2 1.3 1.4 2.2 2.3 2.4 {} {} > {}temp0",data[0].get_path(),data[1].get_path(),save_path);

    system(join.c_str());

    if (data.size()<3) {
        return std::format("{}{}",save_path,"temp0");
    }

    for (int i = 2 ; i < data.size(); i++) {
        std::string o{"-o 0 "};
        for (int j = 0; j<i*3; j++)
            o += std::format("1.{} ",2+j);
        o += "2.2 2.3 2.4 ";
        join = std::format("join -a1 -a2 -e NaN {} {}temp{} {} > {}temp{}",o,save_path,temp_id-1,data[i].get_path(),save_path,temp_id);
        system(join.c_str());
        temp_id++;
    }
    return std::format("{}temp{}",save_path,temp_id-1);
}










/////////////////////////////////////////////////////////////////////////////////////
///                                                                               ///
///                                 Data info                                     ///
///                                                                               ///
/////////////////////////////////////////////////////////////////////////////////////




Data_info::Data_info(const std::string_view name, const std::string_view path) {
    this->path = path;
    this->name = name;
}

//Add
void Data_info::add_column_name(const std::string_view name, const std::string_view units) {
    columns_name.push_back({name,units});
}

void Data_info::add_column_name(const std::vector<std::pair<std::string_view,std::string_view>>& names) {
    for (auto& [fst, snd] : names)
        columns_name.push_back({fst,snd});
}

void Data_info:: add_column_name(const std::vector<Data_label>& names) {
    columns_name = names;
}

void Data_info::set_line_style(const Line_attribute::Line_attribute style, const std::string_view color, const int size) {
    line_style.style = style;
    line_style.color = color;
    line_style.size = size;
}

void Data_info::set_line_style(const Line_style &style) {
    line_style = style;
}

void Data_info::set_point_style(const Point_attribute::Point_attribute style, const int size) {
    point_style.style = style;
    point_style.size = size;
}

void Data_info::set_point_style(const Point_style style) {
    point_style = style;
}

const std::string_view & Data_info::get_name() const {
    return name;
}

const std::string_view & Data_info::get_path() const {
    return path;
}

const Data_info::Line_style & Data_info::get_line_style() const {
    return line_style;
}

const Data_info::Point_style & Data_info::get_point_style() const {
    return point_style;
}

const Data_info::Data_label & Data_info::get_column_names(const int index) const {
    return columns_name[index];
}

size_t Data_info::get_number_of_columns() const {
    return columns_name.size();
}

bool Data_info::has_point_style_data() const {
    if (point_style.style == Point_attribute::none && point_style.size <= -1) {
        return false;
    }
    return true;
}

bool Data_info::has_line_style_data() const {

    if (line_style.style == Line_attribute::none && line_style.size <= -1 && line_style.color.empty()) {
        return false;
    }
    return true;
}























































