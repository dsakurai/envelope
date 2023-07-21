cmake_minimum_required(VERSION 3.25)
project(envelope)

set(CMAKE_CXX_STANDARD 23)

find_package(CGAL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(envelope main.cpp)

target_link_libraries(envelope
        PRIVATE
        CGAL::CGAL nlohmann_json::nlohmann_json
        )