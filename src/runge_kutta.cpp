
#include "../include/runge_kutta.h"

void rk::RK_base::init_planets_solver(const phy::Planet& pl, int n_param) {
	planets_solver.push_back(Planet_solver{ pl });
	for (int i = 0; i < n_param; i++) {
		planets_solver.back().k_pos.push_back(std::tuple<double, double, double>{0., 0., 0.});
		planets_solver.back().k_speed.push_back(std::tuple<double, double, double>{0., 0., 0.});
	}
}

// ~~~~~~~~~~~~~~~~~~~ Metodi per detectare una collisione

bool rk::RK_base::are_colliding(const phy::Planet& p1, const phy::Planet& p2) {
	
	double d = dist(p1.pos_x.back(), p2.pos_x.back(), p1.pos_y.back(), p2.pos_y.back(), p1.pos_z.back(), p2.pos_z.back());

	if (d <= p1.radius + collision_tollerrance || d <= p2.radius + collision_tollerrance) {
		return true;
	}

	return false;
}

double rk::RK_base::dist(double x1, double x2, double y1, double y2, double z1, double z2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}

// Da velocizzare

void rk::RK_base::handle_collision(int n_param,int to_keep) {

	std::vector<std::string_view> marked;


	for (auto& p1 : planets_solver) {

		if (p1.dead)
			continue;

		for (auto& p2 : planets_solver) {

			if (p2.dead)
				continue;

			if (p1.planet.id != p2.planet.id && are_colliding(p1.planet, p2.planet)) {

				// Forzo un save
				save_data(to_keep);

				// Faggo i pianeti morti
				p1.dead = true;
				p2.dead = true;

				//Creo il nuovo pianeta e lo aggiungo al solver
				phy::Planet new_planet = create_planet(p1.planet,p2.planet);
				init_planets_solver(new_planet, n_param);
				
				//creo il file 
				create_files(planets_solver.back().planet);

			}
		}
	}
}

phy::Planet rk::RK_base::create_planet(const phy::Planet& p1, const phy::Planet& p2) {
	phy::Planet new_planet;
	
	double radius{ (p1.radius + p2.radius) / 2 };
	double mass{ p1.mass + p2.mass };
	
	double pos_x{ (p1.pos_x.back() + p2.pos_x.back()) / 2 };
	double pos_y{ (p1.pos_y.back() + p2.pos_y.back()) / 2 };
	double pos_z{ (p1.pos_z.back() + p2.pos_z.back()) / 2 };
	
	double speed_x{ (p1.mass * p1.speed_x.back() + p2.mass * p2.speed_x.back()) / mass };
	double speed_y{ (p1.mass * p1.speed_y.back() + p2.mass * p2.speed_y.back()) / mass };
	double speed_z{ (p1.mass * p1.speed_z.back() + p2.mass * p2.speed_z.back()) / mass };

	std::string id = p1.id + p2.id;

	phy::set_ic(new_planet, pos_x, pos_y, pos_z, speed_x, speed_y, speed_z, mass, radius, id);

	return new_planet;
}

// ~~~~~~~~~~~~~~~~ Metodi salvare su file ~~~~~~~~~~~~~~~~~~~~~~~~~~~

void rk::RK_base::clear_vectors(int n_to_keep) {

	//Salvo e rimuovo l'ultimo elemento
	std::vector<double> t, x, y, z, vx, vy, vz;

	
	t = std::vector<double>(time.end()-n_to_keep,time.end());
	time.clear();
	time = t;

	for (auto& p : planets_solver) {

		
		x = std::vector<double>(p.planet.pos_x.end() - n_to_keep, p.planet.pos_x.end());
		y = std::vector<double>(p.planet.pos_y.end() - n_to_keep, p.planet.pos_y.end());
		z = std::vector<double>(p.planet.pos_z.end() - n_to_keep, p.planet.pos_z.end());
		vx = std::vector<double>(p.planet.speed_x.end() - n_to_keep, p.planet.speed_x.end());
		vy = std::vector<double>(p.planet.speed_y.end() - n_to_keep, p.planet.speed_y.end());
		vz = std::vector<double>(p.planet.speed_z.end() - n_to_keep, p.planet.speed_z.end());

		
		// Svuotamento vector
		p.planet.pos_x.clear();
		p.planet.pos_y.clear();
		p.planet.pos_z.clear();
		p.planet.speed_x.clear();
		p.planet.speed_y.clear();
		p.planet.speed_z.clear();

		// Inserimento dato salvato
		p.planet.pos_x = x;
		p.planet.pos_y = y;
		p.planet.pos_z = z;
		p.planet.speed_x = vx;
		p.planet.speed_y = vy;
		p.planet.speed_z = vz;
		
	}

}

void rk::RK_base::save_data(int n_to_keep) {
	//diventa implicitamente skip_last, dato che se tengo lultimo dato, quando gli scrivo questo non lo scrivo
	write_to_file(n_to_keep);
	clear_vectors(n_to_keep);
	step_counter = 0;
}

void rk::RK_base::write_to_file(int n_to_keep) {

	std::ofstream outf;

	for (auto& ps : planets_solver) {

		if (ps.dead)
			continue;

		outf.open(std::string(output_path) + ps.planet.id, std::ios_base::app);

		
		if(save_all_data)
			//Scrive tutti i dati
			for (int i = 0; i < time.size() - n_to_keep; i++) {
				write_element(ps.planet, i, outf);
			}
		else
			//scrive solo l'ultimo
			write_element(ps.planet, time.size()-1 - n_to_keep, outf);

		outf.close();
	}

}

void rk::RK_base::write_element(const phy::Planet& p, int i, std::ofstream& outf) {
	outf << time[i] << " ";
	outf << p.pos_x[i] << " ";
	outf << p.pos_y[i] << " ";
	outf << p.pos_z[i] << " ";
	outf << p.speed_x[i] << " ";
	outf << p.speed_y[i] << " ";
	outf << p.speed_z[i] << '\n';
}

void rk::RK_base::create_files() {

	std::ofstream outf;

	for (auto& p : planets_solver) {
		outf.open(std::string{ output_path } + p.planet.id);
		outf << "#time position_x position_y position_z speed_x speed_y speed_z\n";
		outf.close();
	}
}

void rk::RK_base::create_files(const phy::Planet planet) {
	std::ofstream outf;

	outf.open(std::string{ output_path } + planet.id);
	outf << "#time position_x position_y position_z speed_x speed_y speed_z\n";

	outf.close();
}

void rk::RK_base::set_save(std::string_view output_path, bool save_all_data, int step_to_next_save) {
	this->save_all_data = save_all_data;
	this->step_to_save = step_to_next_save;
	this->output_path = output_path;
}