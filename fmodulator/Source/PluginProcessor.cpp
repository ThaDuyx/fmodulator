/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MiniProjectSPAudioProcessor::MiniProjectSPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),  apvts (*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoices());
}

MiniProjectSPAudioProcessor::~MiniProjectSPAudioProcessor()
{
}

//==============================================================================
const juce::String MiniProjectSPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MiniProjectSPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MiniProjectSPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MiniProjectSPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MiniProjectSPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MiniProjectSPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MiniProjectSPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MiniProjectSPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MiniProjectSPAudioProcessor::getProgramName (int index)
{
    return {};
}

void MiniProjectSPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MiniProjectSPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoices*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void MiniProjectSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MiniProjectSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MiniProjectSPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    
    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoices*>(synth.getVoice(i))) {
            // Osc Controls
            // ADSR
            
            auto& attack = *apvts.getRawParameterValue("A");
            auto& decay = *apvts.getRawParameterValue("D");
            auto& sustain = *apvts.getRawParameterValue("S");
            auto& release = *apvts.getRawParameterValue("R");
            
            auto& mAttack = *apvts.getRawParameterValue("mA");
            auto& mDecay = *apvts.getRawParameterValue("mD");
            auto& mSustain = *apvts.getRawParameterValue("mS");
            auto& mRelease = *apvts.getRawParameterValue("mR");
            
            auto& depth = *apvts.getRawParameterValue("DEPTHFM");
            auto& freq = *apvts.getRawParameterValue("FREQFM");
            
            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
            voice->setModulationParams(depth.load(), freq.load());
            voice->updateADSRModParams(mAttack.load(), mDecay.load(), mSustain.load(), mRelease.load());
        }
    }
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
}

//==============================================================================
bool MiniProjectSPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MiniProjectSPAudioProcessor::createEditor()
{
    return new MiniProjectSPAudioProcessorEditor (*this);
}

//==============================================================================
void MiniProjectSPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MiniProjectSPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MiniProjectSPAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MiniProjectSPAudioProcessor::createParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Carrier envelope
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"A", 1}, "Attack", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"D", 1}, "Decay", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"S", 1}, "Sustain", juce::NormalisableRange<float> {0.0f, 1.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"R", 1}, "Release", juce::NormalisableRange<float> {0.1f, 3.0f, 0.1f}, 0.4f));
    
    // Modulator envelope
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"mA", 1}, "mAttack", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"mD", 1}, "mDecay", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"mS", 1}, "mSustain", juce::NormalisableRange<float> {0.0f, 1.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"mR", 1}, "mRelease", juce::NormalisableRange<float> {0.1f, 3.0f, 0.1f}, 0.4f));
    
    // Controlling Modulator frequency
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"FREQFM", 1}, "FreqFM", juce::NormalisableRange<float> {0.0f, 1000.0f, 1.0f}, 0.0f));
    // Controlling Modulator depth
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"DEPTHFM", 1}, "DepthFM", juce::NormalisableRange<float> {0.0f, 1000.0f, 1.0f}, 0.0f));

    return { params.begin(), params.end() };
}
