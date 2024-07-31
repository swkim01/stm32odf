############### Add include ###################
#list(APPEND ADD_INCLUDE
#       	"../../../components/freertos/CMSIS_RTOS_V2"
#       	"../../../components/freertos/include"
#       	"../../../components/freertos/portable/GCC/ARM_CM4F"
#     )
# list(APPEND ADD_PRIVATE_INCLUDE "")
###############################################

############ Add source files #################
list(APPEND ADD_SRCS  "main.c"
    )
# aux_source_directory(src ADD_SRCS)
# list(REMOVE_ITEM COMPONENT_SRCS "src/test2.c")
###############################################

###### Add required/dependent components ######
list(APPEND ADD_REQUIREMENTS stm32hal freertos)
###############################################

############ Add static libs ##################
# list(APPEND ADD_STATIC_LIB "lib/libos.a")
###############################################


register_component()

