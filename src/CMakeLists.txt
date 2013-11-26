
include_directories(${SRCDIR}/facecrop)
include_directories(${SRCDIR}/gaborbank)
include_directories(${SRCDIR}/adaboost)
include_directories(${SRCDIR}/utils)
include_directories(${SRCDIR}/detector)

add_subdirectory(facecrop)
add_subdirectory(gaborbank)
add_subdirectory(adaboost)
add_subdirectory(dataset)
add_subdirectory(utils)
add_subdirectory(detector)

