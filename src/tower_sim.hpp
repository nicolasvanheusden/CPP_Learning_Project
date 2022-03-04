#pragma once

class Airport;
struct AircraftType;
class AircraftManager;

class TowerSimulation
{
private:
    bool help                = false;
    Airport* airport         = nullptr;
    AircraftManager* manager = nullptr;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_aircraft(const AircraftType& type) const;
    void create_random_aircraft() const;

    void create_keystrokes() const;
    void display_help() const;

    void init_airport();
    void init_manager();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
