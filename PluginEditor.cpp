/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ExampleProjectAgAudioProcessorEditor::ExampleProjectAgAudioProcessorEditor (juce::AudioProcessor& parent, juce::AudioProcessorValueTreeState& vts)
: juce::AudioProcessorEditor (parent),
  valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 150);
    
    // Aaron: Example of setting up a slider to display
    mySlider.setBounds(10, 20, 100, 100);
    mySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 12);
    mySlider.setTextValueSuffix(" units");
    addAndMakeVisible(mySlider);
    
    // Aaron: attach the slider to the paramID, you made in the APVTS
    mySliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "paramID", mySlider));
}

ExampleProjectAgAudioProcessorEditor::~ExampleProjectAgAudioProcessorEditor()
{
    // Aaron: ensure the attachments die when the GUI is closed
    mySliderAttachment = nullptr;
}

//==============================================================================
void ExampleProjectAgAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    /*
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
     */
}

void ExampleProjectAgAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
