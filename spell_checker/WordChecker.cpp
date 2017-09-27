#include "WordChecker.hpp"
#include "otherFunctions.hpp"
#include<iostream>
#include <algorithm>
#include <string>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}

/*
  METHODS:
	   1.  Swapping each adjacent pair of characters in the word.
	   2.  In between each adjacent pair of characters in the word (also before the first character and after the last character),
			each letter from 'A' through 'Z' is inserted.
	   3.  Deleting each character from the word.
	   4.  Replacing each character in the word with each letter from 'A' through 'Z'.
	   5.  Splitting the word into a pair of words by adding a space in between each adjacent pair of characters in the word.
			It should be noted that this will only generate a suggestion if both words in the pair are found in the word set.
*/

std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	std::vector<std::string> suggestions;


	// 1.
	for(int i = 0; i < word.length(); i++)
	{
		std::string newWord = word;
		newWord[i] = word[i + 1];
		newWord[i + 1] = word[i];
		std::transform(newWord.begin(), newWord.end(), newWord.begin(), ::toupper);
		if(words.contains(newWord))
		{
			addSuggestion(newWord, suggestions);
		}
	}

	// 2.
	for(int i = 0; i <= word.length(); i++)
	{
		for(int j = 0; j < 26; j++)
		{
			std::string newWord = word;
			newWord.insert(i, 1, alphabet[j]);
			std::transform(newWord.begin(), newWord.end(), newWord.begin(), ::toupper);

			if(words.contains(newWord))
			{
				addSuggestion(newWord, suggestions);
			}
		}
	}

	// 3.
	for(int i = 0; i < word.length(); i++)
	{
		std::string newWord = word;
		newWord = newWord.erase(i, 1);
		std::transform(newWord.begin(), newWord.end(), newWord.begin(), ::toupper);
		if(words.contains(newWord))
		{
			addSuggestion(newWord, suggestions);
		}
	}

	// 4.
	for(int i = 0; i < word.length(); i++)
	{
		for(int j = 0; j < 26; j++)
		{
			std::string newWord = word;
			newWord[i] = alphabet[j];
			std::transform(newWord.begin(), newWord.end(), newWord.begin(), ::toupper);
			if(words.contains(newWord))
			{
				addSuggestion(newWord, suggestions);
			}
		}
	}

	// 5.
	for(int i = 0; i < word.length(); i++)
	{
		std::string firstWord = word.substr(0, i);
		std::string secondWord = word.substr(i, word.length());
		std::string newWord = firstWord + " " + secondWord;
		std::transform(firstWord.begin(), firstWord.end(), firstWord.begin(), ::toupper);
		std::transform(secondWord.begin(), secondWord.end(), secondWord.begin(), ::toupper);
		if(words.contains(firstWord))
		{
			if(words.contains(secondWord))
			{
				addSuggestion(newWord, suggestions);
			}
		}
		
	}
	return suggestions;
	}

