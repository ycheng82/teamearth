cmake_minimum_required(VERSION 2.8)
 
project(DeformableRegistration16)
 
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
endif()
 
add_executable(DeformableRegistration16 MACOSX_BUNDLE DeformableRegistration16.cxx)
 
if( "${ITK_VERSION_MAJOR}" LESS 4 )
  target_link_libraries(DeformableRegistration16 ITKReview ${ITK_LIBRARIES})
else( "${ITK_VERSION_MAJOR}" LESS 4 )
  target_link_libraries(DeformableRegistration16 ${ITK_LIBRARIES})
endif( "${ITK_VERSION_MAJOR}" LESS 4 )