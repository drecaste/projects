#include"otherFunctions.hpp"

void addSuggestion(std::string suggestion, std::vector<std::string>& Suggestions)
{
	if(!(std::find(Suggestions.begin(), Suggestions.end(), suggestion) != Suggestions.end()))
	{
		Suggestions.push_back(suggestion);
	}

}