cmake_minimum_required(VERSION 2.8.9)

project(Slicer-TransformSmoother)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "https://github.com/lchauvin/Slicer-TransformSmoother")
set(EXTENSION_CATEGORY "IGT")
set(EXTENSION_CONTRIBUTORS "Laurent Chauvin (BWH), Jayender Jagadeesan (BWH), Andras Lasso (Queen's University)")
set(EXTENSION_DESCRIPTION "Smooth Linear Transforms")
set(EXTENSION_ICONURL "https://raw.github.com/lchauvin/Slicer-TransformSmoother/master/Slicer-TransformSmoother.png")
set(EXTENSION_SCREENSHOTURLS "http://www.example.com/Slicer/Extensions/Slicer-TransformSmoother/Screenshots/1.png")

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(TransformSmoother)
## NEXT_MODULE

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_CPACK})
