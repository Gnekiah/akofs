set_property(GLOBAL	PROPERTY USE_FOLDERS ON)

set(LIB_TYPE STATIC)
if(BUILD_SHARED_LIBS)
	set(LIB_TYPE SHARED)
	add_definitions(-DLIBCONFIG_EXPORTS)
	add_definitions(-DLIBCONFIGXX_EXPORTS)
else()	
	add_definitions(-DLIBCONFIG_STATIC)
	add_definitions(-DLIBCONFIGXX_STATIC)
endif()

include(GNUInstallDirs)
add_subdirectory(libconfig)
