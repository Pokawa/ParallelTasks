//
// Created by hubert on 10/23/19.
//

#ifndef OK_FILEREADER_HPP
#define OK_FILEREADER_HPP

#include <fstream>
#include <utility>

class fileReader
{
private:
    std::fstream fileHandler;
    const std::string fileName;
    std::string line;

public:
    explicit fileReader(std::string  fileName) : fileName(std::move(fileName))
    {
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

    void open()
    {
        fileHandler.open(fileName, std::ios::in);
        prepareFile();
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
