cmake_minimum_required(VERSION 3.14)
project(FTXUIExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thêm thư viện FTXUI
include(FetchContent)
FetchContent_Declare(
    FTXUI
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG main
)

FetchContent_MakeAvailable(FTXUI)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE ftxui::screen ftxui::dom ftxui::component)
