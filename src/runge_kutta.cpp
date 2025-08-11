
#include "../include/runge_kutta.h"

#include <algorithm>

#include "solver_runge_kutta_3.h"
#include "solver_runge_kutta_4.h"
#include "solver_runge_kutta_adaptive.h"

// Init planet solver

template<int size>
void Solver_base<size>::init_planets_solver(const std::vector<phy::Planet>& pl) {
	planets_solver = new std::vector<Planet_solver<size>>();

	for (const auto& p : pl) {
		add_planet(p);
	}
}

template<int size>
void Solver_base<size>::add_planet(const phy::Planet& p) {
	planets_solver.push_back(Planet_solver<size>{});

	planets_solver.back().position = p.position;
	planets_solver.back().speed = p.speed;
	planets_solver.back().acceleration = p.acceleration;
	planets_solver.back().mass = p.mass;
	planets_solver.back().radius = p.radius;
	planets_solver.back().id = p.id;

	for (int i = 0; i < size; i++) {
		planets_solver.back().k_position[i] = {0.,0.,0.};
		planets_solver.back().k_speed[i] = {0.,0.,0.};
	}
}


// ~~~~~~~~~~~~~~~~~~~ Metodi per detectare una collisione
template<int size>
bool Solver_base<size>::are_colliding(const Planet_solver<size>& p1, const Planet_solver<size>& p2) const{

	double d = p1.position.back().distance(p2.position.back());

	if (d <= p1.radius + collision_tolerance || d <= p2.radius + collision_tolerance) {
		return true;
	}

	return false;
}

// Da velocizzare
template<int size>
void Solver_base<size>::handle_collision() {

	for (auto current = planets_solver.begin(); current != planets_solver.end(); ++current) {
		if (current->dead)
			continue;
		for (auto planet = planets_solver.begin(); planet != planets_solver.end(); ++planet) {
			if (planet->dead)
				continue;

			if (planet != current && are_colliding(*planet,*current)) {

				// Forzo un save
				save_data(false);

				//Creo il nuovo pianeta e lo aggiungo al solver
				phy::Planet new_planet = merge_planet(*planet,*current);
				add_planet(new_planet);

				//creo il file
				create_file(planets_solver.back());


				// Gli flaggo
				current->dead = true;
				planet->dead = true;
			}
		}
	}

	remove_dead_planets();
}

template<int size>
phy::Planet Solver_base<size>::merge_planet(const Planet_solver<size>& p1, const Planet_solver<size>& p2) {

	phy::Planet new_planet;

	double radius{ (p1.radius + p2.radius) / 2 };
	double mass{ p1.mass + p2.mass };

	phy::Vector3 position {(p1.position.back() + p2.position.back())/2};
	phy::Vector3 speed{(p1.speed.back()* p1.mass + p2.speed.back() * p2.mass)/mass};

	// TODO da rifare
	std::string id = p1.id + p2.id;

	phy::set_ic(new_planet, position,speed, mass, radius, id);

	return new_planet;
}

// ~~~~~~~~~~~~~~~~ Metodi salvare su file ~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<int size>
void Solver_base<size>::clear_vectors(const bool all_data) {

	const auto t = std::vector(time.end() - data_to_keep, time.end());
	time.clear();

	if (!all_data) {
		time = t;
	}

	for (auto& p : planets_solver) {

		auto position = std::vector(p.position.end() - data_to_keep,p.position.end());
		auto speed = std::vector(p.speed.end()-data_to_keep,p.speed.end());

		p.position.clear();
		p.speed.clear();

		if (!all_data) {
			p.position = position;
			p.speed = speed;
		}
	}
}

// Salva tutti i dati
template<int size>
void Solver_base<size>::save_data() {
	save_data(true);
}

template<int size>
void Solver_base<size>::save_data(bool all_data) {
	write_to_file(all_data);
	clear_vectors(all_data);
}

template<int size>
void Solver_base<size>::set_next_save_time() {
	next_save_time += save_every_s;
}

template<int size>
void Solver_base<size>::write_to_file(bool all_data) {

	std::ofstream outf;
	int to_keep = data_to_keep;
	for (auto& ps : planets_solver) {

		outf.open(std::string(output_path) + ps.id, std::ios_base::app);

		if (ps.dead || all_data)
			to_keep = 0;
		else
			to_keep = data_to_keep;

		if(save_all_data)
			//Scrive tutti i dati
			for (int i = 0; i < time.size() - to_keep; i++) {
				write_element(ps, i, outf);
			}
		else
			//scrive solo l'ultimo
			write_element(ps, time.size()-1 - to_keep, outf);

		outf.close();
	}

}

template<int size>
void Solver_base<size>::write_element(const Planet_solver<size>& p, int index, std::ofstream& outf) {
	outf << time[index] << " ";
	outf << p.position[index].x << " ";
	outf << p.position[index].y << " ";
	outf << p.position[index].z << " ";
	outf << p.speed[index].x << " ";
	outf << p.speed[index].y << " ";
	outf << p.speed[index].z << " ";
	outf << p.acceleration[index].x << " ";
	outf << p.acceleration[index].y << " ";
	outf << p.acceleration[index].z << "\n";
}

template<int size>
void Solver_base<size>::create_files() {

	for (auto& p : planets_solver) {
		 create_file(p);
	}
}

template<int size>
void Solver_base<size>::create_file(const Planet_solver<size>& planet) const{
	std::ofstream outf;

	outf.open(std::string{ output_path } + planet.id);
	outf << "#time position_x position_y position_z speed_x speed_y speed_z acceleration_x acceleration_y acceleration_z\n";

	outf.close();
}

template<int size>
void Solver_base<size>::set_save(const std::string_view output_path, const bool save_all_data, const int next_save_time) {
	this->save_all_data = save_all_data;
	this->next_save_time = next_save_time;
	this->output_path = output_path;
}

template<int size>
void Solver_base<size>::remove_dead_planets() {
	auto it = std::find_if(planets_solver.begin(), planets_solver.end(),[](const Planet_solver<size>& p) {return !p.dead;});
	while (it != planets_solver.end()) {
		planets_solver.erase(it);
		it = std::find_if(planets_solver.begin(), planets_solver.end(),[](const Planet_solver<size>& p) {return !p.dead;});
	}
}


template<int size>
Solver_base<size>::Solver_base(const std::vector<phy::Planet>& planets,double step,double start_time,double end_time,double collision_tolerance)
								: h{step},start_time{start_time},end_time{end_time},collision_tolerance{collision_tolerance} {
	init_planets_solver(planets);
}

template<int size>
void Solver_base<size>::solve() {

	time.push_back(start_time);
	create_files();

	for (int i = 0; i * h + start_time < end_time; ++i) {

		time.push_back(i * h + start_time);

		//Deve essere implementato da ogni sotto classe
		next_step();

		//Puo essere ridefinito
		handle_collision();

		if (  time.back() >= next_save_time ) {
			save_data(false);
			set_next_save_time();
		}
	}

	save_data(true);
}















