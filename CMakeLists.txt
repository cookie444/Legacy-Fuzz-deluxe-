cmake_minimum_required(VERSION 3.22)

project(FuzzPedal VERSION 1.0.0)

# Add JUCE
add_subdirectory(JUCE)

# Create the plugin target
juce_add_plugin(FuzzPedal
    COMPANY_NAME "YourCompany"
    PLUGIN_MANUFACTURER_CODE YrCo
    PLUGIN_CODE FzPd
    FORMATS VST3 Standalone
    PRODUCT_NAME "Legacy Fuzz (Deluxe)"
    DESCRIPTION "A fuzz pedal VST with mix, compression, character controls, and BPM-syncable LFO"
    VERSION 1.0.0
)

# Add source files
target_sources(FuzzPedal PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

# Disable VST2 to avoid conflicts
target_compile_definitions(FuzzPedal PRIVATE JUCE_VST3_CAN_REPLACE_VST2=0)

# Link JUCE libraries
target_link_libraries(FuzzPedal
    PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_plugin_client
    juce::juce_dsp
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

