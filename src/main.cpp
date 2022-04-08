#include "tower_sim.hpp"

void test_generic_points()
{
    Point<3, int> p1;
    Point<3, int> p2;
    auto p3 = p1 + p2;
    p1 += p3;
    p1 *= 3;

    Point<2, float> p4;
    Point<2, float> p5;
    auto p6 = p4 + p5;
    p4 += p6;
    p4 *= 3.f;

    Point<3, double> p7;
    Point<3, double> p8;
    auto p9 = p7 + p8;
    p7 += p9;
    p7 *= 3.0;
}

// int main(void)
// {
//     test_generic_points();
// }

int main(int argc, char** argv)
{
    TowerSimulation simulation { argc, argv };
    simulation.launch();

    return 0;
}