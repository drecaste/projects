#ifndef TRIPREADER_HPP
#define TRIPREADER_HPP

#include <vector>
#include "Trip.hpp"
#include "InputReader.hpp"



class TripReader
{
public:
    std::vector<Trip> readTrips(InputReader& in);
};



#endif // TRIPREADER_HPP

