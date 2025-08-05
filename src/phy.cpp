
#include "phy.h"


void phy::set_ic(phy::Planet& p, double x, double y, double z, double vx, double vy,double vz, double m,double r, std::string_view id) {
	p.pos_x.push_back(x);
	p.pos_y.push_back(y);
	p.pos_z.push_back(z);
	p.speed_x.push_back(vx);
	p.speed_y.push_back(vy);
	p.speed_z.push_back(vz);
	p.mass = m;
	p.radius = r;
	p.id = id;
}


double phy::gravity_calc_x(const double m,const double x1,const double x2,const double y1,const double y2, const  double z1, const  double z2) {
	return G * (m) / pow(pow(x1 - x2, 2.) + pow(y1 - y2, 2.) + pow(z1 - z2, 2.), 3./2.) * (x2 - x1);
}

double phy::gravity_calc_y(const double m, const  double x1, const  double x2, const  double y1, const  double y2, const  double z1, const  double z2) {
	return G * (m) / pow(pow(x1 - x2, 2.) + pow(y1 - y2, 2.) + pow(z1 - z2, 2.), 3. / 2.) * (y2 - y1);
}

double phy::gravity_calc_z(const double m, const  double x1, const  double x2, const  double y1, const  double y2, const  double z1, const  double z2) {
	return G * (m) / pow(pow(x1 - x2, 2.) + pow(y1 - y2, 2.) + pow(z1 - z2, 2.), 3. / 2.) * (z2 - z1);
}