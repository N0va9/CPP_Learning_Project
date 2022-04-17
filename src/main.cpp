#include "tower_sim.hpp"
#include "geometry.hpp"

using namespace std;

void test_generic_points() {
    Point<2, float> p1 = {1,2};
    Point<2, float> p2 = {7, 4};
    //auto p3 = p1 + p2;
    p1 += p2;
    p1 *= 3.f; // ou 3.f, ou 3.0 en fonction du type de Point

}

int main(int argc, char** argv)
{
    //test_generic_points();
    TowerSimulation simulation { argc, argv };
    simulation.launch();

    return 0;
}