#ifndef SOLVER_RUNGE
#define SOLVER_RUNGE

#include "phy.h"
#include <vector>
#include <fstream>



	/** Class that extend planet struct adding the parameters vector
	 * No parameters for acceleration
	 * if a planet dosent need other update is marked with the dead flag
	*/

	template<int size>
	struct Planet_solver : phy::Planet {
		bool dead{ false };
		std::array<phy::Vector3,size> k_position;
		std::array<phy::Vector3,size> k_speed;
	};

	template <int size>
	class Solver_base {
	protected:

		//##################### ATTRIBUTI ########################

		std::vector<Planet_solver<size>>& planets_solver{ };
		std::vector<double> time{ }; //!< contiene il tempo t dello step, le equazioni sono tutte indipendenti da t.

		// Parametri simulazione
		const double h {0.};                     //!< grandezza step
		const double start_time {0.};            //!< condizione iniziale del tempo
		const double end_time {0.};              //!< tempo per fine simulazione
		const double collision_tolerance {0.};   //!< distanza tra due perche' avvenga la collisione,
						                         //!< se 0 la collisione avviene quando i raggi dei due pianeti si toccano

		bool save_all_data {true};    //!< salvare tutti i dati o solo gli ultimi elaborati prima del save
		double save_every_s {0.};     //!< ogni quanto salvare in secondi di simulazione
		std::string_view output_path; //!< Path dove salvare i dati, i file avranno il nome dell'id del pianeta
		double next_save_time{ save_every_s };        //!< Conta quanti step sono stati fatti

		const int data_to_keep {1};
		//##################### Metodi Protected ########################

		virtual void handle_collision();

		virtual void save_data();

		virtual void next_step() = 0;

		void remove_dead_planets();

	private:
		void set_next_save_time();
		void save_data(bool all_data);
		//~~~~~~~~~~~~~~~~~~~~~ Gestione collisioni ~~~~~~~~~~~~~~~~~~~~~
		bool are_colliding(const Planet_solver<size>& p1, const Planet_solver<size>& p2) const;
		static phy::Planet merge_planet(const Planet_solver<size>& p1, const Planet_solver<size>& p2);

		//~~~~~~~~~~~~~~~~~~~~~ Gestione salvataggio ~~~~~~~~~~~~~~~~~~~~~
		void clear_vectors(bool all_data);
		void write_to_file(bool all_data);
		void write_element(const Planet_solver<size>& p, int index, std::ofstream& outf);
		void create_files();
		void create_file(const Planet_solver<size>& planet) const;


		// Inizializzazione
		void init_planets_solver(const std::vector<phy::Planet>& pl);
		void add_planet(const phy::Planet& p);

	public:

		Solver_base(const std::vector<phy::Planet>& planets,double step,double start_time,double end_time,double collision_tolerance);

		virtual void set_save(std::string_view output_path, bool save_all_data, int step_to_next_save);

		virtual void solve();

		virtual ~Solver_base() = default;
	};


#endif