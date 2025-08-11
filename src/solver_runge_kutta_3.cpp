
#include "../include/solver_runge_kutta_3.h"

// *************   Funzioni nel namespace ********************************

// ~~~~~~~~~~~~~~~  Funzioni per calcolare i k ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//K1

phy::Vector3 rk3::gravity_k1(const Planet_solver<3>& self,const std::vector<Planet_solver<3>>& planets_solver,const double h) {

	phy::Vector3 k{0.,0.,0.};


	for (auto& pl : planets_solver) {
		if (pl.id == self.id || pl.dead)
			continue;
		k += phy::gravity_calc(pl.mass,self.position.back(),pl.position.back());
	}

	return k*h;
}

phy::Vector3 rk3::position_k1(const Planet_solver<3>& self,const double h) {
	return self.speed.back() * h;
}

//K2

phy::Vector3 rk3::gravity_k2(const Planet_solver<3>& self,const std::vector<Planet_solver<3>>& planets_solver,const double h) {
	
	using std::get;

	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : (planets_solver)) {
		if (pl.id == self.id || pl.dead)
			continue;

		const auto p1 = arg_k2(self.position.back(),self.k_position[K1]);
		const auto p2 = arg_k2(pl.position.back(),pl.k_position[K1]);
		k += phy::gravity_calc(pl.mass,p1,p2);
	}

	return k*h;
}

phy::Vector3 rk3::position_k2(const Planet_solver<3>& self, const double h) {
	return arg_k2(self.position.back(),self.k_position[K1]) * h;
}

phy::Vector3 rk3::arg_k2(const phy::Vector3 &param , const phy::Vector3 &k1) {
	return (param + k1) / 3.;
}

//k3

phy::Vector3 rk3::gravity_k3(const Planet_solver<3>& self,const std::vector<Planet_solver<3>>& planets_solver,const double h) {
	
	phy::Vector3 k{0.,0.,0.};

	for (auto& pl : planets_solver) {
		if (pl.id == self.id || pl.dead)
			continue;

		const auto p1 = arg_k3(self.position.back(),self.k_position[K2]);
		const auto p2 = arg_k3(pl.position.back(),pl.k_position[K2]);
		k += phy::gravity_calc(pl.mass,p1,p2);

	}

	return k*h;
}

phy::Vector3 rk3::position_k3(const Planet_solver<3>& self, const double h) {
	return arg_k3(self.position.back(),self.k_position[K2]) * h;
}

phy::Vector3 rk3::arg_k3(const phy::Vector3 &param , const phy::Vector3 &k2) {
	return (param + k2) / (3.*2);
}

// ~~~~~~~~~~~ Funzioni per calcolare lo step successivo

phy::Vector3 rk3::next_point(const phy::Vector3 &w, const std::array<phy::Vector3,3> &param , const double h) {
	return w + ( param[K1] / h + param[K3] * 3.) * h /4.;
}

void rk3::next_step(std::vector<Planet_solver<3>>& ps,const double h) {

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

	//calc the values
	for (auto& pl : ps) {
		if (pl.dead)
			continue;

		pl.position.push_back(next_point(pl.position.back(),pl.k_position, h) );
		pl.speed.push_back(next_point(pl.speed.back(),pl.k_speed, h) );
	}

}



void rk3::RK3_solver::next_step(){
	rk3::next_step(planets_solver,h);
}


rk3::RK3_solver::~RK3_solver() {
	planets_solver.clear();
}



