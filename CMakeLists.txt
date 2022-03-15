project(moteur)

add_subdirectory(dep/SDL dep/SDL)

include(${CMAKE_CURRENT_SOURCE_DIR}/glew.cmake)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui)

set(IMGUI_INCLUDE_DIR ${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(IMGUI_SRCS

${IMGUI_DIR}/imgui.cpp
${IMGUI_DIR}/imgui_draw.cpp
${IMGUI_DIR}/imgui_tables.cpp
${IMGUI_DIR}/imgui_widgets.cpp
${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
)

# swap implementation between OGL3 and OGL2
if(${IMGUI_USE_OGL3})
  # supposes that GLEW is available
  add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)
  set(IMGUI_SRCS ${IMGUI_SRCS} ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
else()
  set(IMGUI_SRCS ${IMGUI_SRCS} ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp)
endif()

add_library(imgui STATIC ${IMGUI_SRCS} "src/DebugConsole.cpp" "Header/DebugConsole.h")


if(${IMGUI_USE_OGL3})
target_link_libraries(imgui glew)
endif()

set(SDL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/SDL/include)
set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/glm)
add_executable(moteur "src/main.cpp" "Header/gc_3d_defs.hpp" "Header/camera.hpp" "src/camera.cpp" "src/Geometry.cpp" "Shader/loadShader.cpp" "Header/shader.hpp" "Header/Buffer.hpp" "src/Buffer.cpp"   "src/Shape.cpp" "Header/Shape.hpp" "src/DebugConsole.cpp" "Header/DebugConsole.h")
target_include_directories(moteur PRIVATE 
	${SDL_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
  	${IMGUI_DIR}
  	
)
target_link_libraries(moteur PRIVATE SDL2-static SDL2main opengl32 glew)

