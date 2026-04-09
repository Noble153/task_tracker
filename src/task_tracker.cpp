#include "task_tracker.h"

namespace task_tracker{

/*---------------------------------------------------------------------------------
                                PUBLIC
---------------------------------------------------------------------------------*/
void TaskTracker::AddTask(const std::string& task_description) {
    auto task = std::make_shared<Task>(GenerateId(),
                                        task_description,
                                        Status::TODO,
                                        GetDate(),
                                        GetDate());
    
    all_tasks_.push_back(task);
    to_do_tasks_.push_back(task);
    ids_to_tasks_[task.get()->id] = task;
}

void TaskTracker::AddTask(shared_ptr<Task> task) {
    all_tasks_.push_back(task);
    ids_to_tasks_[task->id] = task;

    switch(task->status) {
        case Status::TODO:
            to_do_tasks_.push_back(task);
            break;
        case Status::IN_PROGRESS:
            in_prodress_tasks_.push_back(task);
            break;
        case Status::DONE:
            done_tasks_.push_back(task);
            break;
    }


}

void TaskTracker::UpdateTask(int id, std::string description, std::string status) {
    // bool desc_empty = description.empty();
    // bool st_emty = status.empty();
    // if(desc_empty != true || st_emty != true) {
    //     auto task = ids_to_tasks_.at(id);
    //     if(!desc_empty) {
    //         UpdateTaskDescription(task, description);
    //     }
    //     if(!st_emty) {
    //         UpdateTaskStatus(task, status);
    //     }

    //     task.get()->updated_at = GetDate();
    // }


    if(!description.empty()) {
        UpdateTaskDescription(ids_to_tasks_.at(id), description);
    }
    if(!status.empty()) {
        UpdateTaskStatus(ids_to_tasks_.at(id), status);
    }
    ids_to_tasks_.at(id).get()->updated_at = GetDate();
}

void TaskTracker::DeleteTask(int id) {
    if(!ids_to_tasks_.count(id)) {
        return;
    }

    auto current_task = ids_to_tasks_.at(id);
    Status status = current_task.get()->status;
    switch (status)
    {
    case Status::TODO:
        DeleteTaskFromToDoTasks(current_task);
        break;

    case Status::IN_PROGRESS:
        DeleteTaskFromAInProgressTasks(current_task);
        break;

    case Status::DONE:
        DeleteTaskFromDoneTasks(current_task);
        break;
    }

    DeleteTaskFromAllTasks(current_task);

    ids_to_tasks_.erase(ids_to_tasks_.find(id));
}

void TaskTracker::SetID(int id) {
    task_id_ = id;
}

int TaskTracker::GetID() const {
    return task_id_;
}

const std::deque<std::shared_ptr<Task>>& TaskTracker::GetTasks() const noexcept {
    return all_tasks_;
}

const std::deque<std::shared_ptr<Task>>& TaskTracker::GetDoneTasks() const noexcept {
    return done_tasks_;
}

const std::deque<std::shared_ptr<Task>>& TaskTracker::GetInProgressTasks() const noexcept {
    return in_prodress_tasks_;
}

const std::deque<std::shared_ptr<Task>>& TaskTracker::GetToDoTasks() const noexcept {
    return to_do_tasks_;
}

/*---------------------------------------------------------------------------------
                                PRIVATE
---------------------------------------------------------------------------------*/

int TaskTracker::GenerateId() {
    return task_id_++;
}

std::string TaskTracker::GetDate() {
    std::time_t t = std::time(NULL);
    char buf[20];
    std::strftime(buf, 20, "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    std::string date_str = buf;

    return date_str;
}

void TaskTracker::UpdateTaskStatus(std::shared_ptr<Task> task, std::string status) {
    Status current_status = task.get()->status;
    switch (current_status)
    {
    case Status::TODO:
        DeleteTaskFromToDoTasks(task);
        break;

    case Status::IN_PROGRESS:
        DeleteTaskFromAInProgressTasks(task);
        break;

    case Status::DONE:
        DeleteTaskFromDoneTasks(task);
        break;
    }

    Status new_status = StrToStatus(status);
    task.get()->status = new_status;
    switch (new_status)
    {
    case Status::TODO:
        to_do_tasks_.push_back(task);
        break;

    case Status::IN_PROGRESS:
        in_prodress_tasks_.push_back(task);
        break;

    case Status::DONE:
        done_tasks_.push_back(task);
        break;
    }
}

void TaskTracker::UpdateTaskDescription(std::shared_ptr<Task> task, std::string description) {
    task.get()->description = description;
}

void TaskTracker::DeleteTaskFromAllTasks(std::shared_ptr<Task> task) {
    auto it = std::find(all_tasks_.begin(), all_tasks_.end(), task);
    all_tasks_.erase(it);
}

void TaskTracker::DeleteTaskFromToDoTasks(std::shared_ptr<Task> task) {
    auto it = std::find(to_do_tasks_.begin(), to_do_tasks_.end(), task);
    to_do_tasks_.erase(it);
}

void TaskTracker::DeleteTaskFromAInProgressTasks(std::shared_ptr<Task> task) {
    auto it = std::find(in_prodress_tasks_.begin(), in_prodress_tasks_.end(), task);
    in_prodress_tasks_.erase(it);
}

void TaskTracker::DeleteTaskFromDoneTasks(std::shared_ptr<Task> task) {
    auto it = std::find(done_tasks_.begin(), done_tasks_.end(), task);
    done_tasks_.erase(it);
}

}