#pragma once

#include "aircraft_factory.hpp"
#include "aircraft_manager.hpp"

class Airport;
struct AircraftType;

struct ContextInitializer {
    ContextInitializer(int argc, char** argv) {
        MediaPath::initialize(argv[0]);
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        GL::init_gl(argc, argv, "Airport Tower Simulation");
    }
};

class TowerSimulation
{
private:
    bool help        = false;
    Airport* airport = nullptr;
    ContextInitializer contextInitializer;
    AircraftManager aircraftManager;
    AircraftFactory aircraftFactory;


    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_keystrokes();
    static void display_help();

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};