/*
  ==============================================================================

    SynthVoices.cpp
    Created: 6 Feb 2023 6:32:08pm
    Author:  Simon Andersen

  ==============================================================================
*/

#include "SynthVoices.h"

bool SynthVoices::canPlaySound(juce::SynthesiserSound * sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoices::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
    carrier.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + freqModulator);
    adsr.noteOn();
    modEnvelope.noteOn();
    previousMIDIInfo = midiNoteNumber;
}

void SynthVoices::stopNote (float velocity, bool allowTailOff) {
    adsr.noteOff();
    modEnvelope.noteOff();
    
    if(!allowTailOff || !adsr.isActive()) {
        clearCurrentNote();
    }
}

void SynthVoices::controllerMoved(int controllerNumber, int newControllerValue) {

}

void SynthVoices::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoices::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    
    adsr.setSampleRate(sampleRate);
    modEnvelope.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    carrier.prepare(spec);
    modulator.prepare(spec);
    gain.prepare(spec);
    
    gain.setGainLinear(0.05f);
    
    isPrepared = true;
}

void SynthVoices::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    adsr.setParameters(adsrParams);
}

void SynthVoices::updateADSRModParams(const float attack, const float decay, const float sustain, const float release) {
    modulatorADSRParams.attack = attack;
    modulatorADSRParams.decay = decay;
    modulatorADSRParams.sustain = sustain;
    modulatorADSRParams.release = release;
    
    modEnvelope.setParameters(modulatorADSRParams);
}

void SynthVoices::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);
    
    if(!isVoiceActive()) {
        return;
    }
    
    buffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    buffer.clear();
    
    // Checking for sample by sample processing instead of using the entire block of data
    // This way we are iterating over every channel and sample in the block.
    juce::dsp::AudioBlock<float> audioBlock { buffer };
    
    carrier.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    modEnvelope.applyEnvelopeToBuffer(buffer, 0, numSamples);
    
    for (int channelBlock = 0; channelBlock < audioBlock.getNumChannels(); channelBlock++) {
        for (int sampleBlock = 0; sampleBlock < audioBlock.getNumSamples(); sampleBlock++) {
            freqModulator = modulator.processSample(audioBlock.getSample(channelBlock, sampleBlock)) * depthModulator;
        }
    }
    
    adsr.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples());
    
    
    for(int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
        outputBuffer.addFrom(channel, startSample, buffer, channel, 0, numSamples);
        
        if(!adsr.isActive()) {
            clearCurrentNote();
        }
    }
}

void SynthVoices::setModulationParams(const float depth, const float freq) {
    modulator.setFrequency(freq);
    depthModulator = depth;
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(previousMIDIInfo) + freqModulator;
    
    carrier.setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0);
}
