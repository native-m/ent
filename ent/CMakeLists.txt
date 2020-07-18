# CMakeList.txt : CMake project for ent, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ent "ent.cpp" "ent.h" "World.h" "World.cpp" "Entity.h" "Entity.cpp"  "Component.h"  "Component.cpp" "ComponentManager.cpp" "ComponentManager.h" "ComponentPool.h" "Def.h")

# TODO: Add tests and install targets if needed.
