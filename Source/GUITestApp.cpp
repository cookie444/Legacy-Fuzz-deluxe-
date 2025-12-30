#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
class GUITestWindow : public juce::DocumentWindow
{
public:
    GUITestWindow()
        : DocumentWindow("Legacy Fuzz GUI Test",
                        juce::Desktop::getInstance().getDefaultLookAndFeel()
                            .findColour(juce::ResizableWindow::backgroundColourId),
                        DocumentWindow::allButtons)
    {
        processor = std::make_unique<FuzzPedalAudioProcessor>();
        editor = std::make_unique<FuzzPedalAudioProcessorEditor>(*processor);
        
        setContentOwned(editor.release(), true);
        setResizable(true, true);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }

    void closeButtonPressed() override
    {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }

private:
    std::unique_ptr<FuzzPedalAudioProcessor> processor;
    std::unique_ptr<FuzzPedalAudioProcessorEditor> editor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUITestWindow)
};

//==============================================================================
class GUITestApplication : public juce::JUCEApplication
{
public:
    GUITestApplication() {}

    const juce::String getApplicationName() override { return "Legacy Fuzz GUI Test"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String& commandLine) override
    {
        window = std::make_unique<GUITestWindow>();
    }

    void shutdown() override
    {
        window = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

private:
    std::unique_ptr<GUITestWindow> window;
};

//==============================================================================
START_JUCE_APPLICATION(GUITestApplication)

