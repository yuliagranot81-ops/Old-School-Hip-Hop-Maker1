#include "PluginEditor.h"

OldSchoolHipHopMakerAudioProcessorEditor::OldSchoolHipHopMakerAudioProcessorEditor(OldSchoolHipHopMakerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(620, 440);
    addAndMakeVisible(loadButton);
    loadButton.onClick = [this]
    {
        chooser = std::make_unique<juce::FileChooser>("Choose an audio sample", juce::File{}, "*.wav;*.aiff;*.aif;*.flac;*.ogg");
        chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                const auto file = fc.getResult();
                if (file.existsAsFile() && processor.loadSample(file))
                    sampleLabel.setText(processor.getSampleName(), juce::dontSendNotification);
            });
    };

    addAndMakeVisible(sampleLabel);
    sampleLabel.setText(processor.getSampleName(), juce::dontSendNotification);
    sampleLabel.setJustificationType(juce::Justification::centredLeft);
    sampleLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    const std::array<juce::Colour, 4> palette {
        juce::Colour(0xffff5c35), juce::Colour(0xff00c7bd),
        juce::Colour(0xffffcc33), juce::Colour(0xffed4fa3)
    };
    for (int i = 0; i < 16; ++i)
    {
        pads[(size_t)i] = std::make_unique<juce::TextButton>("PAD " + juce::String(i + 1));
        auto* pad = pads[(size_t)i].get();
        addAndMakeVisible(pad);
        pad->setColour(juce::TextButton::buttonColourId, palette[(size_t)(i % 4)]);
        pad->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
        pad->onClick = [this, i] { processor.triggerPad(i); };
    }
}

void OldSchoolHipHopMakerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff17151d));
    g.setColour(juce::Colour(0xffff5c35));
    g.setFont(juce::Font(juce::FontOptions(27.0f, juce::Font::bold)));
    g.drawText("OLD SCHOOL", 22, 14, 280, 34, juce::Justification::left);
    g.setColour(juce::Colour(0xff00c7bd));
    g.drawText("HIP HOP MAKER", 22, 46, 360, 34, juce::Justification::left);
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(13.0f);
    g.drawText("LOAD A RECORDING • 16 SLICE PADS • MIDI NOTES 36–51", 22, 83, 570, 20, juce::Justification::left);
}

void OldSchoolHipHopMakerAudioProcessorEditor::resized()
{
    loadButton.setBounds(22, 116, 190, 34);
    sampleLabel.setBounds(225, 116, 370, 34);
    const int left = 22, top = 170, gap = 10, w = (getWidth() - 44 - gap * 3) / 4, h = 54;
    for (int i = 0; i < 16; ++i)
    {
        const int row = i / 4, col = i % 4;
        pads[(size_t)i]->setBounds(left + col * (w + gap), top + row * (h + gap), w, h);
    }
}
