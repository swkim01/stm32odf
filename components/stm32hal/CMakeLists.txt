
if(CONFIG_LIB_STM32HAL_ENABLE)
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

    list(APPEND ADD_INCLUDE "./include"
                            "../freertos/include/freertos"
                            "CMSIS/Include"
			    "CMSIS/Device/ST/${STM32_DIR_PATH}/Include"
			    "${STM32_DIR_PATH}_HAL_Driver/Inc"
			    "${STM32_DIR_PATH}_HAL_Driver/Inc/Legacy"
        )
    # list(APPEND ADD_PRIVATE_INCLUDE "include_private")
    ###############################################

    ############## Add source files ###############
    aux_source_directory("${STM32_DIR_PATH}_HAL_Driver/Src" ADD_SRCS_STM32HAL)
    aux_source_directory("bsp/${STM32_DIR_PATH}" ADD_SRCS_BSP)

    #include(asm_source.cmake)
    #set(ADD_ASM_SRCS "bsp/startup_stm32f446retx.s"
    stm32_get_chip_type(${FAMILY} ${DEVICE} TYPE)
    string(TOLOWER ${DEVICE} DEVICE_L)
    string(TOLOWER ${TYPE} TYPE_L)
    find_file(STARTUP
            NAMES startup_stm32${TYPE_L}.s
	    PATHS "CMSIS/Device/ST/${STM32_DIR_PATH}/Source/Templates/gcc"
	    NO_CACHE
            NO_DEFAULT_PATH
        )
    set(ADD_ASM_SRCS "${STARTUP}")
    if(NOT ADD_ASM_SRCS)
        set(STM_DEVICES_FOUND FALSE)
        message(VERBOSE "stm32hal: did not find file: startup_stm32${TYPE_L}.s")
        break()
    endif()

    list(APPEND ADD_SRCS  ${ADD_SRCS_STM32HAL}
                          ${ADD_SRCS_BSP}
                          ${ADD_ASM_SRCS}
                            )
    # list(REMOVE_ITEM COMPONENT_SRCS "src/test.c")
    #SET_PROPERTY(SOURCE ${ADD_ASM_SRCS} PROPERTY LANGUAGE C)
    #SET_SOURCE_FILES_PROPERTIES(${ADD_ASM_SRCS} PROPERTIES COMPILE_FLAGS "-mcpu=cortex-m4 -x assembler-with-cpp --specs=nano.specs -mthumb")
    SET_PROPERTY(SOURCE ${ADD_ASM_SRCS} PROPERTY LANGUAGE ASM)
    SET_SOURCE_FILES_PROPERTIES(${ADD_ASM_SRCS} PROPERTIES COMPILE_FLAGS "${STM32_ASM_FLAGS}")
    ###############################################

    ###### Add required/dependent components ######
    list(APPEND ADD_REQUIREMENTS gcc m c)
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

