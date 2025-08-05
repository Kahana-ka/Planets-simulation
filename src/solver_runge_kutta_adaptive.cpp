#include "solver_runge_kutta_adaptive.h"
#include <iostream>
#include "solver_runge_kutta_3.h"

void rk_add::RK_adaptive::runge_kutta_solver() {
	std::vector<rk::Planet_solver> planets_ctrl{ };

	time.push_back(start_time);

	create_files();

	double curr_h = h;
	for (int i = 0; i * curr_h + start_time < end_time; ++i) {

		++step_counter;

		// Brutto, copia
		planets_ctrl = planets_solver;

		rk4::next_step(planets_solver,h);

		rk5::next_step(planets_ctrl,h);


		if (const double q{ calc_q(planets_ctrl) }; q < 1) {

			//elimini ultimo elemento prima del ricalcolo
			for (auto& p : planets_solver) {
				if (p.dead)
					continue;

				p.planet.pos_x.pop_back();
				p.planet.pos_y.pop_back();
				p.planet.pos_z.pop_back();
				p.planet.speed_x.pop_back();
				p.planet.speed_y.pop_back();
				p.planet.speed_z.pop_back();
			}

			if (q * h > h_min) {
				curr_h = q*h;
			} else {
				curr_h = h_min;
			}

			rk4::next_step(planets_solver,curr_h);
			time.push_back(i * curr_h + start_time);
		} else {
			curr_h = h;
			time.push_back(i * h + start_time);
		}

		handle_collision(n_param, data_keep);

		if (step_counter >= step_to_save) {
			save_data(data_keep);
		}
	}

	save_data(0);

}

double rk_add::RK_adaptive::calc_q(const std::vector<rk::Planet_solver>& planets_ctrl) {
	double q = 0;
	int sum = 0;

	for (int i = 0; i < planets_solver.size(); i++) {
		q += q_formula(planets_solver[i].planet.pos_x.back(), planets_ctrl[i].planet.pos_x.back());

		q += q_formula(planets_solver[i].planet.pos_y.back(), planets_ctrl[i].planet.pos_y.back());

		q += q_formula(planets_solver[i].planet.pos_z.back(), planets_ctrl[i].planet.pos_z.back());

		q += q_formula(planets_solver[i].planet.speed_x.back(), planets_ctrl[i].planet.speed_x.back());

		q += q_formula(planets_solver[i].planet.speed_y.back(), planets_ctrl[i].planet.speed_y.back());

		q += q_formula(planets_solver[i].planet.speed_z.back(), planets_ctrl[i].planet.speed_z.back());

		sum += 6;
	}

	return q/sum;
}

double rk_add::RK_adaptive::q_formula(double w1,double w2) {

	double q{ 0.84 * std::pow(toll * h / std::fabs(w1 - w2), 1. / 4.) };

	if (std::isinf(q)) {

		return 0.;
	}
	return q;
}

// ~~~~~~~~~~~~~~~~ Costruttori ~~~~~~~~~~~~~~~~~~~~~~~~~~~

rk_add::RK_adaptive::RK_adaptive(const std::vector<phy::Planet>& planets, const double start_time, const double end_time, const double step, const double step_min, const double toll, const double collision_tollerrance) :
	RK_adaptive(planets, start_time, end_time, step,step_min,toll, collision_tollerrance, "", true, 100) {
}

rk_add::RK_adaptive::RK_adaptive(const std::vector<phy::Planet>& planets, const double start_time, const double end_time, const double step, const double step_min, const double toll, const double collision_tollerrance,
	std::string_view out_path, const bool save_all, const int step_to_next_save) {

	for (auto& pl : planets) {
		init_planets_solver(pl, n_param);
	}

	// Parametri simulazione
	this->h = step;
	this->h_min = step_min;
	this->start_time = start_time;
	this->end_time = end_time;
	this->collision_tollerrance = collision_tollerrance;
	this->toll = toll;

	// Parametri salvataggio dati
	set_save(out_path, save_all, step_to_next_save);

}

rk_add::RK_adaptive::~RK_adaptive() {
	planets_solver.clear();
}
