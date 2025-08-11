
#include "solver_runge_kutta_5.h"


phy::Vector3 rk5::gravity_k1(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, const double h) {
	phy::Vector3 k{0.,0.,0.};

	//Viene fatta per tutti i pianeti meno se stesso.
	for (auto& pl : planets_solver) {
		if (pl.id == self.id || pl.dead)
			continue;

		k += phy::gravity_calc(pl.mass,self.position.back(),pl.position.back());
	}

	return k*h;
}
phy::Vector3 rk5::position_k1(const Planet_solver<6>& self, const double h) {
	return self.speed.back()*h;
}

//K2

phy::Vector3 rk5::gravity_k2(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, const double h) {

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : (planets_solver)) {
		if (pl.id == self.id || pl.dead)
			continue;

		auto p1 = arg_k2(self.position.back(),self.k_position);
		auto p2 = arg_k2(pl.position.back(),pl.k_position);

		k += phy::gravity_calc(pl.mass,p1,p2);

	}

	return k*h;
}
phy::Vector3 rk5::position_k2(const Planet_solver<6>& self, const double h) {
	return arg_k2(self.speed.back(),self.k_speed)*h;
}
phy::Vector3 rk5::arg_k2(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k) {
	return param + k[K1] / 4.;
}

//k3

phy::Vector3 rk5::gravity_k3(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, const double h) {

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : (planets_solver)) {
		if (pl.id == self.id || pl.dead)
			continue;

		auto p1 = arg_k3(self.position.back(),self.k_position);
		auto p2 = arg_k3(pl.position.back(),pl.k_position);

		k += phy::gravity_calc(pl.mass,p1,p2);

	}

	return k*h;
}
phy::Vector3 rk5::position_k3(const Planet_solver<6>& self, const double h) {
	return arg_k3(self.speed.back(),self.k_speed)*h;
}
phy::Vector3 rk5::arg_k3(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k) {
	return param + k[K1] / 32. * 3. +  k[K2] * 9. / 32.;
}

//k4

phy::Vector3 rk5::gravity_k4(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, const double h) {

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : (planets_solver)) {
		if (pl.id == self.id || pl.dead)
			continue;

		auto p1 = arg_k4(self.position.back(),self.k_position);
		auto p2 = arg_k4(pl.position.back(),pl.k_position);

		k += phy::gravity_calc(pl.mass,p1,p2);

	}

	return k*h;
}
phy::Vector3 rk5::position_k4(const Planet_solver<6>& self, const double h) {
	return arg_k4(self.speed.back(),self.k_speed)*h;
}
phy::Vector3 rk5::arg_k4(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k) {
	return param + k[K1]*1932./2197. - k[K2]*7200./2197. + k[K3]*7296./2197.;
}

//k5

phy::Vector3 rk5::gravity_k5(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, const double h) {

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : (planets_solver)) {
		if (pl.id == self.id || pl.dead)
			continue;

		auto p1 = arg_k5(self.position.back(),self.k_position);
		auto p2 = arg_k5(pl.position.back(),pl.k_position);

		k += phy::gravity_calc(pl.mass,p1,p2);

	}

	return k*h;
}
phy::Vector3 rk5::position_k5(const Planet_solver<6>& self, const double h) {
	return arg_k5(self.speed.back(),self.k_speed)*h;
}
phy::Vector3 rk5::arg_k5(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k) {
	return param + k[K1]*439./216. - k[K2]*8. + k[K3]*3680./531. - k[K4]*845./4140.;
}

//k6

phy::Vector3 rk5::gravity_k6(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, const double h) {

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : (planets_solver)) {
		if (pl.id == self.id || pl.dead)
			continue;

		auto p1 = arg_k6(self.position.back(),self.k_position);
		auto p2 = arg_k6(pl.position.back(),pl.k_position);

		k += phy::gravity_calc(pl.mass,p1,p2);

	}

	return k*h;
}
phy::Vector3 rk5::position_k6(const Planet_solver<6>& self, const double h) {
	return arg_k6(self.speed.back(),self.k_speed)*h;
}
phy::Vector3 rk5::arg_k6(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k) {
	return param - k[K1] * 8. / 27. + k[K2] * 2. - k[K3] * 3544. / 2565. + k[K4] * 1859. / 4104. - k[K5]*11./40.;
}



phy::Vector3 rk5::next_point(const phy::Vector3 &w, const std::array<phy::Vector3,6> &k) {
	return w + k[K1]*16./135. + k[K2]*0. + k[K3]*6656./12825. + k[K4]*28561./56430. - k[K5]*9./50. + k[K6]*2./55.;
}

void rk5::next_step(std::vector<Planet_solver<6>>& ps,const double h) {

	//Calc k1
	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K1] = gravity_k1(pl,ps,h);
		pl.k_position[K1] = position_k1(pl,h);

		pl.acceleration.push_back(pl.k_speed[K1]*h);
	}

	//Calc k2
	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K2] = gravity_k2(pl,ps,h);
		pl.k_position[K2] = position_k2(pl,h);
	}

	//Calc k3
	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K3] = gravity_k3(pl,ps,h);
		pl.k_position[K3] = position_k3(pl,h);
	}

	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K4] = gravity_k4(pl,ps,h);
		pl.k_position[K4] = position_k4(pl,h);
	}

	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K5] = gravity_k5(pl,ps,h);
		pl.k_position[K5] = position_k5(pl,h);
	}

	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K6] = gravity_k6(pl,ps,h);
		pl.k_position[K6] = position_k6(pl,h);
	}

	//calc the values
	for (auto& pl : ps) {
		if (pl.dead)
			continue;

		pl.position.push_back(next_point(pl.position.back(),pl.k_position) );
		pl.speed.push_back(next_point(pl.speed.back(),pl.k_speed) );
	}

}

// *************   Metodi pubblici ********************************

void rk5::RK5_solver::next_step() {
	rk5::next_step(planets_solver,h);
}


rk5::RK5_solver::~RK5_solver() {
	planets_solver.clear();
}
