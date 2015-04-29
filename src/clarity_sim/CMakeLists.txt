PROJECT(ClarityPolarisClient)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
FIND_PACKAGE (cisst REQUIRED)
FIND_PACKAGE(FLTK REQUIRED)

if (cisst_FOUND)

  # load cisst configuration
	include (${CISST_USE_FILE})
	INCLUDE_DIRECTORIES(${ClarityPolarisClient_SOURCE_DIR}
                     ${FLTK_INCLUDE_DIR}) 
	FLTK_WRAP_UI(ClarityPolarisClient ClarityUI.fl)  
  
  # sawNDITracker has been compiled within cisst, we should find it automatically
  cisst_find_saw_component (sawNDITracker REQUIRED)

  if (sawNDITracker_FOUND)

    # sawNDITracker configuration
    include_directories (${sawNDITracker_INCLUDE_DIR})
    link_directories (${sawNDITracker_LIBRARY_DIR})

    add_executable (ClarityPolarisClient 
					guiTask.cpp
					guiTask.h
					polarisToolComponent.h
					polarisToolComponent.cpp
					clarityClientTask.cpp
					clarityClientTask.h
					main.cpp
					${ClarityPolarisClient_FLTK_UI_SRCS})
					
    set_property (TARGET ClarityPolarisClient PROPERTY FOLDER "sawNDITracker")

    # link against non cisst libraries and saw components
    target_link_libraries (ClarityPolarisClient
                           ${sawNDITracker_LIBRARIES}
                           ${FLTK_LIBRARIES}
                           ${CMAKE_THREAD_LIBS_INIT})

	set (REQUIRED_CISST_LIBRARIES cisstCommon cisstCommonXML
                              cisstVector cisstOSAbstraction cisstNumerical
                              cisstMultiTask cisstParameterTypes)
							  
   # Copy the Python example to the binary tree
    add_custom_command (TARGET ClarityPolarisClient
                        POST_BUILD
                        COMMAND ${CMAKE_COMMAND}
                        ARGS -E copy
                             ${CMAKE_CURRENT_SOURCE_DIR}/config.xml
                             ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/config.xml
                        COMMENT "clarity_sim: copy config.xml to the executable directory"
			
			COMMAND ${CMAKE_COMMAND}
                        ARGS -E copy
                             ${CMAKE_CURRENT_SOURCE_DIR}/8700339.rom
                             ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/8700339.rom
                        COMMENT "clarity_sim: copy rom to the executable directory"
			
			COMMAND ${CMAKE_COMMAND}
                        ARGS -E copy
                             ${CMAKE_CURRENT_SOURCE_DIR}/8700449.rom
                             ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/8700449.rom
                        COMMENT "clarity_sim: copy rom to the executable directory")
			
			
    cisst_target_link_libraries (ClarityPolarisClient ${REQUIRED_CISST_LIBRARIES})

  endif (sawNDITracker_FOUND)

endif (cisst_FOUND)

