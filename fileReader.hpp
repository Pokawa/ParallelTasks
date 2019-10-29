//
// Created by hubert on 10/23/19.
//

#ifndef OK_FILEREADER_HPP
#define OK_FILEREADER_HPP

#include <fstream>

class fileReader
{
private:
    std::fstream fileHandler;
    std::string line;

public:
    fileReader(const std::string & fileName)
    {
        fileHandler.open(fileName, std::ios::in);
        prepareFile();
    }

    std::string getLine()
    {
        readLine();
        return line;
    }

    bool isNotEOF()
    {
        return !fileHandler.eof();
    }

    virtual ~fileReader() {
        fileHandler.close();
    }

private:

    void prepareFile()
    {
        while(lineIsComment()) {
            readLine();
        }
    }

    void readLine()
    {
        std::getline(fileHandler, line);
    }

    bool lineIsComment()
    {
        return fileHandler.peek() == ';';
    }
};

#endif //OK_FILEREADER_HPP
