/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MiniProjectSPAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MiniProjectSPAudioProcessorEditor (MiniProjectSPAudioProcessor&);
    ~MiniProjectSPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider aController;
    juce::Slider dController;
    juce::Slider sController;
    juce::Slider rController;
    
    juce::Slider mAController;
    juce::Slider mDController;
    juce::Slider mSController;
    juce::Slider mRController;
    
    juce::Slider modFreqController;
    juce::Slider modDepthController;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> aConnector;
    std::unique_ptr<SliderAttachment> dConnector;
    std::unique_ptr<SliderAttachment> sConnector;
    std::unique_ptr<SliderAttachment> rConnector;
    
    std::unique_ptr<SliderAttachment> mAConnector;
    std::unique_ptr<SliderAttachment> mDConnector;
    std::unique_ptr<SliderAttachment> mSConnector;
    std::unique_ptr<SliderAttachment> mRConnector;
    
    std::unique_ptr<SliderAttachment> modFreqConnector;
    std::unique_ptr<SliderAttachment> modDepthConnector;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MiniProjectSPAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniProjectSPAudioProcessorEditor)
};
