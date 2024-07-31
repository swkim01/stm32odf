
#include(${PROJECT_SOURCE_DIR}/compile/compile_flags.cmake)

########## set C flags #########
#set(COMMON_C_LINK_FLAGS   -mfloat-abi=softfp
if (CONFIG_ARCH_STM32F4)

    set(COMMON_C_LINK_FLAGS   -mfloat-abi=hard
                    -mfpu=fpv4-sp-d16
                    -mcpu=cortex-m4
                    -mthumb
                    --specs=nano.specs
		    )
    set(FAMILY "F4")
    if (CONFIG_ARCH_STM32F446RE)
        set(DEVICE "F446RE")
    elseif (CONFIG_ARCH_STM32F401RE)
        set(DEVICE "F401RE")
    elseif (CONFIG_ARCH_STM32F429ZI)
        set(DEVICE "F429ZI")
    elseif (CONFIG_ARCH_STM32F411CE)
        set(DEVICE "F411CE")
    else()
        set(DEVICE "F446RE")
    endif()

elseif (CONFIG_ARCH_STM32F1)

    set(COMMON_C_LINK_FLAGS
                    -mcpu=cortex-m3
                    -mthumb
                    --specs=nano.specs
		    )

    set(FAMILY "F1")
    if (CONFIG_ARCH_STM32F103RB)
        set(DEVICE "F103xB")
    elseif (CONFIG_ARCH_STM32F103C6)
        set(DEVICE "F103x6")
    elseif (CONFIG_ARCH_STM32F103C8)
        set(DEVICE "F103C8")
    else()
        set(DEVICE "F103xB")
    endif()

endif()

set(CORE "")

set(OUTPUT_LD_FILE "${CMAKE_CURRENT_BINARY_DIR}/${DEVICE}.ld")

stm32_get_memory_info(FAMILY ${FAMILY} DEVICE ${DEVICE} CORE ${CORE} FLASH SIZE FLASH_SIZE ORIGIN FLASH_ORIGIN)
stm32_get_memory_info(FAMILY ${FAMILY} DEVICE ${DEVICE} CORE ${CORE} RAM SIZE RAM_SIZE ORIGIN RAM_ORIGIN)
stm32_get_memory_info(FAMILY ${FAMILY} DEVICE ${DEVICE} CORE ${CORE} CCRAM SIZE CCRAM_SIZE ORIGIN CCRAM_ORIGIN)
stm32_get_memory_info(FAMILY ${FAMILY} DEVICE ${DEVICE} CORE ${CORE} RAM_SHARE SIZE RAM_SHARE_SIZE ORIGIN RAM_SHARE_ORIGIN)
stm32_get_memory_info(FAMILY ${FAMILY} DEVICE ${DEVICE} CORE ${CORE} HEAP SIZE HEAP_SIZE)
stm32_get_memory_info(FAMILY ${FAMILY} DEVICE ${DEVICE} CORE ${CORE} STACK SIZE STACK_SIZE)

add_custom_command(OUTPUT "${OUTPUT_LD_FILE}"
    COMMAND ${CMAKE_COMMAND}
        -DFLASH_ORIGIN="${FLASH_ORIGIN}"
        -DRAM_ORIGIN="${RAM_ORIGIN}"
        -DCCRAM_ORIGIN="${CCRAM_ORIGIN}"
        -DRAM_SHARE_ORIGIN="${RAM_SHARE_ORIGIN}"
        -DFLASH_SIZE="${FLASH_SIZE}"
        -DRAM_SIZE="${RAM_SIZE}"
        -DCCRAM_SIZE="${CCRAM_SIZE}"
        -DRAM_SHARE_SIZE="${RAM_SHARE_SIZE}"
        -DSTACK_SIZE="${STACK_SIZE}"
        -DHEAP_SIZE="${HEAP_SIZE}"
        -DLINKER_SCRIPT="${OUTPUT_LD_FILE}"
	-P "${SDK_PATH}/tools/cmake/stm32/linker_ld.cmake"
)


set(CMAKE_C_FLAGS   ${COMMON_C_LINK_FLAGS}
                    -fno-common
                    -ffunction-sections
                    -fdata-sections
                    -fstrict-volatile-bitfields
                    -fno-zero-initialized-in-bss
                    -ffast-math
                    -fno-math-errno
                    -fsingle-precision-constant
                    -ffloat-store
                    -std=gnu11
                    -Os
                    -Wall
                    -Werror=all
                    -Wno-error=unused-function
                    -Wno-error=unused-but-set-variable
                    -Wno-error=unused-variable
                    -Wno-error=deprecated-declarations
                    -Wno-error=maybe-uninitialized
                    -Wextra
                    -Werror=frame-larger-than=32768
                    -Wno-unused-parameter
                    -Wno-unused-function
                    -Wno-implicit-fallthrough
                    -Wno-sign-compare
                    -Wno-error=missing-braces
                    -Wno-old-style-declaration
                    -Wno-error=pointer-sign
                    -Wno-pointer-to-int-cast
                    -Wno-strict-aliasing
                    -Wno-int-to-pointer-cast
                    )
################################


###### set CXX(cpp) flags ######
set(CMAKE_CXX_FLAGS ${COMMON_C_LINK_FLAGS}
                    -fno-common
                    -ffunction-sections
                    -fdata-sections
                    -fstrict-volatile-bitfields
                    -fno-zero-initialized-in-bss
                    -Os
                    -std=gnu++17
                    -Wall
                    -Wno-error=unused-function
                    -Wno-error=unused-but-set-variable
                    -Wno-error=unused-variable
                    -Wno-error=deprecated-declarations
                    -Wno-error=maybe-uninitialized
                    -Wextra
                    -Werror=frame-larger-than=32768
                    -Wno-unused-parameter
                    -Wno-unused-function
                    -Wno-implicit-fallthrough
                    -Wno-sign-compare
                    -Wno-error=missing-braces
                    -Wno-error=pointer-sign
                    -Wno-pointer-to-int-cast
                    -Wno-strict-aliasing
                    -Wno-int-to-pointer-cast
                    )
################################

######### set link flags ########
set(LINK_FLAGS ${COMMON_C_LINK_FLAGS}
            -static
            -Wl,-static
            -Wl,--gc-sections
            -Wl,-EL
	    -Wl,-Map=${PROJECT_NAME}.map,--cref
	    -T ${OUTPUT_LD_FILE}
	    #-T ${SDK_PATH}/tools/cmake/stm32f4.ld
	    #-T ${PROJECT_SOURCE_DIR}/compile/stm32f4.ld
            )

set(CMAKE_C_LINK_FLAGS ${CMAKE_C_LINK_FLAGS}
                        ${LINK_FLAGS}
                        )
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_C_LINK_FLAGS}
                        ${LINK_FLAGS}
                        )
# set(CMAKE_EXE_LINKER_FLAGS  ${CMAKE_EXE_LINKER_FLAGS}
#                             ${LINK_FLAGS}
#                             )
# set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS}
#                               ${LINK_FLAGS}
#                               )
# set(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS}
#                               ${LINK_FLAGS}
#                               )


# Convert list to string
string(REPLACE ";" " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE ";" " " LINK_FLAGS "${LINK_FLAGS}")
string(REPLACE ";" " " CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS}")
string(REPLACE ";" " " CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")
