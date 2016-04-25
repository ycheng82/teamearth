# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.8.9)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(ITKv4ImageRegistration)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})


add_executable(DeformableRegistration2 DeformableRegistration2.cxx )
target_link_libraries(DeformableRegistration2 ${ITK_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
