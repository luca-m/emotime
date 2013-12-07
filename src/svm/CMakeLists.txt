
add_executable( svmtrain_cli svmtrain_cli.cpp svmtrain.cpp ../utils/matrix_io.cpp ../utils/csv.cpp)
target_link_libraries( svmtrain_cli ${OpenCV_LIBS} )
INSTALL(TARGETS svmtrain_cli DESTINATION ${ASSETDIR} )

#add_executable( svmpredict_cli svmpredict_cli.cpp svmpredict.cpp ../gaborbank/gaborbank.cpp ../facecrop/facecrop.cpp ../facecrop/registration.cpp ../facecrop/facedetector.cpp ../utils/matrix_io.cpp )
#target_link_libraries( svmpredict_cli ${OpenCV_LIBS} )
#INSTALL(TARGETS svmpredict_cli DESTINATION ${ASSETDIR} )

#add_executable( featselect_cli featselect_cli.cpp featselect.cpp ../utils/matrix_io.cpp )
#target_link_libraries( featselect_cli ${OpenCV_LIBS} )
#INSTALL(TARGETS featselect_cli DESTINATION ${ASSETDIR} )
