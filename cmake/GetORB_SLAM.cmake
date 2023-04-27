EXTERNALPROJECT_ADD(ORB_Slam
        GIT_REPOSITORY https://github.com/ILLIXR/ORB_SLAM3.git
        GIT_TAG e9a0cac07ca778c5863c1737c92548575cc0fc26
        PREFIX ${CMAKE_BINARY_DIR}/_deps/ORB_Slam
        DEPENDS ${DBoW2_DEP_STR} ${Pangolin_DEP_STR} ${g2o_DEP_STR} ${Sophus_DEP_STR}
        BUILD_COMMAND cmake --build . -j1
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_FLAGS="-L${CMAKE_INSTALL_PREFIX}/lib" -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DILLIXR_ROOT=${PROJECT_SOURCE_DIR}/runtime
        )