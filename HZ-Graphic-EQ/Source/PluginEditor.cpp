/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HZ_Graphic_EQAudioProcessorEditor::HZ_Graphic_EQAudioProcessorEditor (HZ_Graphic_EQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    setResizable(true, true);
    
    //this is the title label of the GUI
    addAndMakeVisible(Title);
    Title.setText("Band Center Frequencies", juce::dontSendNotification);
    Title.setJustificationType(juce::Justification::centred);
    
    //this is the label indicating the gain text box
    addAndMakeVisible(bandgaindb);
    bandgaindb.setText("Band Gain, dB", juce::dontSendNotification);
    bandgaindb.setJustificationType(juce::Justification::centred);
    
    int center_freq[10] = {32, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    
    //3. add EQBands to the PluginEditor
    for (int i = 0; i < 10; i++)
    {
        addAndMakeVisible(audioProcessor.eqBands[i].bandSlider);
        addAndMakeVisible(eq_name[i]);
        eq_name[i].setText(juce::String(center_freq[i])+"Hz", juce::dontSendNotification);
        eq_name[i].setJustificationType(juce::Justification::centred);
    }
}

HZ_Graphic_EQAudioProcessorEditor::~HZ_Graphic_EQAudioProcessorEditor()
{
}

//==============================================================================
void HZ_Graphic_EQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void HZ_Graphic_EQAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // set the size for all the things that should be displayed in GUI
    
    int spacing = getRight()/10;
    
    Title.setBounds(0, 10, getWidth()-20, 20);
    
    for (int i = 0; i < 10; i++)
    {
        audioProcessor.eqBands[i].bandSlider.setBounds(2+i*spacing, 50, 50, getHeight()-120);
        audioProcessor.eqBands[i].bandSlider.setTextBoxStyle(audioProcessor.eqBands[i].bandSlider.TextBoxBelow, true, 30, 20);
        eq_name[i].setBounds(2+i*spacing, 30, 50, 20);
    }
    
    bandgaindb.setBounds(0, audioProcessor.eqBands[0].bandSlider.getY()+audioProcessor.eqBands[0].bandSlider.getHeight()+5, getWidth()-20, 20);
}
