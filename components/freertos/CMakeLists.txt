
if(CONFIG_LIB_FREERTOS_ENABLE)
    ################# Add include #################
    list(APPEND ADD_INCLUDE
                            "./include"
        )
    list(APPEND ADD_PRIVATE_INCLUDE
                            "../boards/include"
                            "../stm32hal/CMSIS/Include"
                            "./include/freertos"
        )
    ###############################################

    ############## Add source files ###############
    if(CONFIG_ARCH_STM32F4)
        set(STM32_DEF "STM32F4")
	set(STM32_DIR_PATH "STM32F4xx")
        aux_source_directory("portable/GCC/ARM_CM4F" ADD_SRCS_PORTABLE)
    elseif(CONFIG_ARCH_STM32F1)
        set(STM32_DEF "STM32F1")
	set(STM32_DIR_PATH "STM32F1xx")
        aux_source_directory("portable/GCC/ARM_CM3" ADD_SRCS_PORTABLE)
    else()
        set(STM32_DEF "STM32F4")
	set(STM32_DIR_PATH "STM32F4xx")
        aux_source_directory("portable/GCC/ARM_CM4F" ADD_SRCS_PORTABLE)
    endif()
    aux_source_directory("portable/MemMang" ADD_SRCS_MEMMANG)
    list(APPEND ADD_SRCS  ${ADD_SRCS_PORTABLE} ${ADD_SRCS_MEMMANG}
                         "croutine.c"
                         "event_groups.c"
                         "list.c"
                         "queue.c"
                         "stream_buffer.c"
                         "tasks.c"
                         "timers.c"
                            )
    if(CONFIG_FREERTOS_CMSIS_RTOS_V1)
        list(APPEND ADD_INCLUDE "./CMSIS_RTOS")
        list(APPEND ADD_SRCS "CMSIS_RTOS/cmsis_os.c")
    endif()
    if(CONFIG_FREERTOS_CMSIS_RTOS_V2)
        list(APPEND ADD_INCLUDE "./CMSIS_RTOS_V2")
        list(APPEND ADD_SRCS "CMSIS_RTOS_V2/cmsis_os2.c")
    endif()
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

