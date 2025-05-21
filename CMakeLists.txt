cmake_minimum_required(VERSION 3.8)
project("External Gaming Library")

function(add_public_library name path)
    add_library(${name} ${path})
    target_include_directories(${name} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:include>")
endfunction()

function(link_sfml target)
    target_link_libraries(${target} PUBLIC SFML3)
endfunction()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# #specify output directories for libraries and executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")#for executables (.exe)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")#for static libraries (.lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")#for shared libraries (.dll or .so)

#setting the relative path of our resources folder (contains our various files such as .pngs) to be easily used in code
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
add_definitions(-DRESOURCES_DIR="${RESOURCES_DIR}")

#setting our project name as a defenition so it can be used in our source files
set(PROJECT_NAME "${PROJECT_NAME}")
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

#adding libraries
#SFML library
set(SFML_STATIC_LIBRARIES TRUE)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(SFML_DIR "${CMAKE_SOURCE_DIR}/src/SFML/Windows/lib/cmake/SFML")
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(SFML_DIR "${CMAKE_SOURCE_DIR}/src/SFML/Linux/lib/cmake/SFML")
endif()
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)
add_library(SFML3 INTERFACE)
target_link_libraries(SFML3 INTERFACE SFML::Graphics SFML::Window SFML::System)

#Executable library
add_public_library(Executable src/${PROJECT_NAME}/Executable.cpp)
link_sfml(Executable)

#Animation library
add_public_library(Animation src/${PROJECT_NAME}/Animation.cpp)
link_sfml(Animation)

#Setup library
add_public_library(Game_Library src/${PROJECT_NAME}/Game_Library.cpp)
link_sfml(Game_Library)

#Widgets library
add_public_library(Widgets src/${PROJECT_NAME}/Widgets.cpp)
link_sfml(Widgets)

#User_Interface library
add_public_library(User_Interface src/${PROJECT_NAME}/User_Interface.cpp)
link_sfml(User_Interface)

install(TARGETS
    SFML3
    Animation
    Executable
    Game_Library
    Widgets
    User_Interface
    ARCHIVE DESTINATION lib/${PROJECT_NAME}
    LIBRARY DESTINATION lib/${PROJECT_NAME}
    RUNTIME DESTINATION bin/${PROJECT_NAME}
)

#adding our main executable
add_executable(External_Gaming_Library src/${PROJECT_NAME}/Main.cpp)
target_link_libraries(External_Gaming_Library User_Interface Widgets Game_Library Executable Animation SFML3)