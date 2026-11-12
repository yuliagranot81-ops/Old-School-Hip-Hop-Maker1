#include "PluginProcessor.h"
#include "PluginEditor.h"

OldSchoolHipHopMakerAudioProcessor::OldSchoolHipHopMakerAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    formatManager.registerBasicFormats();
}

void OldSchoolHipHopMakerAudioProcessor::prepareToPlay(double, int) {}
void OldSchoolHipHopMakerAudioProcessor::releaseResources() {}

bool OldSchoolHipHopMakerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto out = layouts.getMainOutputChannelSet();
    return out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo();
}

bool OldSchoolHipHopMakerAudioProcessor::loadSample(const juce::File& file)
{
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader == nullptr) return false;

    const juce::ScopedLock lock(sampleLock);
    const int length = (int) reader->lengthInSamples;
    if (length <= 0) return false;

    sample.setSize((int) juce::jmin((unsigned int) 2, reader->numChannels), length);
    reader->read(&sample, 0, length, 0, true, true);
    sourceSampleRate = reader->sampleRate;
    sampleName = file.getFileName();
    activePad = -1;
    return true;
}

juce::String OldSchoolHipHopMakerAudioProcessor::getSampleName() const
{
    const juce::ScopedLock lock(sampleLock);
    return sampleName;
}

void OldSchoolHipHopMakerAudioProcessor::triggerPad(int pad)
{
    const juce::ScopedLock lock(sampleLock);
    if (sample.getNumSamples() == 0 || pad < 0 || pad >= 16) return;
    const int total = sample.getNumSamples();
    const int start = (total * pad) / 16;
    const int end = (total * (pad + 1)) / 16;
    if (end <= start) return;
    activePad = pad;
    playPosition = start;
    playEnd = end;
}

void OldSchoolHipHopMakerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            const int note = msg.getNoteNumber();
            if (note >= 36 && note <= 51) triggerPad(note - 36);
        }
    }

    const juce::ScopedLock lock(sampleLock);
    if (activePad < 0 || sample.getNumSamples() == 0) return;

    const int outChannels = buffer.getNumChannels();
    const int srcChannels = sample.getNumChannels();
    for (int i = 0; i < buffer.getNumSamples() && playPosition < playEnd; ++i, ++playPosition)
    {
        for (int ch = 0; ch < outChannels; ++ch)
        {
            const int src = juce::jmin(ch, srcChannels - 1);
            buffer.addSample(ch, i, sample.getSample(src, playPosition) * gain);
        }
    }
    if (playPosition >= playEnd) activePad = -1;
}

juce::AudioProcessorEditor* OldSchoolHipHopMakerAudioProcessor::createEditor()
{
    return new OldSchoolHipHopMakerAudioProcessorEditor(*this);
}

void OldSchoolHipHopMakerAudioProcessor::getStateInformation(juce::MemoryBlock&) {}
void OldSchoolHipHopMakerAudioProcessor::setStateInformation(const void*, int) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OldSchoolHipHopMakerAudioProcessor();
}
