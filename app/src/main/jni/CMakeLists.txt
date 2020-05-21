# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


set(pathPROJECT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
message("CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
message("pathPROJECT:${pathPROJECT}")

set(pathJNI ${pathPROJECT}/src/main/jni)
set(pathJNILIBS ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

set(pathLIBOPENCV ${pathJNILIBS}/libopencv_java3.so)
message("pathLIBOPENCV: ${pathLIBOPENCV}")

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

include_directories(${pathJNI}/include)

find_package(OpenMP QUIET)

if(OpenMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    add_definitions( -D_OPENMP=1 )
else()
    message(WARNING "OpenMP not found.")
endif()

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

SET(SOURCES
        ${pathJNI}/FaceTrackerJNI.cpp
        ${pathJNI}/FaceTracker.cpp
        ${pathJNI}/AssetIstream.cpp
        ${pathJNI}/PDM.cc
        ${pathJNI}/CLM.cc
        ${pathJNI}/Patch.cc
        ${pathJNI}/PAW.cc
        ${pathJNI}/FDet.cc
        ${pathJNI}/Tracker.cc
        ${pathJNI}/FCheck.cc
        ${pathJNI}/IO.cc)

add_library(FaceTrackerLib SHARED ${SOURCES})
set_target_properties(FaceTrackerLib PROPERTIES LINKER_LANGUAGE CXX)

add_library(lib_opencv SHARED IMPORTED)
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${pathLIBOPENCV})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        FaceTrackerLib
        lib_opencv
        -landroid
        -ljnigraphics

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} )