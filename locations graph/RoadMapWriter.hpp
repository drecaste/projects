#ifndef ROADMAPWRITER_HPP
#define ROADMAPWRITER_HPP

#include <ostream>
#include "RoadMap.hpp"



class RoadMapWriter
{
public:
    void writeRoadMap(std::ostream& out, const RoadMap& roadMap);
};



#endif // ROADMAPWRITER_HPP

