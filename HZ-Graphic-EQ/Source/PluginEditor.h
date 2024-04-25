/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EQBand.h"

//==============================================================================
/**
*/
class HZ_Graphic_EQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HZ_Graphic_EQAudioProcessorEditor (HZ_Graphic_EQAudioProcessor&);
    ~HZ_Graphic_EQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HZ_Graphic_EQAudioProcessor& audioProcessor;
    juce::Label Title, eq_name[10], bandgaindb; //text labels

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HZ_Graphic_EQAudioProcessorEditor)
};
