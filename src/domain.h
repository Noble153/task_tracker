#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

namespace task_tracker {

using namespace std;
using json = nlohmann::json;

enum class Status {
    TODO,
    IN_PROGRESS,
    DONE
};

inline bool IsStatus(string status) {
    if(status == "todo"s || status == "in-progress"s || status == "done"s) {
        return true;
    }
    return false;
}

inline std::string StatusToStr(Status status) {
    switch(status) {
        case Status::TODO: return "todo";
        case Status::IN_PROGRESS: return "in-progress";
        case Status::DONE: return "done";
        default: return "todo";
    }
}

inline Status StrToStatus(const std::string& str) {
    if (str == "todo") return Status::TODO;
    if (str == "in-progress") return Status::IN_PROGRESS;
    if (str == "done") return Status::DONE;
    return Status::TODO;
}

struct Task {
    int id;                             // id. Должен быть уникальным.
    string description;            // Краткое описание задачи.
    Status status;                      // Статус задачи.
    string created_at;             // Дата и время создания задачи.
    string updated_at;             // Дата и врямя последнего обновления задачи

    Task() = default;

    Task(int id, std::string description, Status status, string created_at, string updated_at)
        : id(id), description(description), status(status), created_at(created_at), updated_at(updated_at) {}
};

// ostream& operator<<(ostream &out, const Task &person) {
    
// }

inline Task from_json(const json& j) {
    Task task;

    j.at("id").get_to(task.id);
    j.at("description").get_to(task.description);

    string status_str;
    j.at("status").get_to(status_str);
    task.status = StrToStatus(status_str);

    j.at("created_at").get_to(task.created_at);
    j.at("updated_at").get_to(task.updated_at);

    return task;
}

inline json to_json(const Task& task) {
    json j = { {"id", task.id},
               {"description", task.description},
               {"status", StatusToStr(task.status)},
               {"created_at", task.created_at},
               {"updated_at", task.updated_at} };
    return j;
}

inline json to_json(shared_ptr<Task> task_ptr) {
    json j = { {"id", task_ptr->id},
               {"description", task_ptr->description},
               {"status", StatusToStr(task_ptr->status)},
               {"created_at", task_ptr->created_at},
               {"updated_at", task_ptr->updated_at} };
    return j;
}

}