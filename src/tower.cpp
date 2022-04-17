#include "tower.hpp"

#include "airport.hpp"
#include "terminal.hpp"

WaypointQueue Tower::get_circle() const
{
    return { { Point3D { -1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, 1.5f, .5f }, wp_air },
             { Point3D { -1.5f, 1.5f, .5f }, wp_air } };
}

WaypointQueue Tower::get_instructions(Aircraft& aircraft)
{
    assert(&aircraft);
    if (!aircraft.is_at_terminal)
    {
        // if the aircraft is far, then just guide it to the airport vicinity
        if (aircraft.distance_to(airport.pos) < 5)
        {
            // try and reserve a terminal for the craft to land
            const auto vp = airport.reserve_terminal(aircraft);
            if (!vp.first.empty())
            {
                reserved_terminals[&aircraft] = vp.second;
                return vp.first;
            }
            else
            {
                return get_circle();
            }
        }
        else
        {
            return get_circle();
        }
    }
    else
    {
        // get a path for the craft to start
        const auto terminal_num = reserved_terminals[&aircraft];

        Terminal& terminal      = airport.get_terminal(terminal_num);
        if (!terminal.is_servicing())
        {
            terminal.finish_service();
            reserved_terminals.erase(&aircraft);
            aircraft.is_at_terminal = false;
            aircraft.cycle_finished = true;
            return airport.start_path(terminal_num);
        }
        else
        {
            return {};
        }
    }
}

void Tower::arrived_at_terminal(const Aircraft& aircraft)
{
    assert(&aircraft);
    airport.get_terminal(reserved_terminals[&aircraft]).start_service(aircraft);
}

WaypointQueue Tower::reserve_terminal(Aircraft& aircraft) {
    assert(&aircraft);
    auto res = airport.reserve_terminal(aircraft);

    if (res.first.empty()) { reserved_terminals[&aircraft] = res.second; }

    return res.first;
}

void Tower::destroy(Aircraft& aircraft){
    assert(&aircraft);
    auto val = reserved_terminals.find(&aircraft);
    if (reserved_terminals.end() != val ){
        airport.get_terminal(val->second).free();
        reserved_terminals.erase(val);
    }
}