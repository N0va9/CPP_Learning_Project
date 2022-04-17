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
        aircrafts.emplace(std::move(avion_ptr));
    }

    bool move() override
    {
        for (auto it = aircrafts.begin();
             it != aircrafts.end();) // Notez bien le manque de l'increment ++it ici car on va incrementer it
                                     // selon la réponse de move()
        {
            if ((*it)->move())
            {
                // ************* déjà fait TASK0 exo 5
                //          display_queue.erase(*it);   // n'oubliez pas d'enlever l'objet de la
                //          display_queue; (5) Faites en sort que ceci est fait automatiquement (par le
                //          destructeur)
                // *************

                // ***** gestion automatique par le destructeur de std::unqiue_ptr
                //          delete *it;                 // c'est pas bien, mais nécessaire parce qu'on a créé
                //          l'avion via new.... cela change dès qu'on trouve un propre owner des avions
                //          (TASK1)
                it = aircrafts.erase(it); // ici, on enleve *it de aircrafts d'une facon safe
            }
            else
                ++it;
        }
        return true;
    }

private:
    std::set<std::unique_ptr<Aircraft>> aircrafts;
};
