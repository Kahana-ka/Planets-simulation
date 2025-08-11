
#ifndef SOLVER_RUNGE_KUTTA_3
#define SOLVER_RUNGE_KUTTA_3


#include <vector>
#include "phy.h"
#include <format>
#include <tuple>
#include "runge_kutta.h"

namespace rk3 {


	enum coeff {
		K1 = 0,
		K2 = 1,
		K3 = 2,
	};


	phy::Vector3 arg_k3(const phy::Vector3& param , const phy::Vector3& k2);
	phy::Vector3 gravity_k3(const Planet_solver<3>& self,const std::vector<Planet_solver<3>>& planets_solver, double h);
	phy::Vector3 position_k3(const Planet_solver<3>& self, double h);


	phy::Vector3 arg_k2(const phy::Vector3 &param , const phy::Vector3 &k1);
	phy::Vector3 gravity_k2(const Planet_solver<3>& self,const std::vector<Planet_solver<3>>& planets_solver, double h);
	phy::Vector3 position_k2(const Planet_solver<3>& self, double h);


	phy::Vector3 gravity_k1(const Planet_solver<3>& self,const std::vector<Planet_solver<3>>& planets_solver, double h);
	phy::Vector3 position_k1(const Planet_solver<3>& self, double h);

	phy::Vector3 next_point(const phy::Vector3 &w, const std::array<phy::Vector3,3> &param , double h);


	void next_step(std::vector<Planet_solver<3>>& ps, double h);


	class RK3_solver : public Solver_base<3> {

		void next_step() override;

		//Quanti dati tenere a ogni salvataggio
		static constexpr int data_keep = 1;

	public:
		//Costruttore 
		RK3_solver(const std::vector<phy::Planet>& planets,double step,double start_time,double end_time,double collision_tolerance)
			: Solver_base(planets,step,start_time,end_time,collision_tolerance) {}

		~RK3_solver() override;
	};
	

}

#endif // !solver_runge_kutta_3

