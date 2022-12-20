#[[
MIT License

Copyright (c) 2019-2023 Christof Ruch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
]]

project(juce-widgets)

# If you also use https://github.com/FigBug/Gin, turn this on
OPTION(GIN_AVAILABLE "Is the Gin library available and included into CMake" OFF)

set(RESOURCE_FILES
	resources/heart-32.png
	resources/blind-symbol-of-an-opened-eye-with-a-slash.png
	resources/iconmonstr-magnifier-6-64.png
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
	CustomizedRotary.cpp CustomizedRotary.h
	HorizontalLayoutContainer.cpp HorizontalLayoutContainer.h
	IconHelper.cpp IconHelper.h
	InfoText.cpp InfoText.h
	InsetBox.cpp InsetBox.h
	FlexBoxHelper.cpp FlexBoxHelper.h
	LambdaButtonStrip.cpp LambdaButtonStrip.h
	LambdaMenuModel.cpp LambdaMenuModel.h
	LayoutConstants.h
	LogView.cpp LogView.h
	MidiChannelEntry.cpp MidiChannelEntry.h
	MidiChannelPropertyEditor.cpp MidiChannelPropertyEditor.h
	MidiLogView.cpp MidiLogView.h
	MouseUpAndDownButton.h
	PatchButton.cpp PatchButton.h
	PatchButtonGrid.h
	ProgressHandlerWindow.cpp ProgressHandlerWindow.h
	PropertyEditor.cpp PropertyEditor.h
	RecycleBin.cpp RecycleBin.h
	SimpleTable.h
	SplitteredComponent.cpp SplitteredComponent.h
	SynthList.cpp SynthList.h
	TextSearchBox.cpp TextSearchBox.h
	Thumbnail.cpp Thumbnail.h
	TreeViewNode.h TreeViewNode.cpp
	TwoLabelSlider.cpp TwoLabelSlider.h
	README.md
	LICENSE
	${RESOURCE_FILES}
)

set_source_files_properties(
	BinaryResources.h
	PROPERTIES GENERATED TRUE
)

IF(GIN_AVAILABLE)
	SET(GIN_LINK_TARGET gin)
ENDIF()

# Setup library
add_library(juce-widgets ${Sources})
target_link_libraries(juce-widgets juce-utils ${GIN_LINK_TARGET} ${APPLE_BOOST} fmt::fmt)
target_include_directories(juce-widgets INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pedantic about warnings and precompiled headers
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(juce-widgets PRIVATE /W4 /WX)
	if(REALLY_USE_PRECOMPILED_HEADERS)
		target_precompile_headers(juce-widgets
			PRIVATE
				<JuceHeader.h>
				<memory>
				<map>
				<set>
				<string>
			)
	endif()
else()
    # lots of warnings and all warnings as errors
    #target_compile_options(juce-widgets PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
