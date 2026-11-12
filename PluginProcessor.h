#pragma once
#include <JuceHeader.h>

class OldSchoolHipHopMakerAudioProcessor : public juce::AudioProcessor
{
public:
    OldSchoolHipHopMakerAudioProcessor();
    ~OldSchoolHipHopMakerAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    bool loadSample(const juce::File& file);
    juce::String getSampleName() const;
    void triggerPad(int pad);

private:
    juce::AudioFormatManager formatManager;
    juce::AudioBuffer<float> sample;
    juce::String sampleName = "No sample loaded";
    juce::CriticalSection sampleLock;
    double sourceSampleRate = 44100.0;
    int activePad = -1;
    int playPosition = 0;
    int playEnd = 0;
    float gain = 0.85f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OldSchoolHipHopMakerAudioProcessor)
};
