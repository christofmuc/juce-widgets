#
#  Copyright (c) 2019 Christof Ruch. All rights reserved.
#
#  Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
#

project(juce-widgets)

# Find JUCE, and add the components we need
IF(NOT DEFINED JUCE_LIBRARIES)
	SET(JUCE_DIR_HINT "${CMAKE_CURRENT_LIST_DIR}/../thirdparty")
	find_package(JUCE REQUIRED 
		COMPONENTS 
			juce_core		
			juce_events
			juce_audio_basics
			juce_audio_devices
			juce_data_structures
			juce_gui_basics
			juce_gui_extra
	)
ENDIF()

set(RESOURCE_FILES 
	resources/heart-32.png
	resources/blind-symbol-of-an-opened-eye-with-a-slash.png
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/BinaryResources.h
	COMMAND ${CMAKE_COMMAND} -P createResources.cmake	
	DEPENDS ${RESOURCE_FILES}
	COMMENT "Compiling binary resources"
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

# Define the sources for the static library
set(Sources
	BinaryResources.h
	CategoryButtons.cpp CategoryButtons.h
	HorizontalLayoutContainer.cpp HorizontalLayoutContainer.h
	InsetBox.cpp InsetBox.h
	LambdaButtonStrip.cpp LambdaButtonStrip.h
	LambdaMenuModel.cpp LambdaMenuModel.h
	LogView.cpp LogView.h
	MidiChannelEntry.cpp MidiChannelEntry.h
	MidiLogView.cpp MidiLogView.h
	MouseUpAndDownButton.h
	PatchButton.cpp PatchButton.h
	PatchButtonGrid.cpp PatchButtonGrid.h
	PropertyEditor.cpp PropertyEditor.h
	SimpleTable.h
	SynthList.cpp SynthList.h
	README.md
	LICENSE
	${RESOURCE_FILES}
)

set_source_files_properties(
	BinaryResources.h
	PROPERTIES GENERATED TRUE
)

# Setup library
add_library(juce-widgets ${Sources})
target_link_libraries(juce-widgets juce-utils)
target_include_directories(juce-widgets INTERFACE ${CMAKE_CURRENT_LIST_DIR} PRIVATE ${JUCE_INCLUDES} ${boost_SOURCE_DIR})

# Pedantic about warnings
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(juce-widgets PRIVATE /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    #target_compile_options(juce-widgets PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
