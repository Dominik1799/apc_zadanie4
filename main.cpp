#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>


struct Metadata {
    std::string inputFile;
    std::string outputFile;
    std::vector<size_t> columnNumbers;
    std::vector<std::vector<std::string>> table;
    static size_t currCompareColumn;
    static bool compareRows(std::vector<std::string> row1, std::vector<std::string> row2);
};
size_t Metadata::currCompareColumn = 0;

void exitProgram(int exitCode, const std::string& message){
    std::cerr << message << "\n";
    exit(exitCode);
}

void parseColumnNumbers(std::string& rawColumns,std::vector<size_t>& columns){
    bool wasNumber = false;
    size_t num,begin = 0;
    for(size_t i = 0; i < rawColumns.size(); i++){
        if ((rawColumns[i] > '0' && rawColumns[i] <= '9') || (rawColumns[i] == '0' && wasNumber)){
            begin = wasNumber ? begin : i;
            wasNumber = true;
        } else if (rawColumns[i] == ',') {
            if (!wasNumber || i == rawColumns.size()-1)
                exitProgram(1,"3rd argument has invalid format.");
            std::string subst = rawColumns.substr(begin,i-begin);
            num = std::stoul(subst);
            if (std::find(columns.begin(),columns.end(),num) != columns.end()){
                exitProgram(1,"3rd argument has invalid format: Duplicates.");
            }
            columns.push_back(num);
            wasNumber = false;
        } else {
            exitProgram(1, "3rd argument has invalid format.");
        }
    }
    std::string subst = rawColumns.substr(begin);
    num = std::stoul(subst);
    if (std::find(columns.begin(),columns.end(),num) != columns.end()){
        exitProgram(1,"3rd argument has invalid format: Duplicates.");
    }
    columns.push_back(num);
}

std::vector<std::string> parseRow(std::string& row){
    std::stringstream lineStream(row);
    std::vector<std::string> result;
    std::string buffer;
    while (std::getline(lineStream,buffer,',')){
        result.push_back(buffer);
    }
    return result;
}

Metadata readData(int argc, char* argv[]){
    if (argc != 4){
        std::cerr << "Invalid arguments" << "\n";
        exit(1);
    }
    std::string str = argv[3];
    Metadata metadata;
    metadata.inputFile = argv[1];
    metadata.outputFile = argv[2];
    parseColumnNumbers(str,metadata.columnNumbers);
    std::ifstream input(metadata.inputFile);
    std::string buffer;
    size_t prevSize = 0;
    while (std::getline(input,buffer)){
        if (buffer.back() == ',')
            buffer.append(",");
        std::vector<std::string> vec = parseRow(buffer);
        if (prevSize != vec.size() && prevSize != 0){
            exitProgram(1,"Invalid input data format.");
        }
        prevSize = vec.size();
        metadata.table.push_back(vec);
    }
    if (!input.eof() || input.bad() || metadata.table.empty() || metadata.table[0].empty())
        exitProgram(1,"Invalid input data format.");
    return metadata;
}

bool Metadata::compareRows(std::vector<std::string> row1, std::vector<std::string> row2){
    return (row1[currCompareColumn] < row2[currCompareColumn]);
}

void sortData(Metadata& metadata){
    for (auto it = metadata.columnNumbers.rbegin(); it != metadata.columnNumbers.rend(); ++it){
        Metadata::currCompareColumn = *it - 1;
        std::stable_sort(metadata.table.begin(),metadata.table.end(),metadata.compareRows);
    }
}

void writeData(Metadata& metadata){
    std::ofstream output(metadata.outputFile);
    for (auto & row : metadata.table){
        for (size_t j = 0; j < row.size(); j++){
            output << row[j];
            if (j+1 == row.size())
                output << "\n";
            else
                output << ",";
        }
    }
    if (output.fail())
        exitProgram(1,"Error while writing data.");
}


int main(int argc, char* argv[]) {
    Metadata metadata = readData(argc,argv);
    sortData(metadata);
    writeData(metadata);
    return 0;
}
