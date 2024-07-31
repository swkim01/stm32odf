
if(CONFIG_LIB_DRIVERS_ENABLE)
    ################# Add include #################
    if(CONFIG_ARCH_STM32F4)
        set(STM32_DEF "STM32F4")
	set(STM32_DIR_PATH "STM32F4xx")
	set(STM32_ASM_FLAGS "-mcpu=cortex-m4 -x assembler-with-cpp -mthumb")
    elseif(CONFIG_ARCH_STM32F1)
        set(STM32_DEF "STM32F1")
	set(STM32_DIR_PATH "STM32F1xx")
	set(STM32_ASM_FLAGS "-mcpu=cortex-m3 -x assembler-with-cpp -mthumb")
    else()
        set(STM32_DEF "STM32F4")
	set(STM32_DIR_PATH "STM32F4xx")
	set(STM32_ASM_FLAGS "-mcpu=cortex-m4 -x assembler-with-cpp -mthumb")
    endif()
    list(APPEND ADD_INCLUDE "."
                            "../stm32hal/include"
                            "../stm32hal/CMSIS/Include"
			    "../stm32hal/CMSIS/Device/ST/${STM32_DIR_PATH}/Include"
			    "../stm32hal/${STM32_DIR_PATH}_HAL_Driver/Inc"
			    "../stm32hal/${STM32_DIR_PATH}_HAL_Driver/Inc/Legacy"
        )
    # list(APPEND ADD_PRIVATE_INCLUDE "include_private")
    ###############################################

    ############## Add source files ###############
    aux_source_directory("." ADD_SRCS_DRIVERS)
    list(APPEND ADD_SRCS  ${ADD_SRCS_DRIVERS}
                            )
    # list(REMOVE_ITEM COMPONENT_SRCS "src/test.c")
    ###############################################

    ###### Add required/dependent components ######
    list(APPEND ADD_REQUIREMENTS gcc m c)
    ###############################################

    ############ Add static libs ##################
    #list(APPEND ADD_STATIC_LIB "libs/lib_mic.a"
    #                           "libs/lib_sipeed_kpu.a"
    #                           )
    ###############################################

    list(APPEND ADD_DEFINITIONS -DCONFIG_LOG_LEVEL=${CONFIG_SDK_LOG_LEVEL}
                                -DCONFIG_LOG_ENABLE=1
                                -DCONFIG_LOG_COLORS=1
                                -DLOG_KERNEL=1
				-D${STM32_DEF}
				-D${STM32_DIR_PATH}
                                )

    register_component()
endif()

