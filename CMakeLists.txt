cmake_minimum_required(VERSION 3.20)
project(progetto) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")


set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel" FORCE)

# Abilita il testing per l'intero progetto 
enable_testing()

# Ricerca pacchetti e creazione lista librerie
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND libraries_list PUBLIC Eigen3::Eigen)

# --- TARGET PRINCIPALE ---
set(SOURCES main.cpp directed_edge.hpp directed_graph.hpp project.hpp components.hpp)
add_executable(main ${SOURCES})
target_link_libraries(main ${libraries_list})
