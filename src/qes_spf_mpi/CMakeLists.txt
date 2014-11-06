


add_executable( viewfactor viewfactor.cpp )
target_link_libraries( viewfactor QESViewfactor )


add_executable( radiation radiation.cpp )
target_link_libraries( radiation QESRadiant QESGUI )


add_executable( simpleLSM simpleLSM.cpp )
target_link_libraries( simpleLSM QESLSM QESViewfactor )


add_executable( UMDTrees UMDTrees.cpp )
target_link_libraries( UMDTrees QESLSM QESGUI QESViewfactor )


add_executable( forceRestoreLSM forceRestoreLSM.cpp )
target_link_libraries( forceRestoreLSM QESLSM QESGUI QESViewfactor )

add_executable( testExport testExport.cpp)
target_link_libraries( testExport QESLSM QESGUI QESViewfactor )

add_executable( nRender normalRender.cpp )
target_link_libraries( nRender QESViewfactor QESNormalRender QESGUI)

add_executable( quicEnvSim quicEnvSim.cpp )
target_link_libraries( quicEnvSim QESViewfactor QESLSM QESGUI )
