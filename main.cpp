
// Std library
#include <iostream>
#include <vector>

#include <format>

// My library
#include "solver_runge_kutta_3.h"
#include "phy.h"
#include "solver_runge_kutta_4.h"
#include "solver_runge_kutta_5.h"
#include "gnuplotting.h"
#include <filesystem>
#include "input_reader.h"

// Name space abbreviation
using std::vector;
using std::cout;
using std::endl;

void solver_test();
void plot_trajectory(std::string_view path,std::string_view plt_name);


constexpr std::string_view save_path3 = "Out/p3/";
constexpr std::string_view save_path4 = "Out/p4/";
constexpr std::string_view save_path5 = "Out/p5/";

constexpr std::string_view plot_out = "Plot/";

// std::vector<Data_info::Data_label> coll_labels{
// 	{"Time", ""},
// 	{"Position x", ""},
// 	{"Position y", ""},
// 	{"Position z", ""},
// 	{"Speed x", ""},
// 	{"Speed y", ""},
// 	{"Speed z", ""}
// };

int main(int argc, char** argv) {

	if (argc < 2) {
		std::cout <<  "Missing file" << std::endl;
	}

	InputReader input_reader{argv[1]};
	input_reader.read_input();

	return 0;
}



void solver_test() {

	using namespace std::literals;
	phy::Planet p1{};
	phy::Planet p2{};
	phy::Planet p3{};
	phy::Planet p4{};

	phy::set_ic(p1,{0.,0.,0.},{1.,1.,0.},1000,0.1, "Pianeta_1"sv);
	phy::set_ic(p2, {0.,11.,0.},{-1.,1.,0.},10,0.1, "Pianeta_2"sv);
	phy::set_ic(p3,{-15.,0.,0.},{0.,1.,0.},10,0.1, "Pianeta_3"sv);
	phy::set_ic(p4,{0.,0.,15.},{-1.,0.4,-1.},100,0.1, "Pianeta_4"sv);

	vector planets{p1,p2,p3,p4};

	double h = 0.0001;
	double start = 0;
	double end = 10;

	rk3::RK3_solver rk3_s(planets, h, start, end, 0.01);
	//rk4::RK4_solver rk4_s(planets, h, start, end, 0.01);
	//rk5::RK5_solver rk5_s(planets, h, start, end, 0.01);

	rk3_s.set_save(save_path3,true,h*20);
	//rk4_s.set_save(save_path4, true, h*20);
	//rk5_s.set_save(save_path5, true, h*20);

	cout << "Simulazione con rk3" << endl;
	rk3_s.solve();
	cout << "completa" << endl;

	//cout << "Simulazione con rk4" << endl;
	//rk4_s.solve();
	//cout << "completa" << endl;

	//cout << "Simulazione con rk5" << endl;
	//rk5_s.solve();
	//cout << "completa" << endl;


}


// void plot_trajectory(std::string_view dir,std::string_view plt_name) {
//
// 	std::vector<Data_info> data;
// 	int temp = 1;
// 	for (const auto & entry : std::filesystem::directory_iterator(dir)) {
// 		std::string out = std::string(entry.path());
// 		data.emplace_back("Pt_" + std::to_string(temp) + " tra",out);
// 		data.back().set_line_style({Line_attribute::none,"",3});
// 		data.back().set_point_style({Point_attribute::none,3});
// 		data.back().add_column_name(coll_labels);
// 		temp++;
// 	}
//
//
// 	Gnu_plotter gp{data,std::string(plot_out),{1920,1080}};
// 	Gnu_plotter::Axis_limits a{-15,15,-10,40,-40,10};
//
// 	//gp.curve_plot(2,3,4,plt_name);
// 	gp.animate_curve_plot(2,3,4,a,plt_name,2000,300,60);
// }

