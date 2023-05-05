# ---Choose CMake version---
cmake_minimum_required(VERSION 3.20)

# ---Choose project name---
project(OpenGL-TPs-Template)

# ---Add p6 library---
include(FetchContent)
FetchContent_Declare(
    p6
    GIT_REPOSITORY https://github.com/julesfouchy/p6
    GIT_TAG b9e6366e1198abdd7bc835e6125f9ff5b89a75b8
)
FetchContent_MakeAvailable(p6)

# ---Create a target for each exercise---
function(add_exercise EXERCISE_NAME)
    set(TARGET_NAME ${EXERCISE_NAME}_exe)
    add_executable(${TARGET_NAME})

    # ---Choose C++ version---
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

    # ---Choose warning level---
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W4)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
    endif()

    # ---Declare source files---
    file(GLOB_RECURSE EXERCISE_FILES CONFIGURE_DEPENDS ${EXERCISE_NAME}/*)
    file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS src-common/*)
    target_sources(${TARGET_NAME} PRIVATE ${EXERCISE_FILES} ${COMMON_FILES})
    target_include_directories(${TARGET_NAME} PRIVATE ${EXERCISE_FILES} src-common)

    # ---Link p6 library---
    target_link_libraries(${TARGET_NAME} PRIVATE p6::p6)

    # ---Set the output folder---
    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${EXERCISE_NAME})

    # ---Copy the assets and the shaders to the output folder (where the executable is created)---
    Cool__target_copy_folder(${TARGET_NAME} assets)
    Cool__target_copy_folder(${TARGET_NAME} ${EXERCISE_NAME}/shaders shaders)
endfunction(add_exercise)

add_exercise(TP3_exo1_triangle_blanc)
add_exercise(TP3_exo2_triangle_couleurs)
add_exercise(TP3_exo3_structure_vertex)
add_exercise(TP3_exo4_quad)
add_exercise(TP3_exo5_disque)
add_exercise(TP3_exo6_IBO_quad)
add_exercise(TP4_exo1_triangle_couleurs)
add_exercise(TP4_exo2_textures_procedurales)
add_exercise(TP4_exo3_Mandelbrot)
add_exercise(TP4_exo4_triangle)
add_exercise(TP6_exo1_sphere)
add_exercise(TP6_exo2_earth_moons)
add_exercise(TP6_exo3_multi-texturing)
add_exercise(TP6_exo4_shaders)
add_exercise(TP7_exo1_trackball_camera)
add_exercise(TP7_exo2_freefly_camera)
add_exercise(TP8_exo1_lumiere_directionnelle)
add_exercise(TP8_exo2_lumiere_ponctuelle)