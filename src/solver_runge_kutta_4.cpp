
#include "solver_runge_kutta_4.h"

// *************   Metodi privati ********************************

// ~~~~~~~~~~~~~~~ Metodi per calcolare i k ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//K1

phy::Vector3 rk4::gravity_k1(const Planet_solver<4>& self,const std::vector<Planet_solver<4>>& planets_solver, const double h) {

	phy::Vector3 k{0.,0.,0.};

	//Viene fatta per tutti i pianeti meno se stesso.
	for (auto& pl : planets_solver) {
		if (pl.id == self.id || pl.dead)
			continue;

		k += phy::gravity_calc(pl.mass,self.position.back(),pl.position.back());
	}

	return k*h;
}

phy::Vector3 rk4::position_k1(const Planet_solver<4>& self, const double h) {
	return self.speed.back()*h;
}

//K2

phy::Vector3 rk4::gravity_k2(const Planet_solver<4>& self,const std::vector<Planet_solver<4>>& planets_solver, const double h) {

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : (planets_solver)) {
		if (pl.id == self.id || pl.dead)
			continue;

		auto p1 = arg_k2(self.position.back(),self.k_position[K1]);
		auto p2 = arg_k2(pl.position.back(),pl.k_position[K1]);

		k += phy::gravity_calc(pl.mass,p1,p2);

	}

	return k*h;
}

phy::Vector3 rk4::position_k2(const Planet_solver<4>& self, const double h) {
	return arg_k2(self.speed.back(),self.k_speed[K1])*h;
}



phy::Vector3  rk4::arg_k2(const phy::Vector3& param , const phy::Vector3& k1) {
	return param + k1 / 2.;
}

//k3

phy::Vector3 rk4::gravity_k3(const Planet_solver<4>& self,const std::vector<Planet_solver<4>>& planets_solver, const double h){

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : planets_solver) {
		if (pl.id == self.id || pl.dead)
			continue;

		auto p1 = arg_k3(self.position.back(),self.k_position[K2]);
		auto p2 = arg_k3(pl.position.back(),pl.k_position[K2]);

		k += phy::gravity_calc(pl.mass,p1,p2);
	}

	return k*h;
}

phy::Vector3 rk4::position_k3(const Planet_solver<4>& self, const double h) {
	return arg_k2(self.speed.back(),self.k_speed[K2])*h;
}


phy::Vector3  rk4::arg_k3(const phy::Vector3& param , const phy::Vector3& k2) {
	return param + k2 /2.;
}

//k4

phy::Vector3 rk4::gravity_k4(const Planet_solver<4>& self,const std::vector<Planet_solver<4>>& planets_solver, const double h) {

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : planets_solver) {
		if (pl.id == self.id || pl.dead)
			continue;

		auto p1 = arg_k3(self.position.back(),self.k_position[K3]);
		auto p2 = arg_k3(pl.position.back(),pl.k_position[K3]);

		k += phy::gravity_calc(pl.mass,p1,p2);
	}

	return k*h;
}

phy::Vector3 rk4::position_k4(const Planet_solver<4>& self, const double h) {
	return arg_k2(self.speed.back(),self.k_speed[K3])*h;
}


phy::Vector3 rk4::arg_k4(const phy::Vector3& param , const phy::Vector3& k3) {
	return param + k3;
}


void rk4::next_step(std::vector<Planet_solver<4>>& ps,const double h) {

	//Calc k1
	for (auto& pl : ps) {

		if (pl.dead)
			continue;

		pl.k_speed[K1] = gravity_k1(pl,ps,h);
		pl.k_position[K1] = position_k1(pl,h);
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



	//calc the values
	for (auto& pl : ps) {
		if (pl.dead)
			continue;

		pl.position.push_back(next_point(pl.position.back(),pl.k_position) );
		pl.speed.push_back(next_point(pl.speed.back(),pl.k_speed) );
	}

}


phy::Vector3 rk4::next_point(const phy::Vector3 &w, const std::array<phy::Vector3,4> &param) {
	return w + (param[K1] + param[K2]*2. + param[K3]*2. + param[K4])/6.;
}

// *************   Metodi pubblici ********************************
void rk4::RK4_solver::next_step() {
	rk4::next_step(planets_solver,h);
}

rk4::RK4_solver::~RK4_solver() {
	planets_solver.clear();
}
