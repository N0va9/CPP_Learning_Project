#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "airport.hpp"
#include "aircraft.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) },
    contextInitializer(argc, argv)
{
    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_keystrokes() {
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { aircraftManager.add_aircraft(aircraftFactory.create_random_aircraft(airport)); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });

    GL::keystrokes.emplace('p', []() { GL::toggle_pause(); });

    GL::keystrokes.emplace('u', []() { GL::up_ticks_per_seconds(); });
    GL::keystrokes.emplace('d', []() { GL::down_ticks_per_seconds(); });

    GL::keystrokes.emplace('m', [this]() { aircraftManager.print_nb_crashs(); });

    for(int i = 0; i < 8; i++ ){
        GL::keystrokes.emplace(i + '0', [this, i]() {aircraftFactory.printFlightNumberInfoAtIndex(i); });
    }


}

void TowerSimulation::display_help() {
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    /* Sans Structured Binding
    for (const auto& ks_pair : GL::keystrokes)
    {
        std::cout << ks_pair.first << ' ';
    }*/

    //Avec Structured Binding
    for (const auto& [key, _] : GL::keystrokes) {
        std::cout << key << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport { aircraftManager, one_lane_airport , Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };

    GL::Displayable::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
    GL::move_queue.emplace(&aircraftManager);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();

    GL::loop();
}
