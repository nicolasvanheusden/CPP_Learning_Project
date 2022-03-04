

#include "aircraft.hpp"

#include <vector>

class AircraftManager : public GL::DynamicObject
{

private:
    std::unordered_set<Aircraft*> aircrafts = {};

public:
    AircraftManager() {}

    void move() override
    {
        std::unordered_set<Aircraft*> to_remove;

        for (auto& aircraft : aircrafts)
        {
            if (!aircraft->move())
            {
                to_remove.emplace(aircraft);
            }
        }

        for (auto& aircraft : to_remove)
        {
            aircrafts.erase(aircraft);
            delete aircraft;
        }
    }

    void add_aircraft(Aircraft* aircraft) { aircrafts.emplace(aircraft); }
};