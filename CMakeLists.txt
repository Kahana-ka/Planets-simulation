cmake_minimum_required(VERSION 3.31)

project(Planets_simulation CXX)

set(CMAKE_CXX_STANDARD 23)

# Per Boost
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
#Include le dir di BOOST
include_directories(${Boost_INCLUDE_DIRS})

#Include le mie dir
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

#Per  fare l'esequibile nella root e non in cmake-build-debug
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(rk_solvers
        include/Solver_base.h
        include/solver_runge_kutta_3.h
        src/solver_runge_kutta_3.cpp
        include/solver_runge_kutta_4.h
        src/solver_runge_kutta_4.cpp
        include/solver_runge_kutta_5.h
        src/solver_runge_kutta_5.cpp
 #           include/solver_runge_kutta_adaptive.h
 #           src/solver_runge_kutta_adaptive.cpp
            )

add_library(phy include/phy.h src/phy.cpp)

add_library(gnu_plot include/gnuplot-iostream.h
                     include/gnuplotting.h
                     src/gnuplotting.cpp
            )

add_library(sys_utils include/sys_utils.h
                      src/sys_utils.cpp
            )


# Crea l'eseguibile
add_executable(Planets_simulation main.cpp)


# Aggiunge al "path" le due cartelle con gli header, i miei e quelli di boost
target_include_directories(Planets_simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

#Linka le librerie di boost
target_link_libraries(gnu_plot PUBLIC ${Boost_LIBRARIES})

target_link_libraries(gnu_plot PUBLIC sys_utils)
target_link_libraries(Planets_simulation PRIVATE rk_solvers)
target_link_libraries(Planets_simulation PRIVATE phy)
target_link_libraries(Planets_simulation PRIVATE gnu_plot)

