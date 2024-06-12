/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MiniProjectSPAudioProcessorEditor::MiniProjectSPAudioProcessorEditor (MiniProjectSPAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 600);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    aConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "A", aController);
    dConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "D", dController);
    sConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "S", sController);
    rConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "R", rController);
    
    mAConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "mA", mAController);
    mDConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "mD", mDController);
    mSConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "mS", mSController);
    mRConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "mR", mRController);
    
    modFreqConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FREQFM", modFreqController);
    modDepthConnector = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DEPTHFM", modDepthController);
    
    aController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    aController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(aController);
    
    dController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(dController);
    
    sController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(sController);
    
    rController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(rController);
    
    mAController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(mAController);
    
    mDController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(mDController);
    
    mSController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(mSController);
    
    mRController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mRController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(mRController);
    
    modFreqController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    modFreqController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(modFreqController);
    
    modDepthController.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    modDepthController.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
    addAndMakeVisible(modDepthController);
    
}

MiniProjectSPAudioProcessorEditor::~MiniProjectSPAudioProcessorEditor()
{
}

//==============================================================================
void MiniProjectSPAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MiniProjectSPAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    aController.setBounds(20, 20, 200, 100);
    dController.setBounds(120, 20, 200, 100);
    sController.setBounds(220, 20, 200, 100);
    rController.setBounds(320, 20, 200, 100);
    
    modFreqController.setBounds(20, 250, 200, 100);
    modDepthController.setBounds(120, 250, 200, 100);
    
    mAController.setBounds(20, 400, 200, 100);
    mDController.setBounds(120, 400, 200, 100);
    mSController.setBounds(220, 400, 200, 100);
    mRController.setBounds(320, 400, 200, 100);
    
}
