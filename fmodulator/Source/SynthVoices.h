/*
  ==============================================================================

    SynthVoices.h
    Created: 6 Feb 2023 6:32:08pm
    Author:  Simon Andersen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoices : public juce::SynthesiserVoice {
public:
    bool canPlaySound(juce::SynthesiserSound * sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
    void updateADSRModParams(const float attack, const float decay, const float sustain, const float release);
    void setModulationParams(const float depth, const float freq);
    
private:
    juce::ADSR adsr;
    juce::ADSR modEnvelope;
    
    juce::ADSR::Parameters adsrParams;
    juce::ADSR::Parameters modulatorADSRParams;
    
    juce::dsp::Oscillator<float> carrier { [](float x) { return std::sin(x); }};
    juce::dsp::Oscillator<float> modulator { [](float x) {return std::sin(x); }};
    
    juce::AudioBuffer<float> buffer;
    
    float freqModulator {0.0f};
    float depthModulator {0.0f};
    int previousMIDIInfo {0};
    juce::dsp::Gain<float> gain;
    bool isPrepared { false };
};
