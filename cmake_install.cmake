# Install script for directory: /afs/desy.de/user/s/schuea/mymarlin

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/examples/mymarlin")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSimProcessor.so.0.1.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSimProcessor.so.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSimProcessor.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/examples/mymarlin/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/lcio/v02-04-01/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/mysql/usr/lib64:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/gear/v01-03/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/CLHEP/2.1.3.1/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/ilcutil/v01-00/lib")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/afs/desy.de/user/s/schuea/mymarlin/lib/libSimProcessor.so.0.1.0"
    "/afs/desy.de/user/s/schuea/mymarlin/lib/libSimProcessor.so.0.1"
    "/afs/desy.de/user/s/schuea/mymarlin/lib/libSimProcessor.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSimProcessor.so.0.1.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSimProcessor.so.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSimProcessor.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/lcio/v02-04-01/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/mysql/usr/lib64:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/gear/v01-03/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/CLHEP/2.1.3.1/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/ilcutil/v01-00/lib:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
           NEW_RPATH "/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/examples/mymarlin/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/Marlin/v01-05/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/lcio/v02-04-01/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/mysql/usr/lib64:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/gear/v01-03/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/CLHEP/2.1.3.1/lib:/nfs/dust/atlas/user/schuea/scratch/sid_complete/v01-17-02-sid/ilcutil/v01-00/lib")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/opt/rh/devtoolset-1.1/root/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/afs/desy.de/user/s/schuea/mymarlin/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/afs/desy.de/user/s/schuea/mymarlin/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
