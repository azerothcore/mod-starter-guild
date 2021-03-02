CU_SET_PATH("CMAKE_MOD_STARTGUILD_DIR" "${CMAKE_CURRENT_LIST_DIR}")

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_startguild.cpp")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod_startguild.conf.dist")

AC_ADD_SCRIPT_LOADER("StartGuild" "${CMAKE_CURRENT_LIST_DIR}/src/loader_startguild.h")
