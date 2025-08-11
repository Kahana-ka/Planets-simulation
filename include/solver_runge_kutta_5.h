#ifndef SOLVER_RUNGE_KUTTA_5
#define SOLVER_RUNGE_KUTTA_5

#include <vector>
#include "phy.h"
#include <format>
#include <tuple>
#include "runge_kutta.h"

namespace rk5 {


	enum Coefficient_index {
		K1 = 0,
		K2 = 1,
		K3 = 2,
		K4 = 3,
		K5 = 4,
		K6 = 5,
	};

	phy::Vector3 gravity_k6(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, double h);
	phy::Vector3 position_k6(const Planet_solver<6>& self, double h);
	phy::Vector3 arg_k6(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k);

	phy::Vector3 gravity_k5(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, double h);
	phy::Vector3 position_k5(const Planet_solver<6>& self, double h);
	phy::Vector3 arg_k5(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k);


	phy::Vector3 gravity_k4(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, double h);
	phy::Vector3 position_k4(const Planet_solver<6>& self, double h);
	phy::Vector3 arg_k4(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k);


	phy::Vector3 gravity_k3(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, double h);
	phy::Vector3 position_k3(const Planet_solver<6>& self, double h);
	phy::Vector3 arg_k3(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k);


	phy::Vector3 gravity_k2(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, double h);
	phy::Vector3 position_k2(const Planet_solver<6>& self, double h);
	phy::Vector3 arg_k2(const phy::Vector3 &param, const std::array<phy::Vector3,6> &k);


	phy::Vector3 gravity_k1(const Planet_solver<6>& self,const std::vector<Planet_solver<6>>& planets_solver, double h);
	phy::Vector3 position_k1(const Planet_solver<6>& self, double h);


	phy::Vector3 next_point(const phy::Vector3 &w, const std::array<phy::Vector3,6> &k);

	void next_step(std::vector<Planet_solver<6>>& ps, double h);


	class RK5_solver : public Solver_base<6> {
	private:
		void next_step() override;
		static constexpr int data_keep = 1;

	public:
		//Costruttore 
		RK5_solver(const std::vector<phy::Planet>& planets,double step,double start_time,double end_time,double collision_tolerance)
								: Solver_base(planets,step,start_time,end_time,collision_tolerance) {}

		// Fa un clear del planets_solver
		~RK5_solver() override;

	};


}

#endif // !SOLVER_RUNGE_KUTTA_5

