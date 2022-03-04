#pragma once

#include "aircraft.hpp"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

class AircraftManager : public GL::DynamicObject
{

private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;

public:
    AircraftManager() {}

    void move() override
    {

        for (auto aircraft_it = aircrafts.begin(); aircraft_it != aircrafts.end();)
        {
            // On doit déréférencer 2x pour obtenir une référence sur l'Aircraft : l'it et le pointeur
            auto& aircraft = **aircraft_it;
            if (aircraft.move())
            {
                ++aircraft_it;
            }
            else
            {
                aircraft_it = aircrafts.erase(aircraft_it);
            }
        }
    }

    void add_aircraft(std::unique_ptr<Aircraft>& aircraft) { aircrafts.emplace_back(std::move(aircraft)); }
};