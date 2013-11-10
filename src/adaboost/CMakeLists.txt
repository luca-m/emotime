
add_executable( adatrain_cli adatrain_cli.cpp adatrain.cpp ../utils/matrix_io.cpp ../utils/csv.cpp)
target_link_libraries( adatrain_cli ${OpenCV_LIBS} )
INSTALL(TARGETS adatrain_cli DESTINATION ${ASSETDIR} )

add_executable( adapredict_cli adapredict_cli.cpp adapredict.cpp ../gaborbank/gaborbank.cpp ../facecrop/facecrop.cpp ../facecrop/registration.cpp ../facecrop/facedetector.cpp ../utils/matrix_io.cpp )
target_link_libraries( adapredict_cli ${OpenCV_LIBS} )
INSTALL(TARGETS adapredict_cli DESTINATION ${ASSETDIR} )

add_executable( featselect_cli featselect_cli.cpp featselect.cpp ../utils/matrix_io.cpp )
target_link_libraries( featselect_cli ${OpenCV_LIBS} )
INSTALL(TARGETS featselect_cli DESTINATION ${ASSETDIR} )


## TESTING ONLY
#add_executable( varimportance_cli varimportance_cli.cpp )
#target_link_libraries( varimportance_cli ${OpenCV_LIBS} )
#INSTALL(TARGETS varimportance_cli DESTINATION ${ASSETDIR} )
