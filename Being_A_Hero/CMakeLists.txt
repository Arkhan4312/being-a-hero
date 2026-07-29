# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)
# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Being_A_Hero")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/glfw)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC external/glad)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

# Включите подпроекты.
set(SOURCES
	src/main.cpp
	src/Core/Game.cpp
	src/Entity/Unit.cpp
	src/Entity/Player.cpp
	src/Entity/Enemy.cpp
	src/Entity/Ability.cpp
	src/Entity/Inventory.cpp
	src/Entity/QuestLog.cpp
	src/Entity/Projectile.cpp
	src/Map/TileMap.cpp
	src/Map/MapGenerator.cpp
	src/Map/GlobalMap.cpp
	src/Graphics/Renderer.cpp
	src/Graphics/Shader.cpp
	src/Graphics/Texture2D.cpp
	src/Graphics/SpriteBatch.cpp
	src/Graphics/Camera2D.cpp
 )

add_executable(Being_A_Hero ${SOURCES})
target_link_libraries(Being_A_Hero 
PRIVATE
glfw
glad
glm)
target_include_directories(Being_A_Hero PRIVATE src)