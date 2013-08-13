
# Cant install because py fire are not targets
#INSTALL(TARGETS initdataset.py DESTINATION ${ASSETDIR} )
#INSTALL(TARGETS trainfiles.py DESTINATION ${ASSETDIR} )
#INSTALL(TARGETS default.conf DESTINATION ${ASSETDIR} )

CONFIGURE_FILE(initdataset.py ${ASSETDIR}/initdataset.py COPYONLY)
CONFIGURE_FILE(trainfiles.py ${ASSETDIR}/trainfiles.py COPYONLY)
CONFIGURE_FILE(default.conf ${ASSETDIR}/default.conf COPYONLY)
CONFIGURE_FILE(filldatasetCohnKanade.py ${ASSETDIR}/filldatasetCohnKanade.py COPYONLY)
CONFIGURE_FILE(cropFaces.py ${ASSETDIR}/cropFaces.py COPYONLY)
CONFIGURE_FILE(calcFeatures.py ${ASSETDIR}/calcFeatures.py COPYONLY)


