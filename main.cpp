#include <iostream>
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>

#include "src/task_tracker.h"
#include "src/request_handler.h"

inline void PrintUsage(std::ostream& out = std::cerr) {
    out << "Usage: transport_catalogue [add|update|delete|list] "
        << "[\"task_description\"|\"task_id\" \"description status\" or \"description\" or \"status\"|\"task_id\"]" << std::endl;
}

int main(int argc, char* argv[]) {
    using json = nlohmann::json;
    using namespace std;
    using namespace literals;
    using namespace task_tracker;
    using namespace request_handler;

    if(argc < 2) {
        PrintUsage();
    }

    string filename = "task.json";
    json data;
    TaskTracker task_tracker;
    
    // RequestHandler rh;
    RequestHandler rh(filename, task_tracker, data);

    std::string_view program_mode = argv[1];
    // Request request;
    // request.mode = program_mode;
    if(program_mode == "add"sv) {
        rh.AddRequestProcessing(argv[2]);
    }
    else if(program_mode == "update"sv) {
        int id = stoi(argv[2]);
        string description;
        string status;
        for(int i = 3; i < argc; ++i) {
            if(IsStatus(argv[i])) {
                status = argv[i];
            }
            else {
                description = argv[i];
            }
        }
        rh.UpdateRequestProcessing(id, description, status);
    }
    else if(program_mode == "delete") {
        int id = stoi(argv[2]);
        rh.DeleteRequestProcessing(id);
    }
    else if(program_mode == "list") {
        std::ofstream out("list.txt");
        rh.ListRequestProcessing(out);
    }
    else {
        PrintUsage();
    }

    // {
    //     using json = nlohmann::json;

    //     json data;
        

    //     std::string str_dump = data.dump();

    //     std::cout << str_dump << std::endl;
    //     std::cout << "Hello, world!" << std::endl;
    // }


}