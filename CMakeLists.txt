#
#  Copyright (c) 2019 Christof Ruch. All rights reserved.
#
#  Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
#

project(juce-widgets)

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
	CollapsibleContainer.cpp CollapsibleContainer.h
	ColourHelpers.cpp ColourHelpers.h
	HorizontalLayoutContainer.cpp HorizontalLayoutContainer.h
	InfoText.cpp InfoText.h
	InsetBox.cpp InsetBox.h
	LambdaButtonStrip.cpp LambdaButtonStrip.h
	LambdaMenuModel.cpp LambdaMenuModel.h
	LogView.cpp LogView.h
	MidiChannelEntry.cpp MidiChannelEntry.h
	MidiChannelPropertyEditor.cpp MidiChannelPropertyEditor.h
	MidiLogView.cpp MidiLogView.h
	MouseUpAndDownButton.h
	PatchButton.cpp PatchButton.h
	PatchButtonGrid.h
	ProgressHandlerWindow.cpp ProgressHandlerWindow.h
	PropertyEditor.cpp PropertyEditor.h
	SimpleTable.h
	SynthList.cpp SynthList.h
	Thumbnail.cpp Thumbnail.h
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
target_link_libraries(juce-widgets juce-utils gin ${APPLE_BOOST})
target_include_directories(juce-widgets INTERFACE ${CMAKE_CURRENT_LIST_DIR} PRIVATE ${boost_SOURCE_DIR})

# Pedantic about warnings
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(juce-widgets PRIVATE /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    #target_compile_options(juce-widgets PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
