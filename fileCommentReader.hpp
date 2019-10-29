//
// Created by hubert on 10/25/19.
//

#ifndef OK_FILECOMMENTREADER_HPP
#define OK_FILECOMMENTREADER_HPP


#include <fstream>
#include <utility>

class fileCommentReader {
private:
    std::fstream fileHandler;
    std::string fieldName;
    std::string fieldValue;
    std::string fileName;
    std::string line;

public:
    fileCommentReader(std::string  fileName) : fileName(std::move(fileName)){
    }

    std::string getValue(std::string nameOfField)
    {
        fieldName = std::move(nameOfField);
        readValue();
        return fieldValue;
    }

private:

    void readValue()
    {
        openFile();
        searchForField();
        closeFile();
    }

    void openFile()
    {
        fileHandler.open(fileName);
    }

    void closeFile()
    {
        fileHandler.close();
    }

    void searchForField()
    {
        while(lineIsComment() && isNotEOF()) {
            readLine();
            ifFieldSaveValue();
        }
    }

    bool isNotEOF()
    {
        return !fileHandler.eof();
    }

    void readLine()
    {
        std::getline(fileHandler, line);
    }

    void ifFieldSaveValue()
    {
        if (line.find(fieldName) != std::string::npos)
            saveValue();
    }

    bool lineIsComment()
    {
        return fileHandler.peek() == ';';
    }

    void saveValue()
    {
        auto positionOfLabel = line.find(fieldName);
        auto beforeValue = line.find(' ', positionOfLabel);
        auto afterValue = line.find(' ', beforeValue + 1);
        fieldValue = line.substr(beforeValue, afterValue - beforeValue);
    }
};


#endif //OK_FILECOMMENTREADER_HPP
