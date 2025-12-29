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
    
    // Main parameter sliders
    juce::Slider mixSlider;
    juce::Slider compressionSlider;
    juce::Slider fuzzCharacterSlider;
    
    // LFO controls
    juce::ComboBox lfoShapeCombo;
    juce::Slider lfoRateSlider;
    juce::ToggleButton lfoSyncButton;
    juce::Slider lfoSwingSlider;
    
    // Per-parameter LFO controls
    juce::ToggleButton lfoMixEnableButton;
    juce::Slider lfoMixAmountSlider;
    juce::ToggleButton lfoCompressionEnableButton;
    juce::Slider lfoCompressionAmountSlider;
    juce::ToggleButton lfoCharacterEnableButton;
    juce::Slider lfoCharacterAmountSlider;
    
    // Labels
    juce::Label mixLabel;
    juce::Label compressionLabel;
    juce::Label fuzzCharacterLabel;
    juce::Label lfoShapeLabel;
    juce::Label lfoRateLabel;
    juce::Label lfoSwingLabel;
    
    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compressionAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fuzzCharacterAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfoShapeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lfoSyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoSwingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lfoMixEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoMixAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lfoCompressionEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoCompressionAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lfoCharacterEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoCharacterAmountAttachment;
    
    // Custom look and feel for knobs
    juce::LookAndFeel_V4 lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FuzzPedalAudioProcessorEditor)
};

