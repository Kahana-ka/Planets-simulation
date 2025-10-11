
// Std library
#include <iostream>
#include <vector>

#include <format>

// My library
#include "solver_runge_kutta_3.h"
#include "phy.h"
#include "solver_runge_kutta_4.h"
#include "solver_runge_kutta_5.h"
#include <filesystem>
#include "input_reader.h"

void start_simulation(const std::vector<phy::Planet>& planets,const InputReader::Sim& param);

int main(int argc, char** argv) {
std::cout << ""
	" _______   __                                  __             ______   __                          __              __      __                     \n"
	"/       \\ /  |                                /  |           /      \\ /  |                        /  |            /  |    /  |                    \n"
	"$$$$$$$  |$$ |  ______   _______    ______   _$$ |_         /$$$$$$  |$$/  _____  ____   __    __ $$ |  ______   _$$ |_   $$/   ______   _______  \n"
	"$$ |__$$ |$$ | /      \\ /       \\  /      \\ / $$   |        $$ \\__$$/ /  |/     \\/    \\ /  |  /  |$$ | /      \\ / $$   |  /  | /      \\ /       \\ \n"
	"$$    $$/ $$ | $$$$$$  |$$$$$$$  |/$$$$$$  |$$$$$$/         $$      \\ $$ |$$$$$$ $$$$  |$$ |  $$ |$$ | $$$$$$  |$$$$$$/   $$ |/$$$$$$  |$$$$$$$  |\n"
	"$$$$$$$/  $$ | /    $$ |$$ |  $$ |$$    $$ |  $$ | __        $$$$$$  |$$ |$$ | $$ | $$ |$$ |  $$ |$$ | /    $$ |  $$ | __ $$ |$$ |  $$ |$$ |  $$ |\n"
	"$$ |      $$ |/$$$$$$$ |$$ |  $$ |$$$$$$$$/   $$ |/  |      /  \\__$$ |$$ |$$ | $$ | $$ |$$ \\__$$ |$$ |/$$$$$$$ |  $$ |/  |$$ |$$ \\__$$ |$$ |  $$ |\n"
	"$$ |      $$ |$$    $$ |$$ |  $$ |$$       |  $$  $$/       $$    $$/ $$ |$$ | $$ | $$ |$$    $$/ $$ |$$    $$ |  $$  $$/ $$ |$$    $$/ $$ |  $$ |\n"
	"$$/       $$/  $$$$$$$/ $$/   $$/  $$$$$$$/    $$$$/         $$$$$$/  $$/ $$/  $$/  $$/  $$$$$$/  $$/  $$$$$$$/    $$$$/  $$/  $$$$$$/  $$/   $$/ \n\n\n";

	if (argc < 2) {
		std::cout <<  "Missing file" << std::endl;
	}
	std::cout << "Reading file" << std::endl;

	InputReader input_reader{std::string(argv[1])};
	input_reader.read_input();

	const std::vector<phy::Planet> planets = input_reader.get_planets();
	const std::vector<InputReader::Sim> sim_param = input_reader.get_sims();
	std::cout << "Done" << std::endl << std::endl;
	std::cout << "Number of simulations: " << sim_param.size() << std::endl;

	if (sim_param.empty()) {
		return -1;
	}

	for (const auto& sim : sim_param) {
		std::cout << "starting simulation" << std::endl;
		start_simulation(planets, sim);
		std::cout << "DONE" << std::endl << std::endl;
	}

	return 0;
}

void start_simulation(const std::vector<phy::Planet>& planets,const InputReader::Sim& param) {

	if (!std::filesystem::exists(param.save_path)) {
		std::cout << "Directory not found" << std::endl;
		std::cout << "Creating directory" << std::endl;
		std::filesystem::create_directories(param.save_path);
	}

	std::cout << "Directory found" << std::endl;


	switch (param.model) {
		case InputReader::RK3: {
			rk3::RK3_solver rk3_solver{planets,param.step,param.start_time,param.end_time,param.collision_toll};
			rk3_solver.set_save(param.save_path,param.save_all,param.save_every);
			rk3_solver.solve();
			break;
		}
		case InputReader::RK4: {
			rk4::RK4_solver rk4_solver{planets,param.step,param.start_time,param.end_time,param.collision_toll};
			rk4_solver.set_save(param.save_path,param.save_all,param.save_every);
			rk4_solver.solve();
			break;
		}

		case InputReader::RK5: {
			rk5::RK5_solver rk5_solver{planets,param.step,param.start_time,param.end_time,param.collision_toll};
			rk5_solver.set_save(param.save_path,param.save_all,param.save_every);
			rk5_solver.solve();
			break;
		}
		default:
		std::cout << "Not yet implemented" << std::endl;
		break;
	}
}