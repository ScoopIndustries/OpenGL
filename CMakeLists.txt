project(moteur)

add_subdirectory(dep/SDL dep/SDL)

include(${CMAKE_CURRENT_SOURCE_DIR}/glew.cmake)

set(SDL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/SDL/include)
set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/glm)
add_executable(moteur "src/main.cpp" "Header/gc_3d_defs.hpp" "Header/camera.hpp" "src/camera.cpp" "src/Geometry.cpp" "Shader/loadShader.cpp" "Header/shader.hpp" "Header/Buffer.hpp" "src/Buffer.cpp"  )
target_include_directories(moteur PRIVATE 
	${SDL_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
)
target_link_libraries(moteur PRIVATE SDL2-static SDL2main opengl32 glew)

