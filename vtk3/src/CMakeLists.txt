
cmake_minimum_required(VERSION 2.8)

PROJECT(DrawText)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(DrawText MACOSX_BUNDLE DrawText)

if(VTK_LIBRARIES)
  target_link_libraries(DrawText ${VTK_LIBRARIES})
else()
  target_link_libraries(DrawText vtkHybrid vtkWidgets)
endif()
