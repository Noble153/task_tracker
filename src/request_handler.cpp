#include "request_handler.h"

namespace request_handler{

/*---------------------------------------------------------------------------------
                                PUBLIC
---------------------------------------------------------------------------------*/
RequestHandler::RequestHandler(string filename, TaskTracker& tt, json& j_data): filename_(filename), tt_(tt), j_data_(j_data) {
    LoadFromFile();
}

void RequestHandler::AddRequestProcessing(string description) {
    tt_.AddTask(description);
    AutoSave();
}

void RequestHandler::UpdateRequestProcessing(int id, string description, string status) {
    tt_.UpdateTask(id, description, status);
    AutoSave();
}

void RequestHandler::DeleteRequestProcessing(int id) {
    tt_.DeleteTask(id);
    AutoSave();
}

void RequestHandler::ListRequestProcessing(ostream& out) {
    out << j_data_.dump(4);
}

void RequestHandler::AutoSave() {
    SaveToFile();
}

/*---------------------------------------------------------------------------------
                                PRIVATE
---------------------------------------------------------------------------------*/

void RequestHandler::LoadFromFile() {
    ifstream file(filename_);
    if(!file.is_open()) {
        return;
    }

    // if (!file.is_open()) {
    //     std::cerr << "File " << "\"" << filename_ << "\"" << "does not open!" << std::endl;
    //     return;
    // }

    j_data_ = json::parse(file);

    if(j_data_.contains("tasks") && j_data_["tasks"].is_array())
    {
        for(const auto& curr_task: j_data_["tasks"]) {
            shared_ptr<Task> task_ptr = make_shared<Task>(from_json(curr_task));
            tt_.AddTask(task_ptr);
        }
    }

    if(j_data_.contains("next_id"))
    {
        tt_.SetID(j_data_["next_id"].get<int>());
    }
    else {
        // Находим максимальный ID
        int max_id = 0;
        for (const auto& task : tt_.GetTasks()) {
            if (task->id > max_id) {
                max_id = task->id;
            }
        }
        tt_.SetID(max_id + 1);
    }
}

void RequestHandler::SaveToFile() {
    j_data_.clear();

    json task_array = json::array();
    for(const auto& task: tt_.GetTasks()) {
        task_array.push_back(to_json(task));
    }
    j_data_["tasks"] = task_array;
    
    // Сохраняем следующий ID
    j_data_["next_id"] = tt_.GetID();
    
    // Записываем в файл
    std::ofstream file(filename_);
    // if (!file.is_open()) {
    //     std::cerr << "Ошибка: не могу открыть файл " << filename_ << " для записи" << std::endl;
    //     return;
    // }
    
    file << j_data_.dump(4); // dump(4) для красивого форматирования с отступами
    file.close();    
}

// void RequestHandler::RequestProcessing(std::string_view mode) {

// }
    
// /*---------------------------------------------------------------------------------
//                                 Загрузка/Сохранение
// ---------------------------------------------------------------------------------*/
// void RequestHandler::LoadFromFile() {
//     std::ifstream file(filename_);
//     if (!file.is_open()) {
//         std::cout << "Файл " << filename_ << " не найден. Создаю новый." << std::endl;
//         return;
//     }
    
//     try {
//         nlohmann::json j;
//         file >> j;
        
//         // Очищаем текущие данные
//         all_tasks_.clear();
//         to_do_tasks_.clear();
//         in_prodress_tasks_.clear();
//         done_tasks_.clear();
//         ids_to_tasks_.clear();
        
//         // Загружаем задачи
//         if (j.contains("tasks") && j["tasks"].is_array()) {
//             for (const auto& task_json : j["tasks"]) {
//                 Task task = Task::from_json(task_json);
//                 auto task_ptr = std::make_shared<Task>(std::move(task));
                
//                 // Добавляем во все контейнеры
//                 all_tasks_.push_back(task_ptr);
//                 ids_to_tasks_[task_ptr->id] = task_ptr;
                
//                 // Добавляем в соответствующий контейнер по статусу
//                 switch (task_ptr->status) {
//                     case Status::TODO:
//                         to_do_tasks_.push_back(task_ptr);
//                         break;
//                     case Status::IN_PROGRESS:
//                         in_prodress_tasks_.push_back(task_ptr);
//                         break;
//                     case Status::DONE:
//                         done_tasks_.push_back(task_ptr);
//                         break;
//                 }
//             }
//         }
        
//         // Восстанавливаем счетчик ID
//         if (j.contains("next_id")) {
//             task_id_ = j["next_id"].get<int>();
//         } else {
//             // Находим максимальный ID
//             int max_id = 0;
//             for (const auto& task : all_tasks_) {
//                 if (task->id > max_id) max_id = task->id;
//             }
//             task_id_ = max_id + 1;
//         }
        
//         std::cout << "Загружено " << all_tasks_.size() << " задач из файла " << filename_ << std::endl;
        
//     } catch (const std::exception& e) {
//         std::cerr << "Ошибка загрузки файла: " << e.what() << std::endl;
//         std::cerr << "Начинаю с пустого списка задач." << std::endl;
//     }
// }

// void RequestHandler::SaveToFile() const {
//     nlohmann::json j;
    
//     // Сохраняем задачи
//     nlohmann::json tasks_array = nlohmann::json::array();
//     for (const auto& task : all_tasks_) {
//         tasks_array.push_back(task->to_json());
//     }
//     j["tasks"] = tasks_array;
    
//     // Сохраняем следующий ID
//     j["next_id"] = task_id_;
    
//     // Записываем в файл
//     std::ofstream file(filename_);
//     if (!file.is_open()) {
//         std::cerr << "Ошибка: не могу открыть файл " << filename_ << " для записи" << std::endl;
//         return;
//     }
    
//     file << j.dump(4); // dump(4) для красивого форматирования с отступами
//     file.close();
// }

// void RequestHandler::AutoSave() const {
//     SaveToFile();
// }

// /*---------------------------------------------------------------------------------
//                                 Публичные методы
// ---------------------------------------------------------------------------------*/
// // void TaskTrecker::AddTask(const std::string& task_description) {
// //     auto task = std::make_shared<Task>(GenerateId(),
// //                                         task_description,
// //                                         Status::TODO,
// //                                         GetDate(),
// //                                         GetDate());
    
// //     all_tasks_.push_back(task);
// //     to_do_tasks_.push_back(task);
// //     ids_to_tasks_[task.get()->id] = task;
    
// //     AutoSave(); // Сохраняем после добавления
// // }

// // void TaskTrecker::UpdateTask(int id, std::string description, std::string status) {
// //     bool desc_empty = description.empty();
// //     bool st_emty = status.empty();
// //     if(!desc_empty || !st_emty) {
// //         auto task = ids_to_tasks_.at(id);
// //         if(!desc_empty) {
// //             UpdateTaskDescription(task, description);
// //         }
// //         if(!st_emty) {
// //             UpdateTaskStatus(task, status);
// //         }

// //         task.get()->updated_at = GetDate();
// //         AutoSave(); // Сохраняем после обновления
// //     }
// // }

// // void TaskTrecker::DeleteTask(int id) {
// //     if(!ids_to_tasks_.count(id)) {
// //         return;
// //     }

// //     auto current_task = ids_to_tasks_.at(id);
// //     Status status = current_task.get()->status;
// //     switch (status)
// //     {
// //     case Status::TODO:
// //         DeleteTaskFromToDoTasks(current_task);
// //         break;

// //     case Status::IN_PROGRESS:
// //         DeleteTaskFromAInProgressTasks(current_task);
// //         break;

// //     case Status::DONE:
// //         DeleteTaskFromDoneTasks(current_task);
// //         break;
// //     }

// //     DeleteTaskFromAllTasks(current_task);

// //     ids_to_tasks_.erase(ids_to_tasks_.find(id));
    
// //     AutoSave(); // Сохраняем после удаления
// // }

// // Остальные методы остаются без изменений...

}
