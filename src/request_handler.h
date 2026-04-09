#pragma once

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include "task_tracker.h"
#include "domain.h"


namespace request_handler{

using json = nlohmann::json;
using namespace std;
using namespace task_tracker;

// struct Request {
//     string_view mode;
//     int id;
//     string description;
//     string status;
// };

class RequestHandler {
public:
    // RequestHandler() = default;

    RequestHandler(string filename, TaskTracker& tt, json& j_data);

    void AddRequestProcessing(string description);
    void UpdateRequestProcessing(int id, string description, string status);
    void DeleteRequestProcessing(int id);
    void ListRequestProcessing(ostream& out = cout);

    void AutoSave();

private:
    void LoadFromFile();
    void SaveToFile();

private:
    string filename_;
    // RequestHandler использует агрегацию объектов
    TaskTracker& tt_;
    json& j_data_;
};

}
