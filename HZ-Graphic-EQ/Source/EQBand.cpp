//  Created by Hanzhi Zhang on 11/1/23.
//

#include "EQBand.h"
#include <JuceHeader.h>
//#include <juce_IIRFilter.h>

EQBand::EQBand()
{
    addAndMakeVisible(bandSlider); //i. Add the slider to the parent component (EQBand).
    bandSlider.setSliderStyle(juce::Slider::LinearVertical); //ii. Set the slider to vertical by calling setSliderStyle().
    bandSlider.setRange(0.01f, 2.0f); //iii. Set the slider range to (0.01, 2) and the initial value to 1.
    bandSlider.setValue(1);
    bandSlider.setNumDecimalPlacesToDisplay(2);
    bandSlider.addListener(this); //iv. Add a listener to the slider.
    gain = 1; //v. Set member variable gain to a default value of 1.
    
    filter = std::vector<juce::IIRFilter>(1);
}

void EQBand::resized()
{
    bandSlider.setBounds(getLocalBounds()); //i. The slider should take up the entire size of this component, so use getLocalBounds() for this info.
}

void EQBand::initializeVTS(juce::AudioProcessorValueTreeState& vts)
{
//    gainAttachment.SliderAttachment(vts, gain, bandSlider);
//    gainAttachment.reset()
//    gainAttachment = std::make_unique<EQBand>(new EQBand);
    
    juce::String newid = juce::String(frequency) + "gain";
    
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, newid, bandSlider)); //i. Attach the gainSlider to the VTS using gainAttachment.
    
}

void EQBand::prepare(float frequency, int sampleRate, float gain)
{
    // i. Set the member variables: frequency, sampleRate, and gain.
    this->frequency = frequency;
    this->sampleRate = sampleRate;
    this->gain = gain;
    
    //auto iirfilter = filter.IIRFilterBase();

    //ii. Set up filter(s): Create IIRCoefficients for a Peak filter
    for (auto& i: filter)
        i.setCoefficients(juce::IIRCoefficients::makePeakFilter(sampleRate, frequency, 0.8, gain)); //iii. For each element in the filter vector, use setCoefficients()

}

void EQBand::process(juce::AudioBuffer<float>& block)
{
    //i. Check to make sure that the number of channels in the incoming AudioBuffer matches the number of elements in the filter vector.
    if (filter.size() != block.getNumChannels())
    {
        filter = std::vector<juce::IIRFilter>(block.getNumChannels());
        
        prepare(frequency, sampleRate, gain);
    }
    
    for (int i = 0; i < block.getNumChannels(); i++)
    {
        float *samples = block.getWritePointer(i);
        filter[i].processSamples(samples, block.getNumSamples());//ii. Process the audio input using the filter(s) and the processSamples() method of the filter.
    }
 
}

void EQBand::sliderValueChanged(juce::Slider *slider)
{
    gain = bandSlider.getValue();//i. Set the value of gain based on current value of the slider.
    prepare(frequency, sampleRate, gain);//ii. Call prepare() with rhw updated gain.
}

void EQBand::reset()
{
    for (auto &i: filter)
    {
        i.reset(); //i. Reset the state of each filter in the vector using its reset() method.
    }
}
