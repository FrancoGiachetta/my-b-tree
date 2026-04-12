add_library(libbtree btree.cpp)
add_library(btree::libbtree ALIAS libbtree)

set_target_properties(libbtree
  PROPERTIES OUTPUT_NAME btree
)

target_compile_features(libbtree PUBLIC cxx_std_23)

target_include_directories(libbtree
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>

  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
)

include(GNUInstallDirs)
install(TARGETS libbtree
  EXPORT btree-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
  EXPORT btree-targets
  FILE btree-config.cmake
  NAMESPACE btree::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/btree
)
