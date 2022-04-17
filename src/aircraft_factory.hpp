#pragma once

#include "airport.hpp"
#include "aircraft_types.hpp"

#include <algorithm>
#include <vector>

class AircraftFactory {
    public :
        AircraftFactory() {
            init_aircraft_types();
        }

        ~AircraftFactory()= default;

        std::unique_ptr<Aircraft> create_random_aircraft(Airport* airport) {
            return create_aircraft(*(aircraft_types[rand() % 3]), airport);
        }

    private:
        static constexpr size_t NUM_AIRCRAFT_TYPES = 3;
        AircraftType* aircraft_types[NUM_AIRCRAFT_TYPES] {};
        const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
        std::vector<std::string> assignedFlightNumber;

        // note: textures have to be initialized after initializing glut, so we'll need
        // our own init here
        inline void init_aircraft_types()
        {
            aircraft_types[0] = new AircraftType { .02f, .05f, .02f, MediaPath { "l1011_48px.png" } };
            aircraft_types[1] = new AircraftType { .02f, .05f, .02f, MediaPath { "b707_jat.png" } };
            aircraft_types[2] = new AircraftType { .02f, .1f, .02f, MediaPath { "concorde_af.png" } };
        }

        std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type, Airport* airport) {
            assert(airport); // make sure the airport is initialized before creating aircraft

            //To have an unique flight number
            std::string flight_number;
            do {
                flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
            } while (std::find(assignedFlightNumber.begin(), assignedFlightNumber.end(), flight_number) != assignedFlightNumber.end());

            assignedFlightNumber.push_back(flight_number);

            const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
            const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
            const Point3D direction = (-start).normalize();

            return std::make_unique<Aircraft>(type, flight_number, start, direction, airport->get_tower());
        }
};
