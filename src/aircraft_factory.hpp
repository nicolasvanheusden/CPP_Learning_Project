#pragma once

#include "aircraft_manager.hpp"
#include "aircraft_types.hpp"
#include "airport.hpp"

#include <string.h>
#include <unordered_set>

class AircraftFactory
{

private:
    static const int NUM_AIRCRAFT_TYPES = 3;
    AircraftType* aircraft_types[NUM_AIRCRAFT_TYPES] {};
    const std::string airlines[8]                      = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
    std::unordered_set<std::string> flight_numbers_set = {};

    // note: textures have to be initialized after initializing glut, so we'll need
    // our own init here
    void init_aircraft_types()
    {
        aircraft_types[0] = new AircraftType { .02f, .05f, .02f, MediaPath { "l1011_48px.png" } };
        aircraft_types[1] = new AircraftType { .02f, .05f, .02f, MediaPath { "b707_jat.png" } };
        aircraft_types[2] = new AircraftType { .02f, .07f, .05f, MediaPath { "concorde_af.png" } };
    }

    bool exists(const std::string& flight_number) const
    {
        return flight_numbers_set.find(flight_number) != flight_numbers_set.end();
    }

public:
    AircraftFactory() { init_aircraft_types(); }

    void create_aircraft(const AircraftType& type, AircraftManager* manager, Airport* airport)
    {
        assert(airport); // make sure the airport is initialized before creating aircraft
        assert(manager); // make sure the manager is initialized before adding aircraft

        std::string fn = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
        while (exists(fn))
        {
            fn = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
        }
        const std::string flight_number = fn;
        const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
        const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
        const Point3D direction = (-start).normalize();

        Aircraft* aircraft = new Aircraft { type, flight_number, start, direction, airport->get_tower() };
        manager->add_aircraft(aircraft);

        flight_numbers_set.emplace(flight_number);
    }

    void create_random_aircraft(AircraftManager* manager, Airport* airport)
    {
        create_aircraft(*(aircraft_types[rand() % 3]), manager, airport);
    }
};