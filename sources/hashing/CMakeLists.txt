project(hashing)



#message( STATUS "***** hash" ${SOURCES_ROOT_DIR} )
ADD_LIBRARY( ${PROJECT_NAME} OBJECT
	hashing.c
	)


set(PROJECT_INCLUDE_DIRS ${SOURCES_ROOT_DIR} )
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS}/${PROJECT_NAME}) #import headers

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS}/) #export headers

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		      sodium
		      misc
		      accounts
		      )

