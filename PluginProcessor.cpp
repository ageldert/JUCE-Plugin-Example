/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ExampleProjectAgAudioProcessor::ExampleProjectAgAudioProcessor() :
    #ifndef JucePlugin_PreferredChannelConfigurations
         AudioProcessor (BusesProperties()
                         #if ! JucePlugin_IsMidiEffect
                          #if ! JucePlugin_IsSynth
                           .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          #endif
                           .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                         #endif
                           ),
    #endif
    // Aaron: set up the AVPTS here
    parameters (*this, nullptr, juce::Identifier("MyAPVTS"),
    {
        // Aaron: a parameter where the slider ranges from 0 to 100, default 50
        std::make_unique<juce::AudioParameterFloat>("paramID",
                                                    "Parameter Name",
                                                    juce::NormalisableRange<float> (0.0f, 100.0f),
                                                    50.0f)
    })
{
    // Aaron: this lets the processor run parameterChanged()
    parameters.addParameterListener("paramID", this);
    
    // Aaron: initialization
    myParameter = parameters.getRawParameterValue("paramID");
}

ExampleProjectAgAudioProcessor::~ExampleProjectAgAudioProcessor()
{
}

//==============================================================================
const juce::String ExampleProjectAgAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ExampleProjectAgAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ExampleProjectAgAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ExampleProjectAgAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ExampleProjectAgAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ExampleProjectAgAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ExampleProjectAgAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ExampleProjectAgAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ExampleProjectAgAudioProcessor::getProgramName (int index)
{
    return {};
}

void ExampleProjectAgAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ExampleProjectAgAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Aaron: put any initialization that requires the sample rate or buffer size here
}

void ExampleProjectAgAudioProcessor::parameterChanged (const juce::String &parameterID, float newValue)
{
    // Aaron: respond to param changes
    if (parameterID == "paramID")
    {
        *myParameter = newValue;
        // Aaron: may also be used to pass relevant params to any child objects
        // myAudioFilter.setFrequency(newValue);
    }
}

void ExampleProjectAgAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ExampleProjectAgAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void ExampleProjectAgAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    /* I prefer to just run a single loop over each sample, and handle all channels properly within that loop */
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Pointers used for accessing our input audio
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(0);
    if(totalNumInputChannels > 1)
        rightChannel = buffer.getWritePointer(1);
    
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        // sample level signal processing can go in here
        
        // set the output audio
        buffer.setSample(0, i, leftChannel[i]);
        if(totalNumOutputChannels > 1)
            buffer.setSample(1, i, rightChannel[i]);
    }
}

//==============================================================================
bool ExampleProjectAgAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ExampleProjectAgAudioProcessor::createEditor()
{
    // Aaron: pass the parameters to the new Editor constructor
    return new ExampleProjectAgAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void ExampleProjectAgAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Aaron: state example
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ExampleProjectAgAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // Aaron: state example
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ExampleProjectAgAudioProcessor();
}
