/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
using Parameter = juce::AudioParameterFloat;
// build a parameter layout to handle all the APVTS in the constructor
juce::AudioProcessorValueTreeState::ParameterLayout valuetreelist()
{
    int center_freq[10] = {32, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    juce::String name, id;
    
    juce::AudioProcessorValueTreeState::ParameterLayout parlist;
    for (int i = 0; i < 10; i++)
    {
        name = juce::String(center_freq[i]) + "Gain";
        id = juce::String(center_freq[i]) + "gain";
        parlist.add(std::make_unique<Parameter>(id,name,juce::NormalisableRange<float>(0.01f, 2.0f),1.0f));
    }
    
    return parlist;
}

HZ_Graphic_EQAudioProcessor::HZ_Graphic_EQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr , "PARAMETERS"
                                      , valuetreelist()
                                     /*{
                           std::make_unique<Parameter>("32gain", "32Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("63gain", "63Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("125gain", "125Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("250gain", "250Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("500gain", "500Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("1000gain", "1000Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("2000gain", "2000Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("4000gain", "4000Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("8000gain", "8000Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                           std::make_unique<Parameter>("16000gain", "16000Gain", 0.01f, 2.0f, 1.0f, nullptr, nullptr),
                       }*/)
#endif
{
    int center_freq[10] = {32, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    
    juce::String name, id;
    
    for (int i = 0; i < 10; i++)
    {
        name = juce::String(center_freq[i]) + "Gain";
        id = juce::String(center_freq[i]) + "gain";
        
//        parameters.createAndAddParameter(
//                        std::make_unique<Parameter>(id, name, "", juce::NormalisableRange<float>(0.1f, 2.0f), 1.0f, nullptr, nullptr));
        
        //6. initialize valuetree
        eqBands[i].initializeVTS(parameters);
    }
}

HZ_Graphic_EQAudioProcessor::~HZ_Graphic_EQAudioProcessor()
{
    for (int i = 0; i < 10; i++)
    {
        eqBands[i].reset();
        eqBands[i].gainAttachment.reset();
        eqBands[i].gainAttachment.release();
    }
}

//==============================================================================
const juce::String HZ_Graphic_EQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HZ_Graphic_EQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HZ_Graphic_EQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HZ_Graphic_EQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HZ_Graphic_EQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HZ_Graphic_EQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HZ_Graphic_EQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HZ_Graphic_EQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HZ_Graphic_EQAudioProcessor::getProgramName (int index)
{
    return {"HZ_GraphicEQ"};
}

void HZ_Graphic_EQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HZ_Graphic_EQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //5. initialize each EQBand
    float center_freq[10] = {32, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    
    for (int i = 0; i < 10; i++)
    {
        eqBands[i].prepare(center_freq[i], (int)sampleRate, 1.0f);
    }
}

void HZ_Graphic_EQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HZ_Graphic_EQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HZ_Graphic_EQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    
    //7. pass the AudioBuffer directly to filter[num].process()
    for (int i = 0; i < 10; i++)
    {
        eqBands[i].process(buffer);
    }
}

//==============================================================================
bool HZ_Graphic_EQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HZ_Graphic_EQAudioProcessor::createEditor()
{
    return new HZ_Graphic_EQAudioProcessorEditor (*this);
}

//==============================================================================
void HZ_Graphic_EQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    //8. Getting and setting plugin states in the plugin processor involves taking the values of the ValueTree.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void HZ_Graphic_EQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    //8. Getting and setting plugin states in the plugin processor involves taking the values of the ValueTree.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HZ_Graphic_EQAudioProcessor();
}
