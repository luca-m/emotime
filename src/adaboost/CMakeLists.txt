
add_executable( adatrain_cli adatrain_cli.cpp adatrain.cpp )
target_link_libraries( adatrain_cli ${OpenCV_LIBS} )
INSTALL(TARGETS adatrain_cli DESTINATION ${ASSETDIR} )


add_executable( adapredict_cli adapredict_cli.cpp adapredict.cpp  ../gaborbank/gaborbank.cpp ../facecrop/facecrop.cpp ../facecrop/registration.cpp ../facecrop/facedetector.cpp )
target_link_libraries( adapredict_cli ${OpenCV_LIBS} )
INSTALL(TARGETS adapredict_cli DESTINATION ${ASSETDIR} )

