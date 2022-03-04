#pragma once

#include "GL/opengl_interface.hpp"
#include "config.hpp"

#include <cstdlib>
#include <ctime>

class Airport;
struct AircraftType;
class AircraftManager;
class AircraftFactory;

struct ContextInitializer
{
public:
    ContextInitializer(int argc, char** argv)
    {
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
    ContextInitializer initializer;
    AircraftManager* manager;
    AircraftFactory* factory;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_aircraft() const;

    void create_keystrokes() const;
    void display_help() const;

    void init_airport();
    void init_manager();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
