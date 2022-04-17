#pragma once

#include "aircraft.hpp"

#include <algorithm>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
public:
    void add_aircraft(std::unique_ptr<Aircraft> aircraft)
    {
        // 1. créer l'avion avec make_unique
        auto avion_ptr = std::move(aircraft);
        // 2. rajouter l'avion dans la liste des avions avec aircrafts.emplace
        aircrafts.emplace_back(std::move(avion_ptr));
    }

    bool move() override
    {
        std::sort(aircrafts.begin(), aircrafts.end(), [](const std::unique_ptr<Aircraft>& a1, const std::unique_ptr<Aircraft>& a2) {
            if (a2 -> has_terminal() && !a1 -> has_terminal()) { return false; }
            if (a1 -> has_terminal() && !a2 -> has_terminal()) { return true; }

            return a1->fuel_remaining() < a2->fuel_remaining();
        });

        auto aircraftsToDelete = std::remove_if(aircrafts.begin(), aircrafts.end(), [](const auto& it) {return it->move(); });
        aircrafts.erase(aircraftsToDelete, aircrafts.end());
        return true;
    }

    int get_required_fuel() const {
        return std::accumulate(aircrafts.begin(), aircrafts.end(), 0, [](int acc, const std::unique_ptr<Aircraft>& actual_aircraft){
           if (actual_aircraft -> has_cycle_finished() || !actual_aircraft->is_low_on_fuel()){
               return acc;
           }
           return 3000 + acc - actual_aircraft->fuel_remaining();
        });
    }

private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
};
