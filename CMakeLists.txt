# Please see README.md for build instructions

cmake_minimum_required(VERSION 3.16.4)

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_SYSTEM_PROCESSOR arm)

# Use add_library() with the STATIC option to name the source file in the generated project.
# This avoids running the linker and is intended for use with cross-compiling toolchains that
# cannot link without custom flags or linker scripts
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo MinSizeRel)

if(BUILD_KIT)
	message(NOTICE "Build Kit is defined as ${BUILD_KIT}")
endif()

message(NOTICE "CMAKE_CURRENT_BINARY_DIR is defined as ${CMAKE_CURRENT_BINARY_DIR}")

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(isMultiConfig)
    message(NOTICE "Multi-config generator(${CMAKE_GENERATOR}) is active")
else()
    message(NOTICE "Single-config generator(${CMAKE_GENERATOR}) is active")
	if(DEFINED CMAKE_BUILD_TYPE)
	 	message(NOTICE "CMAKE_BUILD_TYPE is defined as ${CMAKE_BUILD_TYPE}")
	elseif(DEFINED BUILD_TYPE)
		set(CMAKE_BUILD_TYPE ${BUILD_TYPE})
		message(NOTICE "CMAKE_BUILD_TYPE is set to BUILD_TYPE ENV ${CMAKE_BUILD_TYPE}")
	else()
		message(FATAL_ERROR "Please specify the CMAKE_BUILD_TYPE with -DCMAKE_BUILD_TYPE=")
	endif()
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
	message(NOTICE "CMAKE_RUNTIME_OUTPUT_DIRECTORY is defined as ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

cmake_minimum_required(VERSION 3.16.4)

# Project configuration
project(RA6M5_CMakeCXX_Test
	VERSION 1.0.0
	LANGUAGES C CXX ASM)

set(CMAKE_CROSSCOMPILING "TRUE")
set(CMAKE_EXPORT_COMPILE_COMMANDS "TRUE")

include(${CMAKE_CURRENT_LIST_DIR}/Config.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/GeneratedCfg.cmake)
# 覆盖RASC的优化等级
set(RASC_DEBUG_FLAGS "-g;-O0")

# 覆盖C++标准版本：移除旧的 -std 选项，再添加新的
foreach(flag ${RASC_CMAKE_CXX_FLAGS})
    if(flag MATCHES "-std=c\\+\\+[0-9][0-9]")
        list(REMOVE_ITEM RASC_CMAKE_CXX_FLAGS "${flag}")
    endif()
endforeach()
list(APPEND RASC_CMAKE_CXX_FLAGS "-std=c++17")

# If required user can amend RASC generated flags here, e.g. list(REMOVE_ITEM RASC_CMAKE_C_FLAGS "-Wall")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/GeneratedSrc.cmake)

# Collect BIG directory source files


# Add Vasl library sources
file(GLOB_RECURSE VASIL_SOURCE_FILES
    # App
    ${CMAKE_CURRENT_SOURCE_DIR}/App/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/App/App_Core.cpp
    # BSP
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_systick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_button.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_display.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_uart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_softtimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_oled.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_exit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_adc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_wdt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_cmd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_esp8266.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_easymenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP/bsp_tjchmi.cpp
    # Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/NanoPrintf/nanoprintf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/syscalls/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/STimer/MultiTimer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/STimer/Vasl_STimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/OLED/SSD1306_S/OLEDv3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/OLED/SSD1306_S/oled_font.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/EasyMenu/Easy_Menu_User.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/EasyMenu/Easy_Menu_Display.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/EasyMenu/Easy_Menu_Core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/EasyMenu/Easy_Menu_Item.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/EasyMenu/Easy_Menu_Page.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/EasyMenu/Easy_Menu.c
    # Test

    # Vasl
    ${CMAKE_CURRENT_SOURCE_DIR}/Vasl/Vasl_Layer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vasl/Strxx/Vasl_Strxx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vasl/NLC/Vasl_NLC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vasl/Button/Vasl_SSKey.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vasl/Interrupt/Vasl_Interrupt.cpp
)

# Add Vasl sources to the executable
target_sources(${PROJECT_NAME}.elf PRIVATE ${VASIL_SOURCE_FILES})

# Add custom include directories for App, BSP, Lib, Vasl
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/arm/CMSIS-DSP/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/App
    ${CMAKE_CURRENT_SOURCE_DIR}/BSP
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/Vasl
)

# Disable -Wmissing-declarations for syscalls.c (Newlib system calls are not declared in headers)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Lib/syscalls/syscalls.c
    PROPERTIES COMPILE_OPTIONS "-Wno-missing-declarations;-Wno-conversion")

# Add nosys.specs to fix nano.specs linker errors for bare-metal ARM
target_link_options(${PROJECT_NAME}.elf PRIVATE --specs=nosys.specs)

# ========== Memory Report ==========
# RAM: 128KB (131072), FLASH: 512KB (524288)
add_custom_target(size_report
    COMMAND arm-none-eabi-size -B -d ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Memory usage"
)
# 使用 POST_BUILD 在 ELF 构建成功时自动输出内存使用报告
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-size -B -d $<TARGET_FILE:${PROJECT_NAME}.elf>
    COMMENT "Memory usage:"
)

