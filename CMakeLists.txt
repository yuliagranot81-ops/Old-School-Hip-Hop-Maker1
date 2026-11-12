cmake_minimum_required(VERSION 3.22)
project(OldSchoolHipHopMaker VERSION 0.1.0)

# JUCE is checked out by the included GitHub Actions workflow into ./JUCE.
add_subdirectory(JUCE)

juce_add_plugin(OldSchoolHipHopMaker
    COMPANY_NAME "Old School Audio"
    PLUGIN_MANUFACTURER_CODE OSAU
    PLUGIN_CODE OSHM
    FORMATS VST3
    PRODUCT_NAME "Old School Hip Hop Maker"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(OldSchoolHipHopMaker PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

target_compile_features(OldSchoolHipHopMaker PRIVATE cxx_std_17)
target_link_libraries(OldSchoolHipHopMaker PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
