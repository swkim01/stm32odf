list(APPEND ADD_INCLUDE "include")
aux_source_directory("src" BOARDS_SRCS)
list(APPEND ADD_SRCS ${BOARDS_SRCS})

register_component()
