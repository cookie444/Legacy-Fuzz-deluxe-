#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FuzzPedalAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FuzzPedalAudioProcessorEditor (FuzzPedalAudioProcessor&);
    ~FuzzPedalAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    FuzzPedalAudioProcessor& audioProcessor;
    
    // Sliders
    juce::Slider mixSlider;
    juce::Slider compressionSlider;
    juce::Slider fuzzCharacterSlider;
    
    // Labels
    juce::Label mixLabel;
    juce::Label compressionLabel;
    juce::Label fuzzCharacterLabel;
    
    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compressionAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fuzzCharacterAttachment;
    
    // Custom look and feel for knobs
    juce::LookAndFeel_V4 lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FuzzPedalAudioProcessorEditor)
};

