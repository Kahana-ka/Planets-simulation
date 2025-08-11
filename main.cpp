
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

// Name space abbreviation
using std::vector;
using std::cout;
using std::endl;

void solver_test();
void plot_tra(std::string_view path,std::string_view plt_name);
void plot_speed_1();
void plot_speed_2();
void plot_speed_3();

constexpr std::string_view save_path3 = "Out/p3/";
constexpr std::string_view save_path4 = "Out/p4/";
constexpr std::string_view save_path5 = "Out/p5/";

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

	solver_test();
	plot_tra(save_path3,"trajectory_3");
	plot_tra(save_path4,"trajectory_4");
	plot_tra(save_path5,"trajectory_5");
	plot_speed_3();
	plot_speed_2();
	plot_speed_1();
	return 0;
}



void solver_test() {

	using namespace std::literals;
	phy::Planet p1{};
	phy::Planet p2{};
	phy::Planet p3{};

	phy::set_ic(p1,{0.,0.,0.},{0.,0.,0.},1000,0.1, "Pianeta_1"sv);
	phy::set_ic(p2, {10.,0.,0.},{0.,10.,0.},10,0.1, "Pianeta_2"sv);
	phy::set_ic(p3,{-5.,0.,0.},{0.,10.,0.},10,0.1, "Pianeta_3"sv);

	vector<phy::Planet> planets{p1,p2,p3};

	double h = 0.0001;
	double start = 0;
	double end = 10;

	rk3::RK3_solver rk3_s(planets, h, start, end, 0.01);
	rk4::RK4_solver rk4_s(planets, h, start, end, 0.01);
	rk5::RK5_solver rk5_s(planets, h, start, end, 0.01);


	rk3_s.set_save(save_path3,true,h*20);
	rk4_s.set_save(save_path4, true, h*20);
	rk5_s.set_save(save_path5, true, h*20);




	cout << "Simulazione con rk3" << endl;
	rk3_s.solve();
	cout << "completa" << endl;

	cout << "Simulazione con rk4" << endl;
	rk4_s.solve();
	cout << "completa" << endl;

	cout << "Simulazione con rk5" << endl;
	rk5_s.solve();
	cout << "completa" << endl;


}


void plot_tra(std::string_view path,std::string_view plt_name) {

	std::string out1 = std::string(path) + "Pianeta_1";
	std::string out2 = std::string(path) + "Pianeta_2";
	std::string out3 = std::string(path) + "Pianeta_3";
	Data_info d1 {"Pt_1 tra",out1};
	Data_info d2 {"Pt_2 tra",out2};
	Data_info d3 {"Pt_3 tra",out3};

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

	gp.curve_plot(2,3,plt_name);
}

void plot_speed_1() {

	std::string out1 = std::string(save_path3) + "Pianeta_1";
	std::string out2 = std::string(save_path4) + "Pianeta_1";
	std::string out3 = std::string(save_path5) + "Pianeta_1";
	Data_info d1 {"Pt_1 rk3",out1};
	Data_info d2 {"Pt_1 rk4",out2};
	Data_info d3 {"Pt_1 rk5",out3};

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

	gp.curve_plot(1,5,"speed_1");
}

void plot_speed_2() {

	std::string out1 = std::string(save_path3) + "Pianeta_2";
	std::string out2 = std::string(save_path4) + "Pianeta_2";
	std::string out3 = std::string(save_path5) + "Pianeta_2";
	Data_info d1 {"Pt_2 rk3",out1};
	Data_info d2 {"Pt_2 rk4",out2};
	Data_info d3 {"Pt_2 rk5",out3};

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

	gp.curve_plot(1,5,"speed_2");
}
void plot_speed_3() {

	std::string out1 = std::string(save_path3) + "Pianeta_3";
	std::string out2 = std::string(save_path4) + "Pianeta_3";
	std::string out3 = std::string(save_path5) + "Pianeta_3";
	Data_info d1 {"Pt_3 rk3",out1};
	Data_info d2 {"Pt_3 rk4",out2};
	Data_info d3 {"Pt_3 rk5",out3};

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

	gp.curve_plot(1,5,"speed_3");
}
