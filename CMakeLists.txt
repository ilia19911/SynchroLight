# обязательная строка, устанавливает требуемую версию CMake
cmake_minimum_required(VERSION 3.15)

# название проекта, версия, описание в свободной форме
project(MyProject
		VERSION 1.0
		DESCRIPTION "NEW_CMAKE_PROJ "
# языки проекта, если не указать, будет принято, что проект на С и С++
		LANGUAGES C ASM)

# более информативный вывод в процессе сборки проекта
SET(CMAKE_VERBOSE_MAKEFILE)


# строка ниже определяет, нужна ли кросс-компиляция
# если сборка будет происходить для той же системы, что и
# текущая, 2 переменные равны. Иначе - не равны.
if (NOT CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME )

message("INCLUDE_LINT = " ${INCLUDE_LINT})
if (${INCLUDE_LINT})
# PC-Lint function
include(${TOOLCHAIN_PREFIX}/pc-lint2.cmake)
message("linting included")
endif()

# добавил дефайн, который можно анализировать в тестах, к примеру
add_definitions(-DMCU_TARGET)

# путь к директории с аccемблерными startup файлами
set(STARTUP_CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# просто отладочный вывод
message("STARTUP_CODE_DIR = " ${STARTUP_CODE_DIR})

# путь к директории со скриптами линкера
set(LINKER_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message("LINKER_SCRIPTS_DIR = " ${LINKER_SCRIPTS_DIR})

# просто отладочный вывод
message (STATUS "CMAKE_TOOLCHAIN_FILE = " ${CMAKE_TOOLCHAIN_FILE})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = " ${CMAKE_CURRENT_SOURCE_DIR})

##assembler flags
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wall -fdata-sections -ffunction-sections")

set(GCC_COMPILE_FLAGS "${GCC_COMPILE_FLAGS} -Wall -fmessage-length=0 -ffunction-sections --specs=nano.specs")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS} ${CMAKE_CXX_DEBUG_FLAGS}")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COMPILE_FLAGS} ${CMAKE_CXX_DEBUG_FLAGS}")


# подключаем cmake файлы, сгенерированные скриптом
#message(FATAL_ERROR "Comment out this line after keil-converted cmake lists are included!")
#message(FATAL_ERROR "Закомментируйте эту строку после добавления cmakе файлов, созданных keil2cmake.py!"
include(${CMAKE_CURRENT_SOURCE_DIR}/Target_1_sources_list.cmake)



# отладочный вывод
message(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_C_FLAGS = " ${CMAKE_CXX_FLAGS})

# добавляем таргет PC-Lint
if(COMMAND add_pc_lint)
# establish list of compile defifntiond
	set (comp_defs_list)
# скорее всего, эти спсики пусты - ничего страшного
	list(APPEND comp_defs_list ${D_MCU})
	list(APPEND comp_defs_list ${D_MCU_EXACT})
	list(APPEND comp_defs_list ${D_HSE_VAL})

# fantastic loop
	foreach(s IN LISTS ${EXEC_NAME}_LIST_OF_SOURCES)

		set(t_name)
		string(REPLACE "/" "_" t_name ${s})

		add_pc_lint(
# compile defines
				"${comp_defs_list}"
# target
				${t_name}
				${EXEC_NAME}
				${s}
				)
	endforeach()
endif(COMMAND add_pc_lint)

else()
	message("Non-MCU target detected")

		# подключаем список исходников
		include(${CMAKE_CURRENT_SOURCE_DIR}/sources_list_desktop.cmake)

	add_definitions(-DDESKTOP_TARGET)
		add_executable(tests ${GROUP_SRC_I386} ${LIST_OF_SOURCES})

		target_link_libraries(
			tests
			c		# c runtime
			m		# math
			)

#здесь пока пусто

endif()

# инфа и диагностика
MESSAGE("*********************")
message("CMAKE_SYSTEM_NAME = " ${CMAKE_SYSTEM_NAME} "      CMAKE_HOST_SYSTEM_NAME = " ${CMAKE_HOST_SYSTEM_NAME})
message("CMAKE_SYSTEM = " ${CMAKE_SYSTEM} "      CMAKE_HOST_SYSTEM = " ${CMAKE_HOST_SYSTEM})
MESSAGE("CMAKE_C_COMPILER = " ${CMAKE_C_COMPILER})
MESSAGE("CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
MESSAGE("*********************")
