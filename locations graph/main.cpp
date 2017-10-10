#include <iostream>
#include "InputReader.hpp"
#include "Digraph.hpp"
#include "RoadMapReader.hpp"
#include "TripReader.hpp"
#include "RoadMapWriter.hpp"
#include <algorithm>
#include <iomanip>
#include <math.h>

std::vector<int> mapInfo(std::map<int, int> shortestPathMap, int startVertex, int endVertex, RoadMap map);
void printTime(double amount);
void printDirections(TripMetric type, std::vector<Trip>::iterator it, std::vector<std::pair<std::string, RoadSegment>> tripsToPrint, RoadMap map);

int main()
{
	InputReader input = InputReader(std::cin);
	RoadMap map = RoadMapReader{}.readRoadMap(input);
	std::vector<Trip> trips = TripReader{}.readTrips(input);
	
	if(! (map.isStronglyConnected()) )
	{
		std::cout << "Disconnected Map" << std::endl;
		return 0;
	}
	
	// functions for distance and time
	std::function<double(const RoadSegment&)> distance = [](RoadSegment roadseg)
	{
		return roadseg.miles;
	};
	std::function<double(const RoadSegment&)> time = [](RoadSegment roadseg)
	{
		return roadseg.miles / roadseg.milesPerHour;
	};
	
	
	for(auto it = trips.begin(); it != trips.end(); it++)
	{
		std::map<int, int> shortestPaths;
		std::vector<std::pair<std::string, RoadSegment>> tripsToPrint;
		
		// get shortest paths using metric
		if(it->metric == TripMetric::Distance) // distance
		{
			shortestPaths = map.findShortestPaths(it->startVertex, distance);
		}
		else // time
		{
			shortestPaths = map.findShortestPaths(it->startVertex, time);
		}
		
		// find shortest path for startVertex to endVertex
		int prevVertex = it->endVertex;
		while(prevVertex != it->startVertex)
		{
			int temp = prevVertex;
			prevVertex = shortestPaths[prevVertex];
			tripsToPrint.push_back(std::make_pair(map.vertexInfo(temp), map.edgeInfo(prevVertex, temp)));
		}
		
		// reverse for print, then print
		std::reverse(tripsToPrint.begin(), tripsToPrint.end());
		printDirections(it->metric, it, tripsToPrint, map);
	}
	return 0;
}

void printDirections(TripMetric type, std::vector<Trip>::iterator it, std::vector<std::pair<std::string, RoadSegment>> tripsToPrint, RoadMap map)
{
	double amount = 0.0;
	if(type == TripMetric::Distance)
	{
		amount = 0.0;
		std::cout << "Shortest distance from " << map.vertexInfo(it->startVertex) << " to " <<
		map.vertexInfo(it->endVertex) << "\n  Begin at " << map.vertexInfo(it->startVertex) << std::endl;
		
		for(int i = 0; i < tripsToPrint.size(); i++)
		{
			std::cout << "  Continue to " << tripsToPrint[i].first << " (" << tripsToPrint[i].second.miles; << " miles)" << std::endl;
			amount += miles;
		}
		
		std::cout << "Total Distance: " << std::setprecision(2) << amount << " miles" << std::endl;
	}
	else
	{
		amount = 0.0;
		std::cout << "Shortest driving time from " << map.vertexInfo(it->startVertex) << " to " <<
		map.vertexInfo(it->endVertex) << "\n  Begin at " << map.vertexInfo(it->startVertex) << std::endl;
			
		for(int i = 0; i < tripsToPrint.size(); i++)
		{
			double currTime = tripsToPrint[i].second.miles / tripsToPrint[i].second.milesPerHour * 3600;
			std::cout << "  Continue to " << tripsToPrint[i].first << " (" << tripsToPrint[i].second.miles << " miles @ " << tripsToPrint[i].second.milesPerHour << " mph = " ;
			printTime(currTime);
			std::cout << ")" << std::endl;
			amount += currTime;
		}
		
		std::cout << "Total Time: ";
		printTime(amount);
	}
}

// print time
void printTime(double amount)
{
	int hours = 0;
	int minutes = 0;
	double seconds = 0.0;

	if(amount < 60)	// less than a minute
	{
		std::cout << amount << " secs";
	}
	else if((amount >= 60) && (amount < 3600))	// less than an hour
	{
		minutes = amount / 60;
		seconds = int(amount) % 60;
		std::cout << minutes << " mins " << seconds << " secs";
	}
	else	// more than one hour
	{
		minutes = amount / 60;
		seconds = int(amount) % 60;
		hours = minutes / 60;
		minutes = minutes % 60;
		std::cout << hours << " hrs " << minutes << " mins " << seconds << " secs";
	}
}
