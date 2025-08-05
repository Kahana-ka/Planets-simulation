
// Std library
#include <iostream>
#include <vector>

#include <format>

// My library
#include "solver_runge_kutta_3.h"
#include "phy.h"
#include "solver_runge_kutta_4.h"
#include "solver_runge_kutta_5.h"
#include "solver_runge_kutta_adaptive.h"
#include "gnuplotting.h"

// Name space abbreviation
using std::vector;
using std::cout;
using std::endl;



constexpr std::string_view save_path3 = "Out/p3/";
constexpr std::string_view save_path4 = "Out/p4/";
constexpr std::string_view save_path5 = "Out/p5/";
constexpr std::string_view save_pathadd = "Out/a/";

constexpr std::string_view plot_out = "Plot/";

std::vector<Data_info::Data_label> coll_labels{
	{"Time", ""},
	{"Position x", ""},
	{"Position y", ""},
	{"Position z", ""},
	{"Speed x", ""},
	{"Speed y", ""},
	{"Speed z", ""}
};

int main() {
	Data_info d1 {"Pt_1 tra","Out/p4/Pianeta_1"};
	Data_info d2 {"Pt_2 tra","Out/p4/Pianeta_2"};
	Data_info d3 {"Pt_3 tra","Out/p4/Pianeta_3"};


	//Settaggio
	d1.set_line_style({Line_attribute::none,"",3});
	d2.set_line_style({Line_attribute::none,"",3});
	d3.set_line_style({Line_attribute::none,"",3});

	d1.set_point_style({Point_attribute::none,3});
	d2.set_point_style({Point_attribute::none,3});
	d3.set_point_style({Point_attribute::none,3});

	d1.add_column_name(coll_labels);
	d2.add_column_name(coll_labels);
	d3.add_column_name(coll_labels);

	Gnu_plotter gp{{d1,d2,d3},plot_out,{1920,1080}};

	Gnu_plotter::Axis_limits a{-12,12,-12,12,0,110};

	gp.curve_plot(2,3,a,"trajector");
	gp.multi_curve_plot({{1,2,1},{1,3,1},{1,4,1}},{1,3},"3d");
	gp.animate_curve_plot(2,3,a,"traj_ani",200,15,60);

	return 0;
}



void solver_test() {

	using namespace std::literals;
	phy::Planet p1{};
	phy::Planet p2{};
	phy::Planet p3{};

	phy::set_ic(p1, 0., 0., 0., 0., 0, 0, 1000, 0.1, "Pianeta_1"sv);
	phy::set_ic(p2, 10., 0.,0.,0., 10.,0., 10., 0.1 , "Pianeta_2"sv);
	phy::set_ic(p3, -5, 0., 0.,0.,10, 0., 10, 0.1, "Pianeta_3"sv);

	vector<phy::Planet> planets{p1,p2,p3};

	double h = 0.0001;
	double start = 0;
	double end = 100;

	rk3::RK3_solver rk3_s(planets, start, end, h, 0.01);
	rk4::RK4_solver rk4_s(planets, start, end, h, 0.01);
	rk5::RK5_solver rk5_s(planets, start, end, h, 0.01);
	rk_add::RK_adaptive rkadd_s(planets, start, end, h,h/100,0.001, 0.01);

	rk3_s.set_save(save_path3,false,100);
	rk4_s.set_save(save_path4, false, 100);
	rk5_s.set_save(save_path5, false, 100);
	rkadd_s.set_save(save_pathadd, false, 500);

	rk3_s.runge_kutta_solver();
	rk4_s.runge_kutta_solver();
	rk5_s.runge_kutta_solver();
	rkadd_s.runge_kutta_solver();
}