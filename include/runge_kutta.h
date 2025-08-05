#ifndef SOLVER_RUNGE
#define SOLVER_RUNGE

#include "phy.h"
#include <vector>
#include <fstream>

namespace rk {

	/** Definisco questa per rendere piu' leggibili i get<>() fatti sulle tuple
	*/
	using Coordinate = int;
	constexpr Coordinate x = 0;
	constexpr Coordinate y = 1;
	constexpr Coordinate z = 2;

	/** Contiene i parametri dei pianeti, questa e' da
	*   espandere per ogni sottoclasse
	*/

	struct Planet_solver {
		phy::Planet planet;
		bool dead{ false };
		std::vector<std::tuple<double, double, double>> k_pos;
		std::vector<std::tuple<double, double, double>> k_speed;
	};

	//template <typename T>
	class RK_base {
	protected:

		//##################### ATTRIBUTI ########################

		//!< modificati quando si salvano su file
		//************ DA vedere come fare ******************* 
		// Puo essere al massimo cambiato in un template
		// Viene inizializzato dalla sotto classe
		std::vector<Planet_solver> planets_solver{ };
		std::vector<double> time{ }; //!< contiene il tempo t dello step, le equazioni sono tutte indipendenti da t.


		// Parametri simulazione
		double h;                     //!< grandezza step
		double start_time;            //!< condizione iniziale del tempo
		double end_time;              //!< tempo per fine simulazione
		double collision_tollerrance; //!< distanza tra due perche' avvenga la collisione, se 0 la collisione avviene quando i raggi dei due pianeti si toccano

		/** I dati vengono salvati su file, simulazioni grandi avranno un enormita di dati, e eliminera' dai
		* vettori i dati dall'inizio fino ad un punto i, questo sara' deciso dalle sotto classi
		* I dati vengono scritti su file dopo che sono passate step_to_save steps.
		*
		* step_to_save viene calcolato da ogni quanto si vuole salvare utilizzando il tempo della simulazione, quindi ad esempio
		* ogni 2s simulati, sara' fatto il calcolo in base al time step.
		*
		* Se save all data e' sul vero verranno salvati tutti i dati, se falso verranno salvati solo i dati dello step =  step_to_save.
		* Viene creato un file per pianeta contenete i dati di questo.
		*/
		bool save_all_data;
		int step_to_save;
		std::string_view output_path; //!< Path dove salvare i dati, i file avranno il nome dell'id del pianeta
		int step_counter{ 0 };        //!< Conta quanti step sono stati fatti


		//##################### Metodi Protected ########################

		//~~~~~~~~~~~~~~~~~~~~~ Gestione collisioni ~~~~~~~~~~~~~~~~~~~~~

		/**
			* Controlla se due pianeti stanno collidendo, ovvero la posizione tra i due e' minore
			* del raggio di collisione
			* @todo vedere di riuscire a velocizzare esseno O(n^2)
		*/
		bool are_colliding(const phy::Planet&, const phy::Planet&);

		/** Calcola la distanza tra due punti in 3D
		*/
		double dist(double x1, double x2, double y1, double y2, double z1, double z2);

		/** Gestisce la collisione, rendendo morti i due pianeti che collidono e
		*   creandone uno nuovo
		*  @todo migliorare la velocita' del controllo
		*  @param deve inizializzare il nuovo pianeta, serve il numero di parametri, e forza una save.
		*/
		void handle_collision(int, int);

		/** Crea e inizializza un nuovo pianete, avente massa la somma dei due
		*  come raggio e posizione la media dei due e come velocita' si usa la conservazione
		*  del momento.
		* @todo migliorare la creazione dell'id
		*/
		phy::Planet create_planet(const phy::Planet& p1, const phy::Planet& p2);

		//~~~~~~~~~~~~~~~~~~~~~~ Gestione salvataggio su file ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		/** Salva i dati su file, e pulisce i vettori di planets,
		*   n_to_keep e' il numero di elemente che si tengono dalla fine del vettore, questi dati
		*   NON saranno scritti su file.
		*/
		void save_data(int n_to_keep);
		void clear_vectors(int n_to_keep);
		void write_to_file(int n_to_keep);

		/** Funzione per scrivere un dato dato il pianeta e il file
		*/
		void write_element(const phy::Planet&, int, std::ofstream&);

		/** Crea i file dove saranno scritti i dati
		*  l'overload crea il file per un nuovo pianeta
		*/
		void create_files();
		void create_files(const phy::Planet planet);

		// Per inizializzare planet solver, l'int sono il numero di parametri
		void init_planets_solver(const phy::Planet&, int);

	public:

		//##################### Metodi Publici ########################

		/** Setta come salvare i dati e dove,
		*   se save_all_data e' vero salvera tutti i dati altimenti solo
		*   l'ultimo dato dei vettori pre salvataggio.
		*/
		void set_save(std::string_view output_path, bool save_all_data, int step_to_next_save);

	};


}

#endif