#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FuzzPedalAudioProcessorEditor::FuzzPedalAudioProcessorEditor (FuzzPedalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set up main parameter sliders
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
    
    // Set up LFO controls
    lfoShapeCombo.addItem("Sine", 1);
    lfoShapeCombo.addItem("Triangle", 2);
    lfoShapeCombo.addItem("Square", 3);
    lfoShapeCombo.addItem("Sawtooth", 4);
    lfoShapeCombo.addItem("Ramp", 5);
    lfoShapeCombo.addItem("Random", 6);
    lfoShapeCombo.setSelectedId(1, juce::dontSendNotification);
    lfoShapeCombo.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff34495e));
    lfoShapeCombo.setColour(juce::ComboBox::textColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoShapeCombo);
    
    lfoRateSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfoRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 18);
    lfoRateSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff9b59b6));
    lfoRateSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2c3e50));
    lfoRateSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoRateSlider);
    
    lfoSyncButton.setButtonText("Sync");
    lfoSyncButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff34495e));
    lfoSyncButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff9b59b6));
    lfoSyncButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffecf0f1));
    lfoSyncButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoSyncButton);
    
    lfoSwingSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfoSwingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 18);
    lfoSwingSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff9b59b6));
    lfoSwingSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2c3e50));
    lfoSwingSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoSwingSlider);
    
    // Set up per-parameter LFO controls
    // Mix LFO
    lfoMixEnableButton.setButtonText("LFO");
    lfoMixEnableButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff34495e));
    lfoMixEnableButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff4a90e2));
    lfoMixEnableButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffecf0f1));
    lfoMixEnableButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoMixEnableButton);
    
    lfoMixAmountSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfoMixAmountSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lfoMixAmountSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff4a90e2));
    lfoMixAmountSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2c3e50));
    lfoMixAmountSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoMixAmountSlider);
    
    // Compression LFO
    lfoCompressionEnableButton.setButtonText("LFO");
    lfoCompressionEnableButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff34495e));
    lfoCompressionEnableButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffe74c3c));
    lfoCompressionEnableButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffecf0f1));
    lfoCompressionEnableButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoCompressionEnableButton);
    
    lfoCompressionAmountSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfoCompressionAmountSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lfoCompressionAmountSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffe74c3c));
    lfoCompressionAmountSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2c3e50));
    lfoCompressionAmountSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoCompressionAmountSlider);
    
    // Character LFO
    lfoCharacterEnableButton.setButtonText("LFO");
    lfoCharacterEnableButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff34495e));
    lfoCharacterEnableButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xfff39c12));
    lfoCharacterEnableButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffecf0f1));
    lfoCharacterEnableButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoCharacterEnableButton);
    
    lfoCharacterAmountSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfoCharacterAmountSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lfoCharacterAmountSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff39c12));
    lfoCharacterAmountSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2c3e50));
    lfoCharacterAmountSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoCharacterAmountSlider);
    
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
    
    lfoShapeLabel.setText("Shape", juce::dontSendNotification);
    lfoShapeLabel.setJustificationType(juce::Justification::centred);
    lfoShapeLabel.attachToComponent(&lfoShapeCombo, false);
    lfoShapeLabel.setColour(juce::Label::textColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoShapeLabel);
    
    lfoRateLabel.setText("Rate", juce::dontSendNotification);
    lfoRateLabel.setJustificationType(juce::Justification::centred);
    lfoRateLabel.attachToComponent(&lfoRateSlider, false);
    lfoRateLabel.setColour(juce::Label::textColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoRateLabel);
    
    lfoSwingLabel.setText("Swing", juce::dontSendNotification);
    lfoSwingLabel.setJustificationType(juce::Justification::centred);
    lfoSwingLabel.attachToComponent(&lfoSwingSlider, false);
    lfoSwingLabel.setColour(juce::Label::textColourId, juce::Colour(0xffecf0f1));
    addAndMakeVisible(&lfoSwingLabel);
    
    // Attach controls to parameters
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "mix", mixSlider);
    compressionAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "compression", compressionSlider);
    fuzzCharacterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "fuzzCharacter", fuzzCharacterSlider);
    
    // Note: AudioParameterChoice uses 0-based indices, combo uses 1-based IDs
    // The attachment handles this automatically, but we need to ensure the combo is set correctly
    lfoShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), "lfoShape", lfoShapeCombo);
    lfoRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "lfoRate", lfoRateSlider);
    lfoSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getValueTreeState(), "lfoSync", lfoSyncButton);
    lfoSwingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "lfoSwing", lfoSwingSlider);
    
    lfoMixEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getValueTreeState(), "lfoMixEnable", lfoMixEnableButton);
    lfoMixAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "lfoMixAmount", lfoMixAmountSlider);
    lfoCompressionEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getValueTreeState(), "lfoCompressionEnable", lfoCompressionEnableButton);
    lfoCompressionAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "lfoCompressionAmount", lfoCompressionAmountSlider);
    lfoCharacterEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getValueTreeState(), "lfoCharacterEnable", lfoCharacterEnableButton);
    lfoCharacterAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "lfoCharacterAmount", lfoCharacterAmountSlider);
    
    // Set window size (larger to accommodate LFO controls)
    setSize (900, 600);
    
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
    g.drawText("LEGACY FUZZ (DELUXE)", getLocalBounds().removeFromTop(60), juce::Justification::centred);
    
    // Decorative line
    g.setColour(juce::Colour(0xff4a90e2));
    g.drawLine(50.0f, 60.0f, (float)getWidth() - 50.0f, 60.0f, 2.0f);
    
    // LFO section label
    g.setColour(juce::Colour(0xff9b59b6));
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    g.drawText("LFO", 50, 80, 100, 25, juce::Justification::left);
}

void FuzzPedalAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop(110); // Space for title and LFO section
    
    // LFO controls section (top)
    auto lfoArea = area.removeFromTop(120);
    int lfoStartX = 50;
    int lfoY = 20;
    
    // LFO Shape combo
    lfoShapeCombo.setBounds(lfoStartX, lfoY, 120, 25);
    lfoStartX += 140;
    
    // LFO Rate
    lfoRateSlider.setBounds(lfoStartX, lfoY - 10, 80, 80);
    lfoStartX += 100;
    
    // LFO Sync button
    lfoSyncButton.setBounds(lfoStartX, lfoY + 10, 60, 30);
    lfoStartX += 80;
    
    // LFO Swing
    lfoSwingSlider.setBounds(lfoStartX, lfoY - 10, 80, 80);
    
    // Main parameter section
    const int knobSize = 150;
    const int spacing = 40;
    const int totalWidth = (knobSize * 3) + (spacing * 2);
    const int startX = (getWidth() - totalWidth) / 2;
    const int yPos = area.getCentreY() - knobSize / 2;
    
    // Per-parameter LFO controls (small buttons and knobs above main knobs)
    const int lfoButtonSize = 50;
    const int lfoKnobSize = 60;
    const int lfoControlY = yPos - 80;
    
    // Mix column
    int mixX = startX;
    lfoMixEnableButton.setBounds(mixX + (knobSize - lfoButtonSize) / 2, lfoControlY, lfoButtonSize, 25);
    lfoMixAmountSlider.setBounds(mixX + (knobSize - lfoKnobSize) / 2, lfoControlY + 30, lfoKnobSize, lfoKnobSize);
    mixSlider.setBounds(mixX, yPos, knobSize, knobSize);
    
    // Compression column
    int compX = startX + knobSize + spacing;
    lfoCompressionEnableButton.setBounds(compX + (knobSize - lfoButtonSize) / 2, lfoControlY, lfoButtonSize, 25);
    lfoCompressionAmountSlider.setBounds(compX + (knobSize - lfoKnobSize) / 2, lfoControlY + 30, lfoKnobSize, lfoKnobSize);
    compressionSlider.setBounds(compX, yPos, knobSize, knobSize);
    
    // Character column
    int charX = startX + (knobSize + spacing) * 2;
    lfoCharacterEnableButton.setBounds(charX + (knobSize - lfoButtonSize) / 2, lfoControlY, lfoButtonSize, 25);
    lfoCharacterAmountSlider.setBounds(charX + (knobSize - lfoKnobSize) / 2, lfoControlY + 30, lfoKnobSize, lfoKnobSize);
    fuzzCharacterSlider.setBounds(charX, yPos, knobSize, knobSize);
}
