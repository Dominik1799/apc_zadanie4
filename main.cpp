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
};

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
            num = std::stoll(subst);
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
    num = std::stoll(subst);
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

void readData(int argc, char* argv[]){
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
            exitProgram(1,"Invalid input data form.");
        }
        prevSize = vec.size();
        metadata.table.push_back(vec);
    }
    if (metadata.table[0].empty())
        exitProgram(1,"Invalid input data form.");
    for (auto & i : metadata.table) {
        for (auto & j : i) {
            std::cout << j << " ";
        }
        std::cout << "\n";
    }
}


int main(int argc, char* argv[]) {
    readData(argc,argv);
    return 0;
}
