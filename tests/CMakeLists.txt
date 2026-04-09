# Находим GoogleTest
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Создаем исполняемый файл тестов
add_executable(task_tracker_tests
    task_tracker_test.cpp
)

# Линкуем с нашей библиотекой и GoogleTest
target_link_libraries(task_tracker_tests
    task_tracker_lib
    GTest::gtest
    GTest::gtest_main
    Threads::Threads
)

# Добавляем include директории
target_include_directories(task_tracker_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Добавляем тесты в CTest
include(GoogleTest)
gtest_discover_tests(task_tracker_tests)