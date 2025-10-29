# =========================
# Project-core CMakeLists
# =========================
cmake_minimum_required(VERSION 3.20)
project(Project-core C CXX ASM)

# -------------------------
# 交叉编译工具链
# -------------------------
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

# -------------------------
# 编译选项
# -------------------------
add_compile_options(
    -mcpu=cortex-m3
    -mthumb
    -mthumb-interwork
    -ffunction-sections
    -fdata-sections
    -fno-common
    -fmessage-length=0
    -mfloat-abi=soft
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_options(-Og -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_options(-Ofast)
endif()

# -------------------------
# 宏定义
# -------------------------
add_definitions(
    -DUSE_STDPERIPH_DRIVER
    -DSTM32F10X_MD
    -DSTM32F103xB
)

# -------------------------
# 头文件
# -------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/core/inc
    ${CMAKE_SOURCE_DIR}/drivers/STM32F10x_FWLib/inc
    ${CMAKE_SOURCE_DIR}/drivers/system/delay
    ${CMAKE_SOURCE_DIR}/drivers/system/sys
    ${CMAKE_SOURCE_DIR}/drivers/system/usart
    ${CMAKE_SOURCE_DIR}/core/startup
)

# 递归添加 hardware 下所有子目录
file(GLOB_RECURSE HARDWARE_HEADERS "${CMAKE_SOURCE_DIR}/drivers/hardware/*.h")
foreach(hfile ${HARDWARE_HEADERS})
    get_filename_component(hdir ${hfile} DIRECTORY)
    include_directories(${hdir})
endforeach()

# -------------------------
# 源文件
# -------------------------
set(CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/core/src/main.c
    ${CMAKE_SOURCE_DIR}/core/src/system_stm32f10x.c
    ${CMAKE_SOURCE_DIR}/core/src/stm32f10x_it.c
    ${CMAKE_SOURCE_DIR}/core/startup/startup_stm32f10x_md.s
)

file(GLOB_RECURSE SYSTEM_SOURCES_DELAY "${CMAKE_SOURCE_DIR}/drivers/system/delay/*.c")
file(GLOB_RECURSE SYSTEM_SOURCES_SYS "${CMAKE_SOURCE_DIR}/drivers/system/sys/*.c")
file(GLOB_RECURSE SYSTEM_SOURCES_USART "${CMAKE_SOURCE_DIR}/drivers/system/usart/*.c")
file(GLOB_RECURSE HARDWARE_SOURCES "${CMAKE_SOURCE_DIR}/drivers/HARDWARE/*.c")
file(GLOB STD_LIB_SOURCES "${CMAKE_SOURCE_DIR}/drivers/STM32F10x_FWLib/src/*.c")

# 合并所有源文件
set(SOURCES
    ${CORE_SOURCES}
    ${SYSTEM_SOURCES_USART}
    ${SYSTEM_SOURCES_DELAY}
    ${SYSTEM_SOURCES_SYS}       
    ${HARDWARE_SOURCES}
    ${STD_LIB_SOURCES}
)

# 检查文件是否存在
foreach(source_file ${SOURCES})
    if(NOT EXISTS ${source_file})
        message(WARNING "源文件未找到: ${source_file}")
        list(REMOVE_ITEM SOURCES ${source_file})
    else()
        message(STATUS "找到源文件: ${source_file}")
    endif()
endforeach()

# -------------------------
# 链接脚本
# -------------------------
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/core/startup/STM32F103C8Tx_FLASH.ld)
if(NOT EXISTS ${LINKER_SCRIPT})
    message(FATAL_ERROR "链接脚本未找到: ${LINKER_SCRIPT}")
else()
    message(STATUS "找到链接脚本: ${LINKER_SCRIPT}")
endif()

# -------------------------
# 创建可执行文件
# -------------------------
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# =========================
# 链接选项 - 指定 .ld 文件
# =========================
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}.map
    -Wl,--gc-sections
)


# -------------------------
# 生成 hex 和 bin
# -------------------------
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}.bin
    COMMAND ${SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>
    COMMENT "生成 HEX 和 BIN 文件"
)

# -------------------------
# flash 目标
# -------------------------
add_custom_target(flash
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f1x.cfg
            -c "program ${CMAKE_CURRENT_BINARY_DIR}/Project-core.elf verify reset exit"
    DEPENDS Project-core.elf
)
