file(GLOB EXAMPLE_SOURCES "./*.c")

foreach(EXAMPLE_FILE ${EXAMPLE_SOURCES})
	
	get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} NAME_WE)

	add_executable(${EXAMPLE_NAME} ${EXAMPLE_FILE})

	set_target_properties(${EXAMPLE_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
	)

	target_link_libraries(${EXAMPLE_NAME} requester)

	target_include_directories(${EXAMPLE_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src")

endforeach()
