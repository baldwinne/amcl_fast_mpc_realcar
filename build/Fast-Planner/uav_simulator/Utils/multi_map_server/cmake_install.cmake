# Install script for directory: /home/handsfree/Amcl_LMPC_Fast_Real/src/Fast-Planner/uav_simulator/Utils/multi_map_server

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/handsfree/Amcl_LMPC_Fast_Real/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/multi_map_server/msg" TYPE FILE FILES
    "/home/handsfree/Amcl_LMPC_Fast_Real/src/Fast-Planner/uav_simulator/Utils/multi_map_server/msg/MultiOccupancyGrid.msg"
    "/home/handsfree/Amcl_LMPC_Fast_Real/src/Fast-Planner/uav_simulator/Utils/multi_map_server/msg/MultiSparseMap3D.msg"
    "/home/handsfree/Amcl_LMPC_Fast_Real/src/Fast-Planner/uav_simulator/Utils/multi_map_server/msg/SparseMap3D.msg"
    "/home/handsfree/Amcl_LMPC_Fast_Real/src/Fast-Planner/uav_simulator/Utils/multi_map_server/msg/VerticalOccupancyGridList.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/multi_map_server/cmake" TYPE FILE FILES "/home/handsfree/Amcl_LMPC_Fast_Real/build/Fast-Planner/uav_simulator/Utils/multi_map_server/catkin_generated/installspace/multi_map_server-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/handsfree/Amcl_LMPC_Fast_Real/devel/include/multi_map_server")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/handsfree/Amcl_LMPC_Fast_Real/devel/share/roseus/ros/multi_map_server")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/handsfree/Amcl_LMPC_Fast_Real/devel/share/common-lisp/ros/multi_map_server")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/handsfree/Amcl_LMPC_Fast_Real/devel/share/gennodejs/ros/multi_map_server")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python2" -m compileall "/home/handsfree/Amcl_LMPC_Fast_Real/devel/lib/python2.7/dist-packages/multi_map_server")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/handsfree/Amcl_LMPC_Fast_Real/devel/lib/python2.7/dist-packages/multi_map_server")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/handsfree/Amcl_LMPC_Fast_Real/build/Fast-Planner/uav_simulator/Utils/multi_map_server/catkin_generated/installspace/multi_map_server.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/multi_map_server/cmake" TYPE FILE FILES "/home/handsfree/Amcl_LMPC_Fast_Real/build/Fast-Planner/uav_simulator/Utils/multi_map_server/catkin_generated/installspace/multi_map_server-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/multi_map_server/cmake" TYPE FILE FILES
    "/home/handsfree/Amcl_LMPC_Fast_Real/build/Fast-Planner/uav_simulator/Utils/multi_map_server/catkin_generated/installspace/multi_map_serverConfig.cmake"
    "/home/handsfree/Amcl_LMPC_Fast_Real/build/Fast-Planner/uav_simulator/Utils/multi_map_server/catkin_generated/installspace/multi_map_serverConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/multi_map_server" TYPE FILE FILES "/home/handsfree/Amcl_LMPC_Fast_Real/src/Fast-Planner/uav_simulator/Utils/multi_map_server/package.xml")
endif()

