#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#

struct Metadata {
    std::string inputFile;
    std::string outputFile;
    std::vector<size_t> columns;
    std::vector<std::vector<std::string>> table;
};

void exitProgram(int exitCode, const std::string& message){
    std::cerr << message << "\n";
    exit(exitCode);
}

std::vector<size_t> parseColumnNumbers(std::string& rawColumns){
    std::vector<size_t> columns;
    bool wasNumber = false;
    size_t begin = 0;
    for(size_t i = 0; i < rawColumns.size(); i++){
        if ((rawColumns[i] > '0' && rawColumns[i] <= '9') || (rawColumns[i] == '0' && wasNumber)){
            begin = wasNumber ? begin : i;
            wasNumber = true;
        } else if (rawColumns[i] == ',') {
            if (!wasNumber || i == rawColumns.size()-1)
                exitProgram(1,"3rd argument has invalid format.");
            std::string subst = rawColumns.substr(begin,i-begin);
            columns.push_back(std::stoll(subst));
            wasNumber = false;
        } else {
            exitProgram(1, "3rd argument has invalid format.");
        }
    }
    std::string subst = rawColumns.substr(begin);
    columns.push_back(std::stoll(subst));
    return columns;
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
    metadata.columns = parseColumnNumbers(str);

}


int main(int argc, char* argv[]) {
    readData(argc,argv);
    return 0;
}
