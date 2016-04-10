#include "Common.hpp"

#include "Population.hpp"

int main ()
{

    Population<double> test (50, 50);

    //cout << test << endl;

    test.live ();

    //cout << test << endl;

    return 0;
}
