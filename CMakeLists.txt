cmake_minimum_required(VERSION 3.5)
project(REPLACE_WITH_YOUR_PROJECT_NAME)

# let claude cook
# Suppress the policy version check for subdirectories
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    ptsd

    URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip

    # Or alternatively if you want to use a specific git commit instead of a release:
    # GIT_REPOSITORY  https://github.com/ntut-open-source-club/practical-tools-for-simple-design.git
    # GIT_TAG         <git_tag>

    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)

include(files.cmake)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
list(TRANSFORM SRC_FILES PREPEND ${SRC_DIR}/)
list(TRANSFORM INCLUDE_FILES PREPEND ${INCLUDE_DIR}/)

# file(GLOB_RECURSE SRC_FILES src/*)
# file(GLOB_RECURSE HEADER_FILES include/*)

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
        include/items/Shape.hpp
        include/items/Quad.hpp
        src/items/Shape.cpp
        src/items/Quad.cpp
        include/buildings/Machine.hpp
        include/buildings/Belt.hpp
        include/items/Item.hpp
        src/items/Item.cpp
        src/buildings/Belt.cpp
        src/buildings/Machine.cpp
        include/World.hpp
        include/buildings/Miner.hpp
        include/buildings/ItemAcceptor.hpp
        include/buildings/ItemEjector.hpp
        src/buildings/ItemEjector.cpp
        src/buildings/Miner.cpp
        src/buildings/ItemAcceptor.cpp
        include/buildings/Trash.hpp
        src/buildings/Trash.cpp
        include/buildings/Rotator.hpp
        src/buildings/Rotator.cpp
        include/buildings/Cutter.hpp
        src/buildings/Cutter.cpp
        include/buildings/Hub.hpp
        src/buildings/Hub.cpp
        include/UIElement/Text.hpp
        include/BigNumStr.hpp
        include/buildings/Balancer.hpp
        src/buildings/Balancer.cpp
        include/buildings/Tunnel.hpp
        src/buildings/Tunnel.cpp
        include/items/Color.hpp
        src/items/Color.cpp
        include/buildings/Stacker.hpp
        src/buildings/Stacker.cpp
        include/buildings/Painter.hpp
        src/buildings/Painter.cpp
        include/Test.hpp
        include/buildings/Mixer.hpp
        src/buildings/Mixer.cpp
        include/UIElement/Button.hpp
        src/UIelement/Button.cpp
        include/scenes/GameScene.hpp
        src/scenes/GameScene.cpp
        src/World.cpp
        include/scenes/Scene.hpp
        include/scenes/TitleScene.hpp
        src/scenes/TitleScene.cpp
        include/AssetLoader.hpp
        src/AssetLoader.cpp
        include/Opti/OptiRenderer.hpp
        src/Opti/OptiRenderer.cpp
        src/Opti/OptiObject.cpp
        include/Opti/OptiObject.hpp
        include/Opti/OptiImage.hpp
        src/Opti/OptiImage.cpp
        include/ThreadPool.hpp
        include/WaitGroup.hpp
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    message(FATAL_ERROR "relative RESOURCE_DIR is WIP, Please use `-DCMAKE_BUILD_TYPE=Debug` build for now.")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    PTSD
)
