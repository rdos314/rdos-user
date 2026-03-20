cmake_minimum_required(VERSION 3.16)

set(CPP_SRC ehc200.cpp frinv.cpp hhcn818.cpp misol.cpp ocppdev.cpp powhvmp.cpp powinv.cpp smameter.cpp)
enable_language(CXX)

set(CPP_OBJ_FILES "")
foreach(CPP_FILE ${CPP_SRC})
  get_filename_component(NAME ${CPP_FILE} NAME_WE)
  set(OBJ_FILE "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.obj")
   list(APPEND CPP_OBJ_FILES ${OBJ_FILE})
endforeach()

set(LIB_FILE "${CMAKE_CURRENT_BINARY_DIR}/classlib.lib")
add_custom_command(
   OUTPUT ${LIB_FILE}
   COMMAND wlib ${LIB_FILE} ${CPP_OBJ_FILES}
   DEPENDS ${CPP_OBJ_FILES}
   COMMENT "Creating library ${LIB_FILE}"
   )
   
add_custom_target(classlib ALL DEPENDS ${LIB_FILE})

