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
};