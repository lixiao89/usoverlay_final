#
#
#   Author(s):  Ali Uneri
#   Created on: 2009-08-13
#
#   (C) Copyright 2007-2011 Johns Hopkins University (JHU), All Rights Reserved.
#
# --- begin cisst license - do not edit ---
#
# This software is provided "as is" under an open source license, with
# no warranty.  The complete license can be found in license.txt and
# http://www.cisst.org/cisst/license.txt.
#
# --- end cisst license ---

# create a list of libraries needed for this project
set (REQUIRED_CISST_LIBRARIES cisstCommon cisstVector cisstOSAbstraction
  cisstMultiTask cisstParameterTypes cisstFLTK cisstCommonQt
  cisstCommonXML)

# find cisst and make sure the required libraries have been compiled
find_package (cisst REQUIRED ${REQUIRED_CISST_LIBRARIES})

if (cisst_FOUND_AS_REQUIRED)

  # load cisst configuration
  include (${CISST_USE_FILE})

  # sawOpenIGTLink has been compiled within cisst, we should find it automatically
  cisst_find_saw_component (sawOpenIGTLink-dev REQUIRED)

  if (sawOpenIGTLink-dev_FOUND)

    # sawOpenIGTLink configuration
    include_directories (${sawOpenIGTLink-dev_INCLUDE_DIR})
    link_directories (${sawOpenIGTLink-dev_LIBRARY_DIR})

    add_executable (sawOpenIGTLinkExample
                    main.cpp
                    NDISimulator.cpp
                    NDISimulator.h
                    )
		  
    set_property (TARGET sawOpenIGTLinkExample PROPERTY FOLDER "sawOpenIGTLink-dev")

    target_link_libraries (sawOpenIGTLinkExample ${sawOpenIGTLink-dev_LIBRARIES})

    cisst_target_link_libraries (sawOpenIGTLinkExample ${REQUIRED_CISST_LIBRARIES})

    # ----------- real tracker --------------
    cisst_find_saw_component( sawNDITracker REQUIRED)

    # initial find_package doesn't have cisstQt
    cisst_set_directories (cisstQt)

    # for Qt generated header files
    include_directories (${CMAKE_CURRENT_BINARY_DIR})
    
    if(sawNDITracker_FOUND)

      include_directories( ${sawNDITracker_INCLUDE_DIR} )
      link_directories (${sawNDITracker_LIBRARY_DIR})
      
      add_executable (NDITracker
        NDITracker.cpp
        )
		  
      set_property (TARGET NDITracker PROPERTY FOLDER "sawOpenIGTLink-dev")
      
      target_link_libraries (NDITracker ${sawOpenIGTLink-dev_LIBRARIES}
	${sawNDITracker_LIBRARIES}
	${sawNDITrackerQt_LIBRARIES}
	${QT_LIBRARIES})
      
      cisst_target_link_libraries (NDITracker
	${REQUIRED_CISST_LIBRARIES})

      add_custom_command (TARGET NDITracker
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} ARGS -E copy
	${CMAKE_CURRENT_SOURCE_DIR}/configNDITracker.xml
	${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/configNDITracker.xml

	COMMAND ${CMAKE_COMMAND} ARGS -E copy
	${CMAKE_CURRENT_SOURCE_DIR}/mainPython.py
	${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/mainPython.py
	COMMENT "sawNDITrackerQtExample: copying files to the executable directory"

	COMMAND ${CMAKE_COMMAND} ARGS -E copy
	${CMAKE_CURRENT_SOURCE_DIR}/8700339.rom
	${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/8700339.rom
	COMMENT "sawNDITrackerQtExample: copying files to the executable directory"

	COMMAND ${CMAKE_COMMAND} ARGS -E copy
	${CMAKE_CURRENT_SOURCE_DIR}/8700449.rom
	${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/8700449.rom
	COMMENT "sawNDITrackerQtExample: copying files to the executable directory")


      
    endif(sawNDITracker_FOUND)
    
  endif (sawOpenIGTLink-dev_FOUND)

else (cisst_FOUND_AS_REQUIRED)
  message ("Information: code in ${CMAKE_CURRENT_SOURCE_DIR} will not be compiled, it requires ${REQUIRED_CISST_LIBRARIES}")
endif (cisst_FOUND_AS_REQUIRED)
