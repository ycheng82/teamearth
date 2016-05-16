cmake_minimum_required(VERSION 2.8)

project(ImageSegmentation)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
endif()

add_executable(WatershedSegmentation2 WatershedSegmentation2.cxx )
target_link_libraries(WatershedSegmentation2 ${ITK_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
