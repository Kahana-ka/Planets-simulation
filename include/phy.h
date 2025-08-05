
#ifndef PHY
#define PHY

#include <cmath>
#include <vector>
#include <string>

namespace phy {

	// !=!=!=!=!=!=!=!=!=!=!=!=!=! Per ora settata ad 1, cambiare quando implemento la dimensionalita' !=!=!=!=!=!=!=!=!=!=!=!=!=!
	constexpr double G = 1;

	/** Struttura che contiene i valori di un pianeta, posizione velocita' massa raggio del pianeta ed un id.
	*/
	struct Planet {
		std::vector<double> pos_x;
		std::vector<double> pos_y;
		std::vector<double> pos_z;
		std::vector<double> speed_x;
		std::vector<double> speed_y;
		std::vector<double> speed_z;
		double mass;
		double radius;
		std::string id;
	};

	/** Setta le condizioni iniziali di un pianeta. fa un push_back su i vector inserendo le condizioni iniziali.
	*	@param Pianeta, andra a modificari i suoi campi
	*   @param x posizione x
	*   @param y posizione y
	*	@param z posizione z
	*	@param vx velocita lungo x
	*	@param vx velocita lungo y
	* 	@param vx velocita lungo z
	*	@param m massa pianeta
	*   @param r raggio pianeta
	*	@param id id del pianeta
	*/
	void set_ic(Planet&, double, double, double, double, double, double, double,double, std::string_view);

	/** Calcola la componente x della gravita' come accellerazione a = Gm/(sqrt((x1-x2)^2 + (y1-y2)^2))^3 * (x2-x1)
	* 	@param m
	*	@param x1
	*	@param x2
	*	@param y1
	*	@param y2
	*	@param z1
	*	@param z2
	*/
	double gravity_calc_x(const double, const  double, const  double, const  double, const  double, const  double, const  double);

	/** Calcola la componente y della gravita' come accellerazione a = Gm/(sqrt((x1-x2)^2 + (y1-y2)^2))^3 * (y2-y1)
	* 	@param m
	*	@param x1
	*	@param x2
	*	@param y1
	*	@param y2
	*	@param z1
	*	@param z2
	*/
	double gravity_calc_y(const double, const  double, const  double, const  double, const  double, const  double, const  double);

	/** Calcola la componente z della gravita' come accellerazione a = Gm/(sqrt((x1-x2)^2 + (y1-y2)^2))^3 * (z2-z1)
	*   @param m
	*	@param x1
	*	@param x2
	*	@param y1
	*	@param y2
	*	@param z1
	*	@param z2
	*/
	double gravity_calc_z(const double, const  double, const  double, const  double, const  double, const  double, const  double);
}

#endif // !PHY

