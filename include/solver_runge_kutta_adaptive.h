#ifndef SOLVER_RUNGE_KUTTA_ADAPT
#define SOLVER_RUNGE_KUTTA_ADAPT

#include "solver_runge_kutta_4.h"
#include "solver_runge_kutta_5.h"
#include "Solver_base.h"
#include "phy.h"

namespace rk_add {


	constexpr int data_keep = 1;
	constexpr int n_param = 7;

	class RK_adaptive : public rk::RK_base {
		
	private:
		double h_min;
		double toll;

		double calc_q(const std::vector<rk::Planet_solver>&);
		double q_formula(double w1, double w2);
	public:
		//Costruttore 
		RK_adaptive(const std::vector<phy::Planet>&, double, double,double,double, double, double);
		RK_adaptive(const std::vector<phy::Planet>&, double, double,double, double,double, double, std::string_view, bool, int);

		// Fa un clear del planets_solver
		~RK_adaptive();

		/** Calcola lo step succesivo, l'overload utilizza uno
		*   step differente da quello disegnato nella classe.
		*/
		void next_step();

		/** Svolge la simulazione utilizzando runge kutta del terzo ordine
		* Salva i dati su file e utilizza lo step indicato nella classe come membro
		*/
		void runge_kutta_solver();

	};
}

#endif