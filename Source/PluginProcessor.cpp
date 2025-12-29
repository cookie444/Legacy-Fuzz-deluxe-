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
                [](float value, int) { return juce::String(value, 1) + " %"; })
        })
{
    mixParam = parameters.getRawParameterValue(MIX_ID);
    compressionParam = parameters.getRawParameterValue(COMPRESSION_ID);
    fuzzCharacterParam = parameters.getRawParameterValue(FUZZ_CHARACTER_ID);
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
    
    inputGain.prepare(spec);
    outputGain.prepare(spec);
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

    // Get parameter values
    float mix = mixParam->load() / 100.0f;
    float compression = compressionParam->load() / 100.0f;
    float fuzzCharacter = fuzzCharacterParam->load() / 100.0f;

    // Process each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float input = channelData[sample];
            
            // Process fuzz
            float fuzzed = processFuzz(input, fuzzCharacter, compression);
            
            // Mix dry and wet
            channelData[sample] = input * (1.0f - mix) + fuzzed * mix;
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

