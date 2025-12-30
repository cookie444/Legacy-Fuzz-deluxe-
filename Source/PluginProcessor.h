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
    static constexpr const char* VOLUME_ID = "volume";
    
    // LFO Parameter IDs
    static constexpr const char* LFO_SHAPE_ID = "lfoShape";
    static constexpr const char* LFO_RATE_ID = "lfoRate";
    static constexpr const char* LFO_SYNC_ID = "lfoSync";
    static constexpr const char* LFO_SWING_ID = "lfoSwing";
    
    // Per-parameter LFO controls
    static constexpr const char* LFO_MIX_ENABLE_ID = "lfoMixEnable";
    static constexpr const char* LFO_MIX_AMOUNT_ID = "lfoMixAmount";
    static constexpr const char* LFO_COMPRESSION_ENABLE_ID = "lfoCompressionEnable";
    static constexpr const char* LFO_COMPRESSION_AMOUNT_ID = "lfoCompressionAmount";
    static constexpr const char* LFO_CHARACTER_ENABLE_ID = "lfoCharacterEnable";
    static constexpr const char* LFO_CHARACTER_AMOUNT_ID = "lfoCharacterAmount";
    
    // Parameter pointers
    std::atomic<float>* mixParam = nullptr;
    std::atomic<float>* compressionParam = nullptr;
    std::atomic<float>* fuzzCharacterParam = nullptr;
    std::atomic<float>* volumeParam = nullptr;
    
    // LFO parameter pointers
    std::atomic<float>* lfoShapeParam = nullptr;
    std::atomic<float>* lfoRateParam = nullptr;
    std::atomic<float>* lfoSyncParam = nullptr;
    std::atomic<float>* lfoSwingParam = nullptr;
    std::atomic<float>* lfoMixEnableParam = nullptr;
    std::atomic<float>* lfoMixAmountParam = nullptr;
    std::atomic<float>* lfoCompressionEnableParam = nullptr;
    std::atomic<float>* lfoCompressionAmountParam = nullptr;
    std::atomic<float>* lfoCharacterEnableParam = nullptr;
    std::atomic<float>* lfoCharacterAmountParam = nullptr;
    
    // DSP processing
    juce::dsp::Gain<float> inputGain;
    juce::dsp::Gain<float> outputGain;
    
    // LFO
    juce::dsp::Oscillator<float> lfo;
    double currentSampleRate = 44100.0;
    double lfoPhase = 0.0;
    double lfoIncrement = 0.0;
    float randomHoldValue = 0.0f;
    double randomHoldPhase = 0.0;
    
    // BPM sync
    double hostBPM = 120.0;
    bool hostBPMValid = false;
    
    // LFO helper functions
    float getLFOValue();
    double calculateLFOFrequency();
    float getLFOShapeValue(float phase, int shape);
    
    // Fuzz processing function
    float processFuzz(float input, float character, float compression);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FuzzPedalAudioProcessor)
};

