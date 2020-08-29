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
class ExampleProjectAgAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    // Aaron: constructor will now have APVTS as argument
    ExampleProjectAgAudioProcessorEditor (juce::AudioProcessor& parent, juce::AudioProcessorValueTreeState& vts);
    ~ExampleProjectAgAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Aaron: I only need the APVTS reference, not the processor reference
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    // Aaron: example with one slider, and a pointer for its attachment
    juce::Slider mySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mySliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExampleProjectAgAudioProcessorEditor)
};
