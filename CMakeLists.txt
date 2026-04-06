cmake_minimum_required(VERSION 3.20)

project(PASS VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_compile_options(/O2 /Oi /Ot)
endif()


add_subdirectory(_3rd_party/rtaudio)

add_executable(${PROJECT_NAME})


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/_3rd_party/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/_3rd_party/SDL/include
    ${CMAKE_CURRENT_SOURCE_DIR}/_3rd_party/SDL_TTF/include
    ${CMAKE_CURRENT_SOURCE_DIR}/_3rd_party/rtaudio/include
)


target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/_3rd_party/SDL/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/_3rd_party/SDL_TTF/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3
    SDL3_ttf
    rtaudio
)


file(GLOB_RECURSE IMGUI_SOURCES "_3rd_party/imgui/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES})


add_subdirectory(source)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL3.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:rtaudio>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

