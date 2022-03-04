

#include "aircraft.hpp"

#include <vector>

class AicraftManager
{

private:
    std::vector<const Aircraft> aircrafts;

public:
    AicraftManager() {}

    void addAircaft(const Aircraft& aircraft) { aircrafts.emplace_back(aicraft); }
}