#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

using namespace std;

namespace HexGame_200104004094
{
    //lastMove and getMove functions throw this exception
    class NoMoveException : public runtime_error
    {
    public:
        NoMoveException()
            :runtime_error("There is no moves to return.") {}
    };

    //overloaded operator() throws this exception
    class InvalidBoardCellException : public runtime_error
    {
    public:
        InvalidBoardCellException()
            :runtime_error("There is no cell with these coordinates in the board."){}
    };

    //allocateMemory function throws this exception
    class MemoryAllocationException : public bad_alloc
    {
    public:
        MemoryAllocationException()
            :bad_alloc(){}
    };

    //readFromFile and writeToFile functions throw this exception
    class CannotOpenFileException : public runtime_error
    {
    public:
        CannotOpenFileException()
            :runtime_error("Cannot open the file."){}
    };
}
#endif // EXCEPTIONS_H_INCLUDED
