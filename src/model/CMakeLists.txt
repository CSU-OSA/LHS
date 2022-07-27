aux_source_directory(./ SRC)
add_library(model ${SRC})
target_link_libraries(model 
    container
)