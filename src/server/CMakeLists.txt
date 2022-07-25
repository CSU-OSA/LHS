aux_source_directory(./ SRC)
add_library(server ${SRC})
target_link_libraries(server container model thread_pool)