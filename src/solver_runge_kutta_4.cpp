
#include "solver_runge_kutta_4.h"

// *************   Metodi privati ********************************

// ~~~~~~~~~~~~~~~ Metodi per calcolare i k ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//K1

std::tuple<double, double, double> rk4::gravity_k1(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, const double h) {
	double kx = 0;
	double ky = 0;
	double kz = 0;
	//Viene fatta per tutti i pianeti meno se stesso.
	for (auto& pl : planets_solver) {
		if (pl.planet.id == self.planet.id || pl.dead)
			continue;

		kx += phy::gravity_calc_x(pl.planet.mass, self.planet.pos_x.back(), pl.planet.pos_x.back(), self.planet.pos_y.back(), pl.planet.pos_y.back(), self.planet.pos_z.back(), pl.planet.pos_z.back());
		ky += phy::gravity_calc_y(pl.planet.mass, self.planet.pos_x.back(), pl.planet.pos_x.back(), self.planet.pos_y.back(), pl.planet.pos_y.back(), self.planet.pos_z.back(), pl.planet.pos_z.back());
		kz += phy::gravity_calc_z(pl.planet.mass, self.planet.pos_x.back(), pl.planet.pos_x.back(), self.planet.pos_y.back(), pl.planet.pos_y.back(), self.planet.pos_z.back(), pl.planet.pos_z.back());
	}

	return std::tuple{kx* h, ky* h, kz* h};
}

std::tuple<double, double, double> rk4::position_k1(const rk::Planet_solver& self, const double h) {
	const double kx = self.planet.speed_x.back();
	const double ky = self.planet.speed_y.back();
	const double kz = self.planet.speed_z.back();
	return std::tuple{kx* h, ky* h, kz* h};
}

//K2

std::tuple<double, double, double> rk4::gravity_k2(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, const double h) {

	using std::get;

	double kx = 0;
	double ky = 0;
	double kz = 0;

	for (auto& pl : (planets_solver)) {
		if (pl.planet.id == self.planet.id || pl.dead)
			continue;

		const double x1 = arg_k2(self.planet.pos_x.back(), get<x>(self.k_pos[K1]));
		const double x2 = arg_k2(pl.planet.pos_x.back(), get<x>(pl.k_pos[K1]));

		const double y1 = arg_k2(self.planet.pos_y.back(), get<y>(self.k_pos[K1]));
		const double y2 = arg_k2(pl.planet.pos_y.back(), get<y>(pl.k_pos[K1]));

		const double z1 = arg_k2(self.planet.pos_z.back(), get<z>(self.k_pos[K1]));
		const double z2 = arg_k2(pl.planet.pos_z.back(), get<z>(pl.k_pos[K1]));

		kx += phy::gravity_calc_x(pl.planet.mass, x1, x2, y1, y2, z1, z2);
		ky += phy::gravity_calc_y(pl.planet.mass, x1, x2, y1, y2, z1, z2);
		kz += phy::gravity_calc_z(pl.planet.mass, x1, x2, y1, y2, z1, z2);
	}

	return std::tuple{kx* h, ky* h, kz* h};
}

std::tuple<double, double, double> rk4::position_k2(const rk::Planet_solver& self, const double h) {
	const double kx = arg_k2(self.planet.speed_x.back(), get<x>(self.k_speed[K1]));
	const double ky = arg_k2(self.planet.speed_y.back(), get<y>(self.k_speed[K1]));
	const double kz = arg_k2(self.planet.speed_z.back(), get<z>(self.k_speed[K1]));
	return std::tuple{kx* h, ky* h, kz* h};
}

double rk4::arg_k2(const double param, const double k1) {
	return param + k1 / 4.;
}

//k3

std::tuple<double, double, double> rk4::gravity_k3(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, const double h) {

	double kx = 0;
	double ky = 0;
	double kz = 0;

	for (auto& pl : planets_solver) {
		if (pl.planet.id == self.planet.id || pl.dead)
			continue;

		const double x1 = arg_k3(self.planet.pos_x.back(), get<x>(self.k_pos[K1]), get<x>(self.k_pos[K2]));
		const double x2 = arg_k3(pl.planet.pos_x.back(), get<x>(pl.k_pos[K1]), get<x>(pl.k_pos[K2]));

		const double y1 = arg_k3(self.planet.pos_y.back(), get<y>(self.k_pos[K1]), get<y>(self.k_pos[K2]));
		const double y2 = arg_k3(pl.planet.pos_y.back(), get<y>(pl.k_pos[K1]), get<y>(pl.k_pos[K2]));

		const double z1 = arg_k3(self.planet.pos_z.back(), get<z>(self.k_pos[K1]), get<z>(self.k_pos[K2]));
		const double z2 = arg_k3(pl.planet.pos_z.back(), get<z>(pl.k_pos[K1]), get<z>(pl.k_pos[K2]));

		kx += phy::gravity_calc_x(pl.planet.mass, x1, x2, y1, y2, z1, z2);
		ky += phy::gravity_calc_y(pl.planet.mass, x1, x2, y1, y2, z1, z2);
		kz += phy::gravity_calc_z(pl.planet.mass, x1, x2, y1, y2, z1, z2);
	}

	return std::tuple{kx* h, ky* h, kz* h};
}

std::tuple<double, double, double> rk4::position_k3(const rk::Planet_solver& self, const double h) {
	const double kx = arg_k3(self.planet.speed_x.back(), get<x>(self.k_speed[K1]), get<x>(self.k_speed[K2]));
	const double ky = arg_k3(self.planet.speed_y.back(), get<y>(self.k_speed[K1]), get<y>(self.k_speed[K2]));
	const double kz = arg_k3(self.planet.speed_z.back(), get<z>(self.k_speed[K1]), get<z>(self.k_speed[K2]));
	return std::tuple{kx* h, ky* h, kz* h};
}

double rk4::arg_k3(const double param, const double k1, const double k2) {
	return param + 3. * k1 / 32. + 9. / 32. * k2;
}

//k4

std::tuple<double, double, double> rk4::gravity_k4(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, const double h) {

	double kx = 0;
	double ky = 0;
	double kz = 0;

	for (auto& pl : planets_solver) {
		if (pl.planet.id == self.planet.id || pl.dead)
			continue;

		const double x1 = arg_k4(self.planet.pos_x.back(), get<x>(self.k_pos[K1]), get<x>(self.k_pos[K2]), get<x>(self.k_pos[K3]));
		const double x2 = arg_k4(pl.planet.pos_x.back(), get<x>(pl.k_pos[K1]), get<x>(pl.k_pos[K2]), get<x>(pl.k_pos[K3]));

		const double y1 = arg_k4(self.planet.pos_y.back(), get<y>(self.k_pos[K1]), get<y>(self.k_pos[K2]), get<y>(self.k_pos[K3]));
		const double y2 = arg_k4(pl.planet.pos_y.back(), get<y>(pl.k_pos[K1]), get<y>(pl.k_pos[K2]), get<y>(pl.k_pos[K3]));

		const double z1 = arg_k4(self.planet.pos_z.back(), get<z>(self.k_pos[K1]), get<z>(self.k_pos[K2]), get<z>(self.k_pos[K3]));
		const double z2 = arg_k4(pl.planet.pos_z.back(), get<z>(pl.k_pos[K1]), get<z>(pl.k_pos[K2]), get<z>(pl.k_pos[K3]));

		kx += phy::gravity_calc_x(pl.planet.mass, x1, x2, y1, y2, z1, z2);
		ky += phy::gravity_calc_y(pl.planet.mass, x1, x2, y1, y2, z1, z2);
		kz += phy::gravity_calc_z(pl.planet.mass, x1, x2, y1, y2, z1, z2);
	}

	return std::tuple{kx* h, ky* h, kz* h};
}

std::tuple<double, double, double> rk4::position_k4(const rk::Planet_solver& self, const double h) {
	const double kx = arg_k4(self.planet.speed_x.back(), get<x>(self.k_speed[K1]), get<x>(self.k_speed[K2]), get<x>(self.k_speed[K3]));
	const double ky = arg_k4(self.planet.speed_y.back(), get<y>(self.k_speed[K1]), get<y>(self.k_speed[K2]), get<y>(self.k_speed[K3]));
	const double kz = arg_k4(self.planet.speed_z.back(), get<z>(self.k_speed[K1]), get<z>(self.k_speed[K2]), get<z>(self.k_speed[K3]));
	return std::tuple{kx* h, ky* h, kz* h};
}

double rk4::arg_k4(const double param, const double k1, const double k2, const double k3) {
	return param + k1*1932./2197. - k2*7200./2197. + k3*7296./2197.;
}

//k5

std::tuple<double, double, double> rk4::gravity_k5(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, const double h) {

	double kx = 0;
	double ky = 0;
	double kz = 0;

	for (auto& pl : planets_solver) {
		if (pl.planet.id == self.planet.id || pl.dead)
			continue;

		const double x1 = arg_k5(self.planet.pos_x.back(), get<x>(self.k_pos[K1]), get<x>(self.k_pos[K2]), get<x>(self.k_pos[K3]), get<x>(self.k_pos[K4]));
		const double x2 = arg_k5(pl.planet.pos_x.back(),   get<x>(pl.k_pos[K1]),   get<x>(pl.k_pos[K2]),    get<x>(pl.k_pos[K3]),  get<x>(pl.k_pos[K4]));

		const double y1 = arg_k5(self.planet.pos_y.back(), get<y>(self.k_pos[K1]), get<y>(self.k_pos[K2]), get<y>(self.k_pos[K3]), get<y>(self.k_pos[K4]));
		const double y2 = arg_k5(pl.planet.pos_y.back(),   get<y>(pl.k_pos[K1]),   get<y>(pl.k_pos[K2]),   get<y>(pl.k_pos[K3]),   get<y>(pl.k_pos[K4]));

		const double z1 = arg_k5(self.planet.pos_z.back(), get<z>(self.k_pos[K1]), get<z>(self.k_pos[K2]), get<z>(self.k_pos[K3]), get<z>(self.k_pos[K4]));
		const double z2 = arg_k5(pl.planet.pos_z.back(),   get<z>(pl.k_pos[K1]),   get<z>(pl.k_pos[K2]),   get<z>(pl.k_pos[K3]),   get<z>(pl.k_pos[K4]));

		kx += phy::gravity_calc_x(pl.planet.mass, x1, x2, y1, y2, z1, z2);
		ky += phy::gravity_calc_y(pl.planet.mass, x1, x2, y1, y2, z1, z2);
		kz += phy::gravity_calc_z(pl.planet.mass, x1, x2, y1, y2, z1, z2);
	}

	return std::tuple{kx* h, ky* h, kz* h};
}

std::tuple<double, double, double> rk4::position_k5(const rk::Planet_solver& self, const double h) {
	const double kx = arg_k5(self.planet.speed_x.back(), get<x>(self.k_speed[K1]), get<x>(self.k_speed[K2]), get<x>(self.k_speed[K3]), get<x>(self.k_speed[K4]));
	const double ky = arg_k5(self.planet.speed_y.back(), get<y>(self.k_speed[K1]), get<y>(self.k_speed[K2]), get<y>(self.k_speed[K3]), get<y>(self.k_speed[K4]));
	const double kz = arg_k5(self.planet.speed_z.back(), get<z>(self.k_speed[K1]), get<z>(self.k_speed[K2]), get<z>(self.k_speed[K3]), get<z>(self.k_speed[K4]));
	return std::tuple{kx* h, ky* h, kz* h};
}

double rk4::arg_k5(const double param, const double k1, const double k2, const double k3, const double k4) {
	return param + k1*439./216. - k2*8. + k3*3680./531. - k4*845./4140.;
}

void rk4::next_step(std::vector<rk::Planet_solver>& ps,const double h) {

	//Calc k1
	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K1] = gravity_k1(pl,ps,h);
		pl.k_pos[K1] = position_k1(pl,h);
	}

	//Calc k2
	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K2] = gravity_k2(pl,ps,h);
		pl.k_speed[K2] = position_k2(pl,h);
	}

	//Calc k3
	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K3] = gravity_k3(pl,ps,h);
		pl.k_speed[K3] = position_k3(pl,h);
	}

	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K4] = gravity_k4(pl,ps,h);
		pl.k_speed[K4] = position_k4(pl,h);
	}

	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K5] = gravity_k5(pl,ps,h);
		pl.k_speed[K5] = position_k5(pl,h);
	}


	//calc the values
	for (auto&[planet, dead, k_pos, k_speed] : ps) {
		if (dead)
			continue;

		planet.pos_x.push_back(next_point(planet.pos_x.back(), get<x>(k_pos[K1]), get<x>(k_pos[K2]), get<x>(k_pos[K3]), get<x>(k_pos[K4]), get<x>(k_pos[K5])));
		planet.pos_y.push_back(next_point(planet.pos_y.back(), get<y>(k_pos[K1]), get<y>(k_pos[K2]), get<y>(k_pos[K3]), get<y>(k_pos[K4]), get<y>(k_pos[K5])));
		planet.pos_z.push_back(next_point(planet.pos_z.back(), get<z>(k_pos[K1]), get<z>(k_pos[K2]), get<z>(k_pos[K3]), get<z>(k_pos[K4]), get<z>(k_pos[K5])));

		planet.speed_x.push_back(next_point(planet.speed_x.back(), get<x>(k_speed[K1]), get<x>(k_speed[K2]), get<x>(k_pos[K3]), get<x>(k_pos[K4]), get<x>(k_pos[K5])));
		planet.speed_y.push_back(next_point(planet.speed_y.back(), get<y>(k_speed[K1]), get<y>(k_speed[K2]), get<y>(k_pos[K3]), get<y>(k_pos[K4]), get<y>(k_pos[K5])));
		planet.speed_z.push_back(next_point(planet.speed_z.back(), get<z>(k_speed[K1]), get<z>(k_speed[K2]), get<z>(k_pos[K3]), get<z>(k_pos[K4]), get<z>(k_pos[K5])));
	}

}

double rk4::next_point(const double w, const double k1, const double k2, const double k3, const double k4,const double k5) {
	return w + 25./216.*k1 + 0.*k2 + 1406./2565.*k3 + 2197./4104.*k4 - 1./5.*k5;
}

// *************   Metodi pubblici ********************************
void rk4::RK4_solver::next_step() {
	rk4::next_step(planets_solver,h);
}


void rk4::RK4_solver::runge_kutta_solver() {

	time.push_back(start_time);

	create_files();

	for (int i = 0; i * h + start_time < end_time; ++i) {

		++step_counter;

		time.push_back(i * h + start_time);
		next_step();

		handle_collision(n_param, data_keep);

		if (step_counter >= step_to_save) {
			save_data(data_keep);
		}

	}

	save_data(0);

}


// ~~~~~~~~~~~~~~~~ Costruttori ~~~~~~~~~~~~~~~~~~~~~~~~~~~

rk4::RK4_solver::RK4_solver(const std::vector<phy::Planet>& planets, const double start_time, const double end_time, const double step, const double collision_tollerrance) :
	RK4_solver(planets, start_time, end_time, step, collision_tollerrance, "", true, 100) {
}

rk4::RK4_solver::RK4_solver(const std::vector<phy::Planet>& planets, const double start_time, const double end_time, const double step, const double collision_tollerrance,
	std::string_view out_path, const bool save_all, const int step_to_next_save) {

	for (auto& pl : planets) {
		init_planets_solver(pl, n_param);
	}

	// Parametri simulazione
	this->h = step;
	this->start_time = start_time;
	this->end_time = end_time;
	this->collision_tollerrance = collision_tollerrance;


	// Parametri salvataggio dati
	set_save(out_path, save_all, step_to_next_save);

}

rk4::RK4_solver::~RK4_solver() {
	planets_solver.clear();
}
