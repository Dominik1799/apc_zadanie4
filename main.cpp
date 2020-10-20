#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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
    std::vector<std::string> result;
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
    while (std::getline(input,buffer)){
        std::cout << buffer;
    }

}


int main(int argc, char* argv[]) {
    readData(argc,argv);
    return 0;
}
