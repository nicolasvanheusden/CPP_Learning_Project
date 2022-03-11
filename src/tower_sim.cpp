#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "aircraft_factory.hpp"
#include "aircraft_manager.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) },
    initializer { ContextInitializer(argc, argv) },
    factory { new AircraftFactory() }

{
    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_aircraft() const
{
    factory->create_random_aircraft(manager, airport);
}

void TowerSimulation::create_keystrokes() const
{
    auto airlines = AircraftFactory().get_airlines();

    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('i', []() { GL::increase_ticks(); });
    GL::keystrokes.emplace('d', []() { GL::decrease_ticks(); });

    int k = 0;
    for (std::string airline : airlines)
    {
        GL::keystrokes.emplace(
            k + '0', [airline, this]()
            { std::cout << airline << " : " << manager->number_of_aircraft_from(airline) << std::endl; });
        k++;
    }
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [letter, fun] : GL::keystrokes)
    {
        std::cout << letter << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };

    GL::move_queue.emplace(airport);
}

void TowerSimulation::init_manager()
{
    manager = new AircraftManager {};
    GL::move_queue.emplace(manager);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();

    init_manager();

    GL::loop();
}
