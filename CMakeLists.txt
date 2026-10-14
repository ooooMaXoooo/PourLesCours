cmake_minimum_required(VERSION 3.20)
project(PourLesCours VERSION 1.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force MSVC à lier la librairie standard statiquement pour TOUS les sous-projets (y compris GLFW)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include(FetchContent)

# 1. Récupération de GLFW
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# 2. Récupération de Dear ImGui
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.9
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp

    "./src/App.cpp"

    "./src/features/note_converter/NoteConverter.cpp"
    "./src/features/student_ranking/StudentRanking.cpp"

    "./src/dataclasses/Class.cpp"
    "./src/dataclasses/Student.cpp"
)

if(WIN32)
    set(RESOURCE_FILES app_icon.rc)
endif()

add_executable(PourLesCours WIN32 main.cpp ${IMGUI_SOURCES} ${RESOURCE_FILES})

# Passe le nom du projet défini par la commande project() au code C++
target_compile_definitions(PourLesCours PRIVATE APP_NAME="${PROJECT_NAME}")

target_include_directories(PourLesCours PRIVATE 
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends

    "./include"
)

target_link_libraries(PourLesCours PRIVATE glfw)

# 3. Édition des liens
if(WIN32)
    target_link_libraries(PourLesCours PRIVATE opengl32)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_options(PourLesCours PRIVATE -static -static-libgcc -static-libstdc++)
    endif()
elseif(APPLE)
    target_link_libraries(PourLesCours PRIVATE "-framework OpenGL")
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(PourLesCours PRIVATE OpenGL::GL)
endif()