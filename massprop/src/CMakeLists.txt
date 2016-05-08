cmake_minimum_required(VERSION 2.8)  
   
PROJECT(polyDataVolume)  
   
find_package(VTK REQUIRED)  
include(${VTK_USE_FILE})  
   
add_executable(polyDataVolume MACOSX_BUNDLE polyDataVolume)  
   
if(VTK_LIBRARIES)  
  target_link_libraries(polyDataVolume ${VTK_LIBRARIES})  
else()  
  target_link_libraries(polyDataVolume vtkHybrid vtkWidgets)  
endif()  