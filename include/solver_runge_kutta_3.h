
#ifndef SOLVER_RUNGE_KUTTA_3
#define SOLVER_RUNGE_KUTTA_3


#include <vector>
#include "phy.h"
#include <format>
#include <tuple>
#include "runge_kutta.h"

namespace rk3 {

	// Non so se bello o brutto, ma mi piace
	using rk::x;
	using rk::y;
	using rk::z;



	enum coeff {
		K1 = 0,
		K2 = 1,
		K3 = 2,
	};

	/// Calcola il valore dell'argomento di f per calcolare k3 (w+...)
	/// @param param Valore iniziale del parametro (w)
	/// @param k2 Valore del coefficiente k2
	/// @return valore argomento
	double arg_k3(double param,double k2);

	/// Dato un vettore di planet_solver calcola i parameri k3 per l'equazioni dell'accellerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k3x k3y k3z
	std::tuple<double, double, double> gravity_k3(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parameri k3 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k3x k3y k3z
	std::tuple<double, double, double> position_k3(const rk::Planet_solver& self, double h);

	/// Calcola il valore dell'argomento di f per calcolare k2 (w+...)
	/// @param param Valore iniziale del parametro (w)
	/// @param k1 Valore del coefficiente k1
	/// @return valore argomento
	double arg_k2(double param, double k1);

	/// Dato un vettore di planet_solver calcola i parameri k2 per l'equazioni dell'accellerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k2x k2y k2z
	std::tuple<double, double, double> gravity_k2(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parameri k2 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k2x k2y k2z
	std::tuple<double, double, double> position_k2(const rk::Planet_solver& self, double h);

	/// Dato un vettore di planet_solver calcola i parameri k1 per l'equazioni dell'accellerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k1x k1y k1z
	std::tuple<double, double, double> gravity_k1(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parameri k1 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k1x k1y k1z
	std::tuple<double, double, double> position_k1(const rk::Planet_solver& self, double h);

	/// Calcola il punto successivo dato il valore del parametro
	/// i coefficienti e grandezza dello step
	/// @param w Parametro corrente
	/// @param k1 coefficiente k1
	/// @param k3 coefficiente k2
	/// @param h grandezza step
	/// @return w al tempo successivo
	double next_point(double w, double k1, double k3, double h);


	/// Dato il vettore di planet_solver e uno step h fa i calcoli
	/// per il punto successivo e li mette in planets_solver.planet. ...
	/// @param ps Vector contenente il planet_solver per tutti i pianeti simulati
	/// @param h Step
	void next_step(std::vector<rk::Planet_solver>& ps, double h);


	/// Numero parametri che utilizza il modello
	constexpr int n_param = 3;

	//Quanti dati tenere ad ogni salvataggio
	constexpr int data_keep = 1;


	class RK3_solver : public rk::RK_base {

	private:
		void next_step();

	public:
		//Costruttore 
		RK3_solver(const std::vector<phy::Planet>&, double, double, double, double);
		RK3_solver(const std::vector<phy::Planet>&, double, double, double, double,std::string_view,bool,int);

		// Fa un clear del planets_solver
		~RK3_solver();

		/** Svolge la simulazione utilizzando runge kutta del terzo ordine
		* Salva i dati su file e utilizza lo step indicato nella classe come membro
		*/
		void runge_kutta_solver();

		double start_time_;
	};
	

}

#endif // !solver_runge_kutta_3

