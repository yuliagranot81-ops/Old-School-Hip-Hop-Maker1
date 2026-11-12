#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class OldSchoolHipHopMakerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit OldSchoolHipHopMakerAudioProcessorEditor(OldSchoolHipHopMakerAudioProcessor&);
    ~OldSchoolHipHopMakerAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    OldSchoolHipHopMakerAudioProcessor& processor;
    juce::TextButton loadButton { "LOAD VINYL / SAMPLE" };
    juce::Label sampleLabel;
    std::array<std::unique_ptr<juce::TextButton>, 16> pads;
    std::unique_ptr<juce::FileChooser> chooser;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OldSchoolHipHopMakerAudioProcessorEditor)
};
