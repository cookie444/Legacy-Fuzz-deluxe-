#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FuzzPedalAudioProcessor::FuzzPedalAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters(*this, nullptr, juce::Identifier("FuzzPedalParams"),
        {
            std::make_unique<juce::AudioParameterFloat>(
                MIX_ID, "Mix",
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + " %"; }),
            
            std::make_unique<juce::AudioParameterFloat>(
                COMPRESSION_ID, "Compression",
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + " %"; }),
            
            std::make_unique<juce::AudioParameterFloat>(
                FUZZ_CHARACTER_ID, "Fuzz Character",
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + " %"; }),
            
            std::make_unique<juce::AudioParameterFloat>(
                VOLUME_ID, "Volume",
                juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { 
                    if (value > 0.0f)
                        return "+" + juce::String(value, 1) + " dB";
                    return juce::String(value, 1) + " dB";
                }),
            
            // LFO Parameters
            std::make_unique<juce::AudioParameterChoice>(
                LFO_SHAPE_ID, "LFO Shape",
                juce::StringArray("Sine", "Triangle", "Square", "Sawtooth", "Ramp", "Random"), 0),
            
            std::make_unique<juce::AudioParameterFloat>(
                LFO_RATE_ID, "LFO Rate",
                juce::NormalisableRange<float>(0.1f, 20.0f, 0.01f), 1.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 2) + " Hz"; }),
            
            std::make_unique<juce::AudioParameterBool>(
                LFO_SYNC_ID, "LFO Sync", false),
            
            std::make_unique<juce::AudioParameterFloat>(
                LFO_SWING_ID, "LFO Swing",
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + " %"; }),
            
            // Per-parameter LFO controls
            std::make_unique<juce::AudioParameterBool>(
                LFO_MIX_ENABLE_ID, "LFO Mix Enable", false),
            std::make_unique<juce::AudioParameterFloat>(
                LFO_MIX_AMOUNT_ID, "",  // Empty name to prevent label display
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + " %"; }),
            
            std::make_unique<juce::AudioParameterBool>(
                LFO_COMPRESSION_ENABLE_ID, "Compression Enable", false),
            std::make_unique<juce::AudioParameterFloat>(
                LFO_COMPRESSION_AMOUNT_ID, "",  // Empty name to prevent label display
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + " %"; }),
            
            std::make_unique<juce::AudioParameterBool>(
                LFO_CHARACTER_ENABLE_ID, "Character Enable", false),
            std::make_unique<juce::AudioParameterFloat>(
                LFO_CHARACTER_AMOUNT_ID, "",  // Empty name to prevent label display
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f,
                juce::String(), juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + " %"; })
        })
{
    mixParam = parameters.getRawParameterValue(MIX_ID);
    compressionParam = parameters.getRawParameterValue(COMPRESSION_ID);
    fuzzCharacterParam = parameters.getRawParameterValue(FUZZ_CHARACTER_ID);
    volumeParam = parameters.getRawParameterValue(VOLUME_ID);
    
    lfoShapeParam = parameters.getRawParameterValue(LFO_SHAPE_ID);
    lfoRateParam = parameters.getRawParameterValue(LFO_RATE_ID);
    lfoSyncParam = parameters.getRawParameterValue(LFO_SYNC_ID);
    lfoSwingParam = parameters.getRawParameterValue(LFO_SWING_ID);
    lfoMixEnableParam = parameters.getRawParameterValue(LFO_MIX_ENABLE_ID);
    lfoMixAmountParam = parameters.getRawParameterValue(LFO_MIX_AMOUNT_ID);
    lfoCompressionEnableParam = parameters.getRawParameterValue(LFO_COMPRESSION_ENABLE_ID);
    lfoCompressionAmountParam = parameters.getRawParameterValue(LFO_COMPRESSION_AMOUNT_ID);
    lfoCharacterEnableParam = parameters.getRawParameterValue(LFO_CHARACTER_ENABLE_ID);
    lfoCharacterAmountParam = parameters.getRawParameterValue(LFO_CHARACTER_AMOUNT_ID);
}

FuzzPedalAudioProcessor::~FuzzPedalAudioProcessor()
{
}

//==============================================================================
const juce::String FuzzPedalAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FuzzPedalAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FuzzPedalAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FuzzPedalAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FuzzPedalAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FuzzPedalAudioProcessor::getNumPrograms()
{
    return 1;
}

int FuzzPedalAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FuzzPedalAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FuzzPedalAudioProcessor::getProgramName (int index)
{
    return {};
}

void FuzzPedalAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FuzzPedalAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());
    
    currentSampleRate = sampleRate;
    inputGain.prepare(spec);
    outputGain.prepare(spec);
    
    // Initialize LFO
    lfoPhase = 0.0;
    lfoIncrement = calculateLFOFrequency() / sampleRate;
    
    // Get host BPM if available
    if (auto* playHead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo info;
        if (playHead->getCurrentPosition(info))
        {
            if (info.bpm > 0.0)
            {
                hostBPM = info.bpm;
                hostBPMValid = true;
            }
        }
    }
}

void FuzzPedalAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FuzzPedalAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    #endif

    return true;
  #endif
}
#endif

void FuzzPedalAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Update host BPM if available
    if (auto* playHead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo info;
        if (playHead->getCurrentPosition(info))
        {
            if (info.bpm > 0.0)
            {
                hostBPM = info.bpm;
                hostBPMValid = true;
            }
        }
    }
    
    // Update LFO frequency
    lfoIncrement = calculateLFOFrequency() / currentSampleRate;
    
    // Get base parameter values
    float baseMix = mixParam->load() / 100.0f;
    float baseCompression = compressionParam->load() / 100.0f;
    float baseFuzzCharacter = fuzzCharacterParam->load() / 100.0f;
    
    // Get LFO parameters
    bool lfoMixEnable = lfoMixEnableParam->load() > 0.5f;
    float lfoMixAmount = lfoMixAmountParam->load() / 100.0f;
    bool lfoCompressionEnable = lfoCompressionEnableParam->load() > 0.5f;
    float lfoCompressionAmount = lfoCompressionAmountParam->load() / 100.0f;
    bool lfoCharacterEnable = lfoCharacterEnableParam->load() > 0.5f;
    float lfoCharacterAmount = lfoCharacterAmountParam->load() / 100.0f;

    // Process each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Get LFO value
            float lfoValue = getLFOValue();
            
            // Apply LFO modulation to parameters
            float mix = baseMix;
            float compression = baseCompression;
            float fuzzCharacter = baseFuzzCharacter;
            
            if (lfoMixEnable)
            {
                float modulation = lfoValue * lfoMixAmount;
                mix = juce::jlimit(0.0f, 1.0f, baseMix + modulation);
            }
            
            if (lfoCompressionEnable)
            {
                float modulation = lfoValue * lfoCompressionAmount;
                compression = juce::jlimit(0.0f, 1.0f, baseCompression + modulation);
            }
            
            if (lfoCharacterEnable)
            {
                float modulation = lfoValue * lfoCharacterAmount;
                fuzzCharacter = juce::jlimit(0.0f, 1.0f, baseFuzzCharacter + modulation);
            }
            
            float input = channelData[sample];
            
            // Process fuzz
            float fuzzed = processFuzz(input, fuzzCharacter, compression);
            
            // Mix dry and wet
            float output = input * (1.0f - mix) + fuzzed * mix;
            
            // Apply volume boost/cut (convert dB to linear gain)
            float volumeDb = volumeParam->load();
            float volumeGain = juce::Decibels::decibelsToGain(volumeDb);
            channelData[sample] = output * volumeGain;
        }
    }
}

float FuzzPedalAudioProcessor::processFuzz(float input, float character, float compression)
{
    // Normalize input to prevent excessive gain
    float normalizedInput = juce::jlimit(-1.0f, 1.0f, input * 2.0f);
    
    // Character controls the type of fuzz:
    // 0% = soft clipping (tanh)
    // 50% = medium clipping (asymmetric)
    // 100% = hard clipping (hard limit)
    float fuzzed;
    
    if (character < 0.33f)
    {
        // Soft clipping (tanh)
        float drive = 1.0f + character * 6.0f; // 1x to 3x drive
        fuzzed = std::tanh(normalizedInput * drive);
    }
    else if (character < 0.66f)
    {
        // Asymmetric clipping
        float drive = 3.0f + (character - 0.33f) * 4.0f; // 3x to 5x drive
        float driven = normalizedInput * drive;
        fuzzed = driven > 0.0f 
            ? std::tanh(driven * 0.7f) 
            : std::tanh(driven * 1.3f);
    }
    else
    {
        // Hard clipping
        float drive = 5.0f + (character - 0.66f) * 10.0f; // 5x to 15x drive
        float driven = normalizedInput * drive;
        fuzzed = juce::jlimit(-1.0f, 1.0f, driven);
    }
    
    // Compression: reduces dynamic range
    // 0% = no compression, 100% = heavy compression
    if (compression > 0.0f)
    {
        float threshold = 0.3f;
        float ratio = 1.0f + compression * 9.0f; // 1:1 to 10:1 ratio
        
        float absInput = std::abs(fuzzed);
        if (absInput > threshold)
        {
            float excess = absInput - threshold;
            float compressed = threshold + excess / ratio;
            fuzzed = fuzzed > 0.0f ? compressed : -compressed;
        }
    }
    
    return fuzzed * 0.5f; // Output gain reduction
}

//==============================================================================
double FuzzPedalAudioProcessor::calculateLFOFrequency()
{
    bool sync = lfoSyncParam->load() > 0.5f;
    float rate = lfoRateParam->load();
    
    if (sync && hostBPMValid)
    {
        // Sync to BPM - rate represents note divisions
        // 1.0 = whole note, 2.0 = half note, 4.0 = quarter note, etc.
        double noteLength = 60.0 / hostBPM; // seconds per whole note
        double division = noteLength / rate;
        return 1.0 / division;
    }
    else
    {
        // Free rate in Hz
        return rate;
    }
}

float FuzzPedalAudioProcessor::getLFOShapeValue(float phase, int shape)
{
    // Normalize phase to 0-1
    float normalizedPhase = phase - std::floor(phase);
    
    switch (shape)
    {
        case 0: // Sine
            return std::sin(normalizedPhase * juce::MathConstants<float>::twoPi) * 0.5f + 0.5f;
        
        case 1: // Triangle
            if (normalizedPhase < 0.5f)
                return normalizedPhase * 2.0f;
            else
                return 2.0f - normalizedPhase * 2.0f;
        
        case 2: // Square
            return normalizedPhase < 0.5f ? 1.0f : 0.0f;
        
        case 3: // Sawtooth
            return normalizedPhase;
        
        case 4: // Ramp (reverse sawtooth)
            return 1.0f - normalizedPhase;
        
        case 5: // Random (sample and hold)
            // Sample and hold - hold value until next cycle
            return randomHoldValue;
        
        default:
            return normalizedPhase;
    }
}

float FuzzPedalAudioProcessor::getLFOValue()
{
    int shape = static_cast<int>(lfoShapeParam->load());
    float swing = lfoSwingParam->load() / 100.0f;
    
    // Update phase
    double oldPhase = lfoPhase;
    lfoPhase += lfoIncrement;
    
    // Handle sample and hold for random shape
    if (shape == 5)
    {
        // Generate new random value when phase wraps
        if (lfoPhase >= 1.0 || (oldPhase < 0.5 && lfoPhase >= 0.5))
        {
            randomHoldValue = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        }
    }
    
    if (lfoPhase >= 1.0)
        lfoPhase -= 1.0;
    
    // Get base LFO value (0-1)
    float baseValue = getLFOShapeValue(static_cast<float>(lfoPhase), shape);
    
    // Apply swing (stretches the first half, compresses the second half)
    if (swing > 0.0f)
    {
        float swingAmount = swing * 0.5f; // Max 50% swing
        if (baseValue < 0.5f)
        {
            // Stretch first half
            baseValue = baseValue * (1.0f + swingAmount) / (1.0f + swingAmount * 0.5f);
        }
        else
        {
            // Compress second half
            float secondHalf = (baseValue - 0.5f) * 2.0f;
            secondHalf = secondHalf * (1.0f - swingAmount);
            baseValue = 0.5f + secondHalf * 0.5f;
        }
    }
    
    // Convert to -1 to 1 range for modulation
    return (baseValue - 0.5f) * 2.0f;
}

//==============================================================================
bool FuzzPedalAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* FuzzPedalAudioProcessor::createEditor()
{
    return new FuzzPedalAudioProcessorEditor (*this);
}

//==============================================================================
void FuzzPedalAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void FuzzPedalAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FuzzPedalAudioProcessor();
}

