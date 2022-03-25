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
    int number_of_aircrafts_crashed = 0;

public:
    AircraftManager() {}

    void sort_aircrafts()
    {
        std::sort(aircrafts.begin(), aircrafts.end(),
                  [](std::unique_ptr<Aircraft>& first, std::unique_ptr<Aircraft>& second)
                  {
                      if (first->has_terminal() && second->has_terminal())
                      {
                          return first->get_remaining_fuel() < second->get_remaining_fuel();
                      }

                      if (first->has_terminal())
                      {
                          return true;
                      }

                      if (second->has_terminal())
                      {
                          return false;
                      }

                      return first->get_remaining_fuel() < second->get_remaining_fuel();
                  });
    }

    void move() override
    {

        aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                       [this](std::unique_ptr<Aircraft>& aircraft)
                                       {
                                           try
                                           {
                                               return !aircraft->move();
                                           } catch (const AircraftCrash& crash)
                                           {
                                               number_of_aircrafts_crashed++;
                                               return true;
                                           }
                                       }),
                        aircrafts.end());
    }

    void add_aircraft(std::unique_ptr<Aircraft>& aircraft) { aircrafts.emplace_back(std::move(aircraft)); }

    int number_of_aircraft_from(const std::string& flight_number) const
    {

        return count_if(aircrafts.begin(), aircrafts.end(),
                        [flight_number](const std::unique_ptr<Aircraft>& aircraft)
                        { return aircraft->get_flight_num().substr(0, 2) == flight_number; });
    }

    int get_required_fuel() const
    {
        return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
                               [](unsigned int acc, const std::unique_ptr<Aircraft>& aircraft)
                               {
                                   if (aircraft->is_low_on_fuel() && aircraft->get_is_at_terminal())
                                   {
                                       return acc + (3000 - aircraft->get_remaining_fuel());
                                   }
                                   return acc;
                               });
    }

    int get_number_of_aircrafts_crashed() const { return number_of_aircrafts_crashed; }
};