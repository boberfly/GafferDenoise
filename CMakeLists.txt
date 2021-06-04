cmake_minimum_required(VERSION 3.0)
project(GafferDenoise)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE OFF)

if(APPLE)
  set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif(APPLE)

list(APPEND CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/cmake"
)

find_package(TBB 2017.0 REQUIRED tbb)
find_package(OpenImageDenoise REQUIRED)

# build the library
#file( GLOB GafferDenoiseSrc src/GafferDenoise/*.cpp )
file( GLOB GafferDenoiseSrc src/GafferDenoise/DenoiseOIDN.cpp src/GafferDenoise/FrameProcessor.cpp src/GafferDenoise/ImageAlgo.cpp )
link_directories( GafferDenoise ${GAFFER_ROOT}/lib )
add_library( GafferDenoise SHARED ${GafferDenoiseSrc} )
target_compile_definitions( GafferDenoise PRIVATE BOOST_SIGNALS_NO_DEPRECATION_WARNING=1 -D_GLIBCXX_USE_CXX11_ABI=0 )
target_include_directories( GafferDenoise PRIVATE include ${GAFFER_ROOT}/include )
target_link_libraries( GafferDenoise Gaffer GafferScene IECore OpenImageDenoise )
install( TARGETS GafferDenoise DESTINATION lib)

# build the python bindings
file( GLOB GafferDenoiseModuleSrc src/GafferDenoiseModule/*.cpp )
add_library( GafferDenoiseModule SHARED ${GafferDenoiseModuleSrc} )
set_target_properties( GafferDenoiseModule PROPERTIES PREFIX "" OUTPUT_NAME "_GafferDenoise" )
target_compile_definitions( GafferDenoiseModule PRIVATE BOOST_SIGNALS_NO_DEPRECATION_WARNING=1 -D_GLIBCXX_USE_CXX11_ABI=0 )
target_link_libraries( GafferDenoiseModule GafferDenoise )

string(REPLACE "." "" PYTHON_VERSION_STRIPPED ${PYTHON_VERSION})
string(REPLACE "3.7" "3.7m" PYTHON_FOLDER ${PYTHON_VERSION})
if(APPLE)
  target_include_directories( GafferDenoiseModule PRIVATE include ${GAFFER_ROOT}/include ${GAFFER_ROOT}/lib/Python.framework/Versions/${PYTHON_VERSION}/include/python${PYTHON_VERSION} )
  target_link_libraries( GafferDenoiseModule GafferBindings IECorePython boost_python Python)
else()
  target_include_directories( GafferDenoiseModule PRIVATE include ${GAFFER_ROOT}/include ${GAFFER_ROOT}/include/python${PYTHON_FOLDER})
  target_link_libraries( GafferDenoiseModule GafferBindings IECorePython boost_python${PYTHON_VERSION_STRIPPED} python${PYTHON_FOLDER})
endif(APPLE)

install( TARGETS GafferDenoiseModule DESTINATION python/GafferDenoise )

# build the python modules
file( GLOB GafferDenoiseModuleFiles python/GafferDenoise/*.py )
install( FILES ${GafferDenoiseModuleFiles} DESTINATION python/GafferDenoise )
file( GLOB GafferDenoiseTestModuleFiles python/GafferDenoiseTest/*.py )
install( FILES ${GafferDenoiseTestModuleFiles} DESTINATION python/GafferDenoiseTest )
file( GLOB GafferDenoiseUIModuleFiles python/GafferDenoiseUI/*.py )
install( FILES ${GafferDenoiseUIModuleFiles} DESTINATION python/GafferDenoiseUI )
file( GLOB GafferDenoiseUITestModuleFiles python/GafferDenoiseUITest/*.py )
install( FILES ${GafferDenoiseUITestModuleFiles} DESTINATION python/GafferDenoiseUITest )

# build the startup configs
install( DIRECTORY startup DESTINATION . FILES_MATCHING PATTERN "*.py" )

# build the extra resources
install( DIRECTORY resources DESTINATION . FILES_MATCHING PATTERN "*.gfr" PERMISSIONS OWNER_READ GROUP_READ WORLD_READ )