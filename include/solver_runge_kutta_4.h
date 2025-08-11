#ifndef SOLVER_RUNGE_KUTTA_4
#define SOLVER_RUNGE_KUTTA_4


#include <vector>
#include "phy.h"
#include <format>
#include <tuple>
#include "runge_kutta.h"

namespace rk4 {


	enum Coefficient_index {
		K1 = 0,
		K2 = 1,
		K3 = 2,
		K4 = 3,
	};


	//Quanti dati tenere a ogni salvataggio


	phy::Vector3 gravity_k4(const Planet_solver<4>& self,const std::vector<Planet_solver<4>>& planets_solver, double h);
	phy::Vector3 position_k4(const Planet_solver<4>& self, double h);
	phy::Vector3 arg_k4(const phy::Vector3& param , const phy::Vector3& k3);

	phy::Vector3 gravity_k3(const Planet_solver<4>& self,const std::vector<Planet_solver<4>>& planets_solver, double h);
	phy::Vector3 position_k3(const Planet_solver<4>& self, double h);
	phy::Vector3 arg_k3(const phy::Vector3& param , const phy::Vector3& k2);


	phy::Vector3 gravity_k2(const Planet_solver<4>& self,const std::vector<Planet_solver<4>>& planets_solver, double h);
	phy::Vector3 position_k2(const Planet_solver<4>& self, double h);
	phy::Vector3 arg_k2(const phy::Vector3& param , const phy::Vector3& k1);

	phy::Vector3 gravity_k1(const Planet_solver<4>& self,const std::vector<Planet_solver<4>>& planets_solver, double h);
	phy::Vector3 position_k1(const Planet_solver<4>& self, double h);

	phy::Vector3 next_point(const phy::Vector3 &w, const std::array<phy::Vector3,4> &param);

	void next_step(std::vector<Planet_solver<4>>& ps, double h);


	class RK4_solver : public Solver_base<4> {
	private:
		static constexpr int data_keep = 1;
		void next_step() override;

	public:
		//Costruttore
		RK4_solver(const std::vector<phy::Planet>& planets,double step,double start_time,double end_time,double collision_tolerance)
								: Solver_base(planets,step,start_time,end_time,collision_tolerance) {}

		// Fa un clear del planets_solver
		~RK4_solver() override;
	};


}

#endif // !SOLVER_RUNGE_KUTTA_4
