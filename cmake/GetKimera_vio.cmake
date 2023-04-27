ExternalProject_Add(Kimera_VIO
        GIT_REPOSITORY https://github.com/ILLIXR/Kimera-VIO.git
        GIT_TAG af2e0dc94b8f3a9b93171084715464077fa66487
        GIT_SUBMODULES_RECURSE TRUE
        DEPENDS ${KimeraRPGO_DEP_STR} ${DBoW2_DEP_STR} ${GTSAM_DEP_STR} ${opengv_DEP_STR}
        PREFIX ${CMAKE_BINARY_DIR}/_deps/kimera_vio
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}  -DCMAKE_SHARED_LINKER_FLAGS=-L${CMAKE_INSTALL_PREFIX}/lib -DCMAKE_EXE_LINKER_FLAGS=-L${CMAKE_INSTALL_PREFIX}/lib -DBUILD_TESTING=OFF -DBUILD_TESTS=OFF
        )