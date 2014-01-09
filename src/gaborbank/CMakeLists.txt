
add_executable(gaborbank_cli gaborbank_cli.cpp GaborBank.cpp ../utils/matrix_io.cpp )
target_link_libraries(gaborbank_cli ${OpenCV_LIBS})

INSTALL(TARGETS gaborbank_cli DESTINATION ${ASSETDIR})

