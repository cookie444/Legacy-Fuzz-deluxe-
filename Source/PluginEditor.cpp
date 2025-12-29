#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FuzzPedalAudioProcessorEditor::FuzzPedalAudioProcessorEditor (FuzzPedalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set up sliders
    mixSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    mixSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff4a90e2));
    mixSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2c3e50));
    mixSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&mixSlider);
    
    compressionSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    compressionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    compressionSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffe74c3c));
    compressionSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2c3e50));
    compressionSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&compressionSlider);
    
    fuzzCharacterSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    fuzzCharacterSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    fuzzCharacterSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff39c12));
    fuzzCharacterSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2c3e50));
    fuzzCharacterSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&fuzzCharacterSlider);
    
    // Set up labels
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setColour(juce::Label::textColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&mixLabel);
    
    compressionLabel.setText("Compression", juce::dontSendNotification);
    compressionLabel.setJustificationType(juce::Justification::centred);
    compressionLabel.attachToComponent(&compressionSlider, false);
    compressionLabel.setColour(juce::Label::textColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&compressionLabel);
    
    fuzzCharacterLabel.setText("Fuzz Character", juce::dontSendNotification);
    fuzzCharacterLabel.setJustificationType(juce::Justification::centred);
    fuzzCharacterLabel.attachToComponent(&fuzzCharacterSlider, false);
    fuzzCharacterLabel.setColour(juce::Label::textColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&fuzzCharacterLabel);
    
    // Attach sliders to parameters
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "mix", mixSlider);
    compressionAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "compression", compressionSlider);
    fuzzCharacterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "fuzzCharacter", fuzzCharacterSlider);
    
    // Set window size
    setSize (600, 400);
    
    // Set dark theme
    lookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff1a1a1a));
}

FuzzPedalAudioProcessorEditor::~FuzzPedalAudioProcessorEditor()
{
}

//==============================================================================
void FuzzPedalAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background gradient
    juce::ColourGradient gradient(
        juce::Colour(0xff2c3e50), 0, 0,
        juce::Colour(0xff1a1a1a), 0, (float)getHeight(),
        false
    );
    g.setGradientFill(gradient);
    g.fillAll();
    
    // Title
    g.setColour(juce::Colour(0xffecf0f1));
    g.setFont(juce::Font(32.0f, juce::Font::bold));
    g.drawText("LEGACY FUZZ", getLocalBounds().removeFromTop(60), juce::Justification::centred);
    
    // Decorative line
    g.setColour(juce::Colour(0xff4a90e2));
    g.drawLine(50.0f, 60.0f, (float)getWidth() - 50.0f, 60.0f, 2.0f);
}

void FuzzPedalAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop(80); // Space for title
    
    const int knobSize = 150;
    const int spacing = 30;
    const int totalWidth = (knobSize * 3) + (spacing * 2);
    const int startX = (getWidth() - totalWidth) / 2;
    const int yPos = (getHeight() - 80 - knobSize) / 2 + 80;
    
    mixSlider.setBounds(startX, yPos, knobSize, knobSize);
    compressionSlider.setBounds(startX + knobSize + spacing, yPos, knobSize, knobSize);
    fuzzCharacterSlider.setBounds(startX + (knobSize + spacing) * 2, yPos, knobSize, knobSize);
}

