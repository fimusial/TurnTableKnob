cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(TurnTableKnob
    VERSION 1.0.0.0 
    DESCRIPTION "TurnTableKnob VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(TurnTableKnob
    source/audiofile/audiosegment32.cpp
    source/audiofile/audiosegment32.h
    source/audiofile/dr_wav.h
    source/processing/declicker.cpp
    source/processing/declicker.h
    source/processing/smoothfollowparameter.cpp
    source/processing/smoothfollowparameter.h
    source/uicontrols/autoplaycontrol.cpp
    source/uicontrols/autoplaycontrol.h
    source/uicontrols/autoplaycontrolfactory.cpp
    source/uicontrols/autoplaycontrolfactory.h
    source/uicontrols/cdrawcontextextensions.h
    source/uicontrols/holdcontrol.cpp
    source/uicontrols/holdcontrol.h
    source/uicontrols/itimelinecontrolprocessor.h
    source/uicontrols/timelinecontrol.cpp
    source/uicontrols/timelinecontrol.h
    source/uicontrols/timelinecontrolfactory.cpp
    source/uicontrols/timelinecontrolfactory.h
    source/cids.h
    source/consts.h
    source/controller.cpp
    source/controller.h
    source/entry.cpp
    source/mydebug.h
    source/processor.cpp
    source/processor.h
    source/version.h
)

# VSTGUI
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(TurnTableKnob
        PRIVATE
            resource/editor.uidesc
    )
    target_link_libraries(TurnTableKnob
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(TurnTableKnob
        RESOURCES
            "resource/editor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)

target_link_libraries(TurnTableKnob
    PRIVATE
        sdk
)

smtg_target_configure_version_file(TurnTableKnob)

target_sources(TurnTableKnob PRIVATE 
    resource/win32resource.rc
)
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TurnTableKnob)

    smtg_target_set_debug_executable(TurnTableKnob
        "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
        "--pluginfolder \"$(OutDir)/\""
    )
endif()

source_group("Header Files/audiofile" REGULAR_EXPRESSION ".*/source/audiofile/.*\\.h$")
source_group("Header Files/processing" REGULAR_EXPRESSION ".*/source/processing/.*\\.h$")
source_group("Header Files/uicontrols" REGULAR_EXPRESSION ".*/source/uicontrols/.*\\.h$")

source_group("Source Files/audiofile" REGULAR_EXPRESSION ".*/source/audiofile/.*\\.cpp$")
source_group("Source Files/processing" REGULAR_EXPRESSION ".*/source/processing/.*\\.cpp$")
source_group("Source Files/uicontrols" REGULAR_EXPRESSION ".*/source/uicontrols/.*\\.cpp$")
