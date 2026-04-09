cmake_minimum_required(VERSION 3.14)

project(task_tracker CXX)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подпроект для тестов (если они есть)
# if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
#     add_subdirectory(tests)
# endif()

set(SRC src/domain.h 
        src/request_handler.h src/request_handler.cpp
        src/task_tracker.h src/task_tracker.cpp)

find_package(nlohmann_json 3.10.5 REQUIRED)

add_executable(tasktracker main.cpp ${SRC})

target_link_libraries(tasktracker PUBLIC nlohmann_json::nlohmann_json)