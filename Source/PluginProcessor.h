#pragma once

#include "JuceHeader.h"

//==============================================================================
/**
*/
class FuzzPedalAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FuzzPedalAudioProcessor();
    ~FuzzPedalAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState& getValueTreeState() { return parameters; }

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;
    
    // Parameter IDs
    static constexpr const char* MIX_ID = "mix";
    static constexpr const char* COMPRESSION_ID = "compression";
    static constexpr const char* FUZZ_CHARACTER_ID = "fuzzCharacter";
    
    // Parameter pointers
    std::atomic<float>* mixParam = nullptr;
    std::atomic<float>* compressionParam = nullptr;
    std::atomic<float>* fuzzCharacterParam = nullptr;
    
    // DSP processing
    juce::dsp::Gain<float> inputGain;
    juce::dsp::Gain<float> outputGain;
    
    // Fuzz processing function
    float processFuzz(float input, float character, float compression);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FuzzPedalAudioProcessor)
};

