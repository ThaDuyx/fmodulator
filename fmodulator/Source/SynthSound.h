/*
  ==============================================================================

    SynthSound.h
    Created: 6 Feb 2023 6:32:36pm
    Author:  Simon Andersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};
