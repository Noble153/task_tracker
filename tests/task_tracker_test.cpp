#include <gtest/gtest.h>
#include "task_tracker.h"
#include <thread>
#include <chrono>

namespace tt = task_tracker;

class TaskTrackerTest : public ::testing::Test {
protected:
    tt::TaskTracker tracker;
    
    void SetUp() override {
        // Код, выполняемый перед каждым тестом
    }
    
    void TearDown() override {
        // Код, выполняемый после каждого теста
    }
};

// Базовый тест добавления задачи
TEST_F(TaskTrackerTest, AddTask_SingleTask) {
    tracker.AddTask("Test task");
    
    const auto& all_tasks = tracker.GetTasks();
    ASSERT_EQ(all_tasks.size(), 1);
    EXPECT_EQ(all_tasks[0]->description, "Test task");
    EXPECT_EQ(all_tasks[0]->status, tt::Status::TODO);
    EXPECT_EQ(all_tasks[0]->id, 1);
}

// Тест генерации ID
TEST_F(TaskTrackerTest, AddTask_IncrementsId) {
    tracker.AddTask("Task 1");
    tracker.AddTask("Task 2");
    tracker.AddTask("Task 3");
    
    const auto& all_tasks = tracker.GetTasks();
    EXPECT_EQ(all_tasks[0]->id, 1);
    EXPECT_EQ(all_tasks[1]->id, 2);
    EXPECT_EQ(all_tasks[2]->id, 3);
}

// Тест получения задач по статусу
TEST_F(TaskTrackerTest, GetTasksByStatus) {
    tracker.AddTask("Task 1");
    tracker.AddTask("Task 2");
    tracker.AddTask("Task 3");
    
    // Изначально все задачи в TODO
    EXPECT_EQ(tracker.GetToDoTasks().size(), 3);
    EXPECT_EQ(tracker.GetInProgressTasks().size(), 0);
    EXPECT_EQ(tracker.GetDoneTasks().size(), 0);
    
    // Меняем статусы
    tracker.UpdateTask(1, "", "in-progress");
    tracker.UpdateTask(2, "", "done");
    
    EXPECT_EQ(tracker.GetToDoTasks().size(), 1); // Task 3
    EXPECT_EQ(tracker.GetInProgressTasks().size(), 1); // Task 1
    EXPECT_EQ(tracker.GetDoneTasks().size(), 1); // Task 2
}

// Тест обновления описания
TEST_F(TaskTrackerTest, UpdateTask_Description) {
    tracker.AddTask("Original description");
    
    std::string original_updated_at = tracker.GetTasks()[0]->updated_at;
    
    // Небольшая задержка для различия во времени
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    tracker.UpdateTask(1, "Updated description", "");
    
    const auto& tasks = tracker.GetTasks();
    EXPECT_EQ(tasks[0]->description, "Updated description");
    EXPECT_EQ(tasks[0]->status, tt::Status::TODO);
    EXPECT_NE(tasks[0]->updated_at, original_updated_at);
}

// Тест обновления статуса
TEST_F(TaskTrackerTest, UpdateTask_Status) {
    tracker.AddTask("Test task");
    
    // TODO -> IN_PROGRESS
    tracker.UpdateTask(1, "", "in-progress");
    
    auto& tasks = tracker.GetTasks();
    EXPECT_EQ(tasks[0]->status, tt::Status::IN_PROGRESS);
    EXPECT_EQ(tracker.GetToDoTasks().size(), 0);
    EXPECT_EQ(tracker.GetInProgressTasks().size(), 1);
    
    // IN_PROGRESS -> DONE
    tracker.UpdateTask(1, "", "done");
    EXPECT_EQ(tasks[0]->status, tt::Status::DONE);
    EXPECT_EQ(tracker.GetInProgressTasks().size(), 0);
    EXPECT_EQ(tracker.GetDoneTasks().size(), 1);
    
    // DONE -> TODO
    tracker.UpdateTask(1, "", "todo");
    EXPECT_EQ(tasks[0]->status, tt::Status::TODO);
    EXPECT_EQ(tracker.GetDoneTasks().size(), 0);
    EXPECT_EQ(tracker.GetToDoTasks().size(), 1);
}

// Тест обновления и описания, и статуса
TEST_F(TaskTrackerTest, UpdateTask_BothDescriptionAndStatus) {
    tracker.AddTask("Original");
    
    tracker.UpdateTask(1, "Updated", "in-progress");
    
    const auto& tasks = tracker.GetTasks();
    EXPECT_EQ(tasks[0]->description, "Updated");
    EXPECT_EQ(tasks[0]->status, tt::Status::IN_PROGRESS);
}

// Тест обновления несуществующей задачи
TEST_F(TaskTrackerTest, UpdateTask_NonExistent_Throws) {
    EXPECT_THROW({
        tracker.UpdateTask(999, "New description", "todo");
    }, std::out_of_range);
}

// Тест удаления задачи
TEST_F(TaskTrackerTest, DeleteTask_Exists) {
    tracker.AddTask("Task 1");
    tracker.AddTask("Task 2");
    tracker.AddTask("Task 3");
    
    tracker.UpdateTask(2, "", "in-progress");
    
    EXPECT_EQ(tracker.GetTasks().size(), 3);
    
    tracker.DeleteTask(2); // Удаляем задачу в IN_PROGRESS
    
    EXPECT_EQ(tracker.GetTasks().size(), 2);
    EXPECT_EQ(tracker.GetInProgressTasks().size(), 0);
    
    // Проверяем, что остались задачи 1 и 3
    const auto& tasks = tracker.GetTasks();
    bool found_task1 = false;
    bool found_task3 = false;
    
    for (const auto& task : tasks) {
        if (task->id == 1) found_task1 = true;
        if (task->id == 3) found_task3 = true;
    }
    
    EXPECT_TRUE(found_task1);
    EXPECT_TRUE(found_task3);
}

// Тест удаления несуществующей задачи
TEST_F(TaskTrackerTest, DeleteTask_NonExistent_DoesNothing) {
    tracker.AddTask("Task");
    
    size_t initial_size = tracker.GetTasks().size();
    
    tracker.DeleteTask(999); // Несуществующий ID
    
    EXPECT_EQ(tracker.GetTasks().size(), initial_size);
}

// Тест пустых параметров в UpdateTask
TEST_F(TaskTrackerTest, UpdateTask_EmptyParameters) {
    tracker.AddTask("Task");
    
    std::string original_description = tracker.GetTasks()[0]->description;
    std::string original_updated_at = tracker.GetTasks()[0]->updated_at;
    
    // Пустые параметры - ничего не должно измениться
    tracker.UpdateTask(1, "", "");
    
    const auto& tasks = tracker.GetTasks();
    EXPECT_EQ(tasks[0]->description, original_description);
    EXPECT_EQ(tasks[0]->updated_at, original_updated_at);
}

// Тест сложного сценария
TEST_F(TaskTrackerTest, ComplexScenario) {
    // Добавляем задачи
    tracker.AddTask("Design database");
    tracker.AddTask("Implement API");
    tracker.AddTask("Write tests");
    
    // Проверяем начальное состояние
    EXPECT_EQ(tracker.GetTasks().size(), 3);
    
    // Меняем статусы
    tracker.UpdateTask(1, "", "in-progress");
    tracker.UpdateTask(2, "Implement REST API", "in-progress");
    
    EXPECT_EQ(tracker.GetToDoTasks().size(), 1);
    EXPECT_EQ(tracker.GetInProgressTasks().size(), 2);
    
    // Завершаем задачу
    tracker.UpdateTask(1, "", "done");
    
    EXPECT_EQ(tracker.GetInProgressTasks().size(), 1);
    EXPECT_EQ(tracker.GetDoneTasks().size(), 1);
    
    // Удаляем задачу
    tracker.DeleteTask(3);
    
    EXPECT_EQ(tracker.GetTasks().size(), 2);
    EXPECT_EQ(tracker.GetToDoTasks().size(), 0);
    
    // Проверяем итоговые данные
    const auto& done_tasks = tracker.GetDoneTasks();
    ASSERT_EQ(done_tasks.size(), 1);
    EXPECT_EQ(done_tasks[0]->id, 1);
    EXPECT_EQ(done_tasks[0]->description, "Design database");
}

// Тест порядка задач в списках
TEST_F(TaskTrackerTest, TaskOrderInLists) {
    for (int i = 1; i <= 5; i++) {
        tracker.AddTask("Task " + std::to_string(i));
    }
    
    // Меняем статусы некоторых задач
    tracker.UpdateTask(2, "", "done");
    tracker.UpdateTask(4, "", "in-progress");
    
    // Проверяем порядок в общем списке
    const auto& all_tasks = tracker.GetTasks();
    for (size_t i = 0; i < all_tasks.size(); i++) {
        EXPECT_EQ(all_tasks[i]->id, static_cast<int>(i + 1));
    }
    
    // Проверяем, что задачи в правильных списках
    EXPECT_EQ(tracker.GetToDoTasks().size(), 3); // 1, 3, 5
    EXPECT_EQ(tracker.GetInProgressTasks().size(), 1); // 4
    EXPECT_EQ(tracker.GetDoneTasks().size(), 1); // 2
}

// Тест получения константных ссылок
TEST_F(TaskTrackerTest, GetMethods_ReturnValidReferences) {
    tracker.AddTask("Task");
    
    // Проверяем, что методы возвращают ссылки на реальные данные
    const auto& ref1 = tracker.GetTasks();
    const auto& ref2 = tracker.GetToDoTasks();
    
    EXPECT_FALSE(ref1.empty());
    EXPECT_FALSE(ref2.empty());
    EXPECT_EQ(ref1[0].get(), ref2[0].get()); // Должны указывать на один объект
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}