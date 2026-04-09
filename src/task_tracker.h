#pragma once

#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <ctime>
#include <unordered_map>
#include <algorithm>
#include <nlohmann/json.hpp>

#include "domain.h"

namespace task_tracker {
using namespace std;
using namespace std::literals;

class TaskTracker {
public:
    TaskTracker() = default;

    void AddTask(const std::string& task_description);
    void AddTask(shared_ptr<Task> task);
    /**
     * Can throw exception
     */
    void UpdateTask(int id, std::string description = ""s, std::string status = ""s);
    void DeleteTask(int id);

    void SetID(int id);

    int GetID() const;

    const std::deque<std::shared_ptr<Task>>& GetTasks() const noexcept;
    // std::deque<std::shared_ptr<Task>>& GetTasks() noexcept;
    const std::deque<std::shared_ptr<Task>>& GetDoneTasks() const noexcept;
    // std::deque<std::shared_ptr<Task>>& GetDoneTasks() noexcept;
    const std::deque<std::shared_ptr<Task>>& GetInProgressTasks() const noexcept;
    // std::deque<std::shared_ptr<Task>>& GetInProgressTasks() noexcept;
    const std::deque<std::shared_ptr<Task>>& GetToDoTasks() const noexcept;
    // std::deque<std::shared_ptr<Task>>& GetToDoTasks() noexcept;

private:
    int GenerateId();
    std::string GetDate();

    void UpdateTaskStatus(std::shared_ptr<Task> task, std::string status);
    void UpdateTaskDescription(std::shared_ptr<Task> task, std::string description);

    void DeleteTaskFromAllTasks(std::shared_ptr<Task> task);
    void DeleteTaskFromToDoTasks(std::shared_ptr<Task> task);
    void DeleteTaskFromAInProgressTasks(std::shared_ptr<Task> task);
    void DeleteTaskFromDoneTasks(std::shared_ptr<Task> task);

private:
    int task_id_= 1;

    std::deque<std::shared_ptr<Task>> all_tasks_;
    std::deque<std::shared_ptr<Task>> to_do_tasks_;
    std::deque<std::shared_ptr<Task>> in_prodress_tasks_;
    std::deque<std::shared_ptr<Task>> done_tasks_;
    std::unordered_map<int, std::shared_ptr<Task>> ids_to_tasks_;
};
}