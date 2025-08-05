#ifndef SOLVER_RUNGE_KUTTA_5
#define SOLVER_RUNGE_KUTTA_5

#include <vector>
#include "phy.h"
#include <format>
#include <tuple>
#include "runge_kutta.h"

namespace rk5 {

	// Non so se bello o brutto
	using rk::x;
	using rk::y;
	using rk::z;

	enum coeff {
		K1 = 0,
		K2 = 1,
		K3 = 2,
		K4 = 3,
		K5 = 4,
		K6 = 5,
	};

	//Numero parametri
	constexpr int n_param = 6;

	//Quanti dati tenere a ogni salvataggio
	constexpr int data_keep = 1;

	/// Dato un vettore di planet_solver calcola i parametri k6 per l'equazioni dell'accelerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k6x k6y k6z
	std::tuple<double, double, double> gravity_k6(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parametri k6 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k6x k6y k6z
	std::tuple<double, double, double> position_k6(const rk::Planet_solver& self, double h);

	/// Calcola il valore dell'argomento di f per calcolare k2 (w+...)
	/// @param param Valore iniziale del parametro (w)
	/// @param k1 Valore del coefficiente k1
	/// @param k2 Valore del coefficiente k2
	/// @param k3 Valore del coefficiente k3
	/// @param k4 Valore del coefficiente k4
	/// @param k5 Valore del coefficiente k5
	/// @return Valore argomento
	double arg_k6(double param,  double k1, double k2, double k3, double k4, double k5);

	/// Dato un vettore di planet_solver calcola i parametri k5 per l'equazioni dell'accelerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k5x k5y k5z
	std::tuple<double, double, double> gravity_k5(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parametri k5 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k5x k5y k5z
	std::tuple<double, double, double> position_k5(const rk::Planet_solver& self, double h);

	/// Calcola il valore dell'argomento di f per calcolare k2 (w+...)
	/// @param param Valore iniziale del parametro (w)
	/// @param k1 Valore del coefficiente k1
	/// @param k2 Valore del coefficiente k2
	/// @param k3 Valore del coefficiente k3
	/// @param k4 Valore del coefficiente k4
	/// @return Valore argomento
	double arg_k5(double param, double k1, double k2, double k3, double k4);

	/// Dato un vettore di planet_solver calcola i parametri k4 per l'equazioni dell'accelerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k4x k4y k4z
	std::tuple<double, double, double> gravity_k4(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parametri k4 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k4x k4y k4z
	std::tuple<double, double, double> position_k4(const rk::Planet_solver& self, double h);

	/// Calcola il valore dell'argomento di f per calcolare k2 (w+...)
	/// @param param Valore iniziale del parametro (w)
	/// @param k1 Valore del coefficiente k1
	/// @param k2 Valore del coefficiente k2
	/// @param k3 Valore del coefficiente k3
	/// @return Valore argomento
	double arg_k4(double param, double k1,double k2,double k3);

	/// Dato un vettore di planet_solver calcola i parametri k3 per l'equazioni dell'accelerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k3x k3y k3z
	std::tuple<double, double, double> gravity_k3(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parametri k3 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k3x k3y k3z
	std::tuple<double, double, double> position_k3(const rk::Planet_solver& self, double h);

	/// Calcola il valore dell'argomento di f per calcolare k2 (w+...)
	/// @param param Valore iniziale del parametro (w)
	/// @param k1 Valore del coefficiente k1
	/// @param k2 Valore del coefficiente k2
	/// @return Valore argomento
	double arg_k3(double param, double k1, double k2);

	/// Dato un vettore di planet_solver calcola i parametri k2 per l'equazioni dell'accelerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k2x k2y k2z
	std::tuple<double, double, double> gravity_k2(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parametri k2 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k2x k2y k2z
	std::tuple<double, double, double> position_k2(const rk::Planet_solver& self, double h);

	/// Calcola il valore dell'argomento di f per calcolare k2 (w+...)
	/// @param param Valore iniziale del parametro (w)
	/// @param k1 Valore del coefficiente k1
	/// @return Valore argomento
	double arg_k2(double param, double k1);

	/// Dato un vettore di planet_solver calcola i parametri k1 per l'equazioni dell'accelerazione
	/// Nei tre assi
	/// @param self Pianeta per cui calcolare i k
	/// @param planets_solver Vector con tutti i pianeti
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k1x k1y k1z
	std::tuple<double, double, double> gravity_k1(const rk::Planet_solver& self,const std::vector<rk::Planet_solver>& planets_solver, double h);

	/// Calcola i parametri k1 per l'equazioni della posizione nei tre assi
	/// Non dipende dagli altri pianeti quindi basta se stesso
	/// @param self Pianeta per cui calcolare i k
	/// @param h Grandezza dello step
	/// @return tupla con i valori di k1x k1y k1z
	std::tuple<double, double, double> position_k1(const rk::Planet_solver& self, double h);

	/// Calcola il punto successivo dato il valore del parametro
	/// i coefficienti e grandezza dello step
	/// @param w Parametro corrente
	/// @param k1 coefficiente k1
	/// @param k2 coefficiente k2
	/// @param k3 coefficiente k3
	/// @param k4 coefficiente k4
	/// @param k5 coefficiente k5
	/// @param k6 coefficiente k6
	/// @return w al tempo successivo
	double next_point(double w, double k1, double k2, double k3, double k4,double k5, double k6);

	/// Dato il vettore di planet_solver e uno step h fa i calcoli
	/// per il punto successivo e li mette in planets_solver.planet. ...
	/// @param ps Vector contenente il planet_solver per tutti i pianeti simulati
	/// @param h Step
	void next_step(std::vector<rk::Planet_solver>& ps,double h);


	class RK5_solver : public rk::RK_base {
	private:
		void next_step();

	public:
		//Costruttore 
		RK5_solver(const std::vector<phy::Planet>&, double, double, double, double);
		RK5_solver(const std::vector<phy::Planet>&, double, double, double, double, std::string_view, bool, int);

		// Fa un clear del planets_solver
		~RK5_solver();

		void runge_kutta_solver();
	};


}

#endif // !SOLVER_RUNGE_KUTTA_5

