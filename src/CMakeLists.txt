
include_directories(${SRCDIR}/training)
include_directories(${SRCDIR}/facedetector)
include_directories(${SRCDIR}/gaborbank)
include_directories(${SRCDIR}/utils)
include_directories(${SRCDIR}/detector)
include_directories(${SRCDIR}/gui)

add_subdirectory(training)
add_subdirectory(facedetector)
add_subdirectory(gaborbank)
add_subdirectory(dataset)
add_subdirectory(utils)
add_subdirectory(detector)
add_subdirectory(gui)

