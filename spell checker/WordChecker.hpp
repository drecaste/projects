#ifndef WORDCHECKER_HPP
#define WORDCHECKER_HPP

#include <string>
#include <vector>
#include "Set.hpp"



class WordChecker
{
public:
    WordChecker(const Set<std::string>& words);

    bool wordExists(const std::string& word) const;

    std::vector<std::string> findSuggestions(const std::string& word) const;


	
private:
    const Set<std::string>& words;
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};



#endif // WORDCHECKER_HPP

