add_custom_target(examples ALL)


if (BUILD_C_BINDINGS)
    add_executable(flann_example_c flann_example.c)
    target_link_libraries(flann_example_c flann)
    set_target_properties(flann_example_c PROPERTIES COMPILE_FLAGS -std=c99)

    add_dependencies(examples flann_example_c)
    install (TARGETS flann_example_c DESTINATION bin )
endif()

if (HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIR})

    add_executable(flann_example_cpp flann_example.cpp)
    target_link_libraries(flann_example_cpp ${HDF5_LIBRARIES} flann_cpp)
    if (HDF5_IS_PARALLEL)
        target_link_libraries(flann_example_cpp ${MPI_LIBRARIES})
    endif()

    add_dependencies(examples flann_example_cpp)
    install (TARGETS flann_example_cpp DESTINATION bin)



	add_executable(new_example_cpp new_example.cpp)
    target_link_libraries(new_example_cpp ${HDF5_LIBRARIES} flann_cpp)
    if (HDF5_IS_PARALLEL)
        target_link_libraries(new_example_cpp ${MPI_LIBRARIES})
    endif()

    add_dependencies(examples new_example_cpp)
    install (TARGETS new_example_cpp DESTINATION bin)



	add_executable(test_code test_code.cpp)
    target_link_libraries(test_code ${HDF5_LIBRARIES} flann_cpp)
    if (HDF5_IS_PARALLEL)
        target_link_libraries(test_code ${MPI_LIBRARIES})
    endif()

    add_dependencies(examples test_code)
    install (TARGETS test_code DESTINATION bin)

	add_executable(linear_test linear_test.cpp)
    target_link_libraries(linear_test ${HDF5_LIBRARIES} flann_cpp)
    if (HDF5_IS_PARALLEL)
        target_link_libraries(linear_test ${MPI_LIBRARIES})
    endif()

    add_dependencies(examples linear_test)
    install (TARGETS linear_test DESTINATION bin)

	add_executable(kmeans_test kmeans_example.cpp)
    target_link_libraries(kmeans_test ${HDF5_LIBRARIES} flann_cpp)
    if (HDF5_IS_PARALLEL)
        target_link_libraries(kmeans_test ${MPI_LIBRARIES})
    endif()

    add_dependencies(examples kmeans_test)
    install (TARGETS kmeans_test DESTINATION bin)

	add_executable(approx_graph_test approx_graph_test.cpp)
    target_link_libraries(approx_graph_test ${HDF5_LIBRARIES} flann_cpp)
    if (HDF5_IS_PARALLEL)
        target_link_libraries(approx_graph_test ${MPI_LIBRARIES})
    endif()

    add_dependencies(examples approx_graph_test)
    install (TARGETS approx_graph_test DESTINATION bin)


    if (USE_MPI AND HDF5_IS_PARALLEL)
        add_executable(flann_example_mpi flann_example_mpi.cpp)
        target_link_libraries(flann_example_mpi flann_cpp ${HDF5_LIBRARIES} ${MPI_LIBRARIES} ${Boost_LIBRARIES})

        add_dependencies(examples flann_example_mpi)
        install (TARGETS flann_example_mpi DESTINATION bin)
    endif()
else()
    message("hdf5 library not found, not compiling flann_example.cpp")
endif()




