cmake_minimum_required(VERSION 3.15)

add_definitions(-D__CMAKE_ROOT_DIRECTORY__="${CMAKE_CURRENT_SOURCE_DIR}/..")

add_executable(${PROJECT_NAME}
    Main.cc

    Node/Node.cc

    CommandHandler/CommandHandler.cc
)

target_link_libraries(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PUBLIC .)