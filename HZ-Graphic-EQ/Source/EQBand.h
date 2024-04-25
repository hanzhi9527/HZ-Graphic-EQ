//  Created by Hanzhi Zhang on 11/1/23.
//

#pragma once
#include <JuceHeader.h>


class EQBand : public juce::Component, public juce::Slider::Listener
{
public:
    EQBand();
    void resized() override;
    void initializeVTS(juce::AudioProcessorValueTreeState& vts);
    void prepare(float frequency, int sampleRate, float gain);
    void process(juce::AudioBuffer<float>& block);
    void sliderValueChanged(juce::Slider *slider) override;
    void reset();
    
    juce::Slider bandSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
    gainAttachment;
    
private:
    
    std::vector<juce::IIRFilter> filter;
    
    int numChannels, sampleRate;
    float frequency, gain;
    
    juce::Label bandLabel;
};


