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

        aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                       [](std::unique_ptr<Aircraft>& aircraft) { return !aircraft->move(); }),
                        aircrafts.end());
    }

    void add_aircraft(std::unique_ptr<Aircraft>& aircraft) { aircrafts.emplace_back(std::move(aircraft)); }

    int number_of_aircraft_from(const std::string& flight_number) const
    {

        return count_if(aircrafts.begin(), aircrafts.end(),
                        [flight_number](const std::unique_ptr<Aircraft>& aircraft)
                        { return aircraft->get_flight_num().substr(0, 2) == flight_number; });
    }
};