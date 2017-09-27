#ifndef INPUTREADER_HPP
#define INPUTREADER_HPP

#include <istream>
#include <string>



class InputReader
{
public:
    InputReader(std::istream& in): in_{in} { }

    std::string readLine();

    int readIntLine();

private:
    std::istream& in_;
};



#endif // INPUTREADER_HPP

