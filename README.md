# Fuzz Pedal VST Plugin

A professional fuzz pedal VST plugin built with JUCE, featuring a modern GUI with three control knobs.

## Features

- **Mix Knob**: Controls the blend between dry and fuzzed signal (0-100%)
- **Compression Knob**: Adjusts dynamic range compression (0-100%)
- **Fuzz Character Knob**: Changes the fuzz character from soft clipping to hard clipping (0-100%)

## Requirements

- **JUCE Framework**: Version 7.0 or later
- **CMake**: Version 3.22 or later
- **C++ Compiler**: C++17 compatible (MSVC, GCC, or Clang)
- **Build System**: CMake

## Building the Plugin

### 1. Clone JUCE

First, you need to add JUCE as a submodule or download it:

```bash
git submodule add https://github.com/juce-framework/JUCE.git JUCE
```

Or if you prefer to download manually:
- Download JUCE from https://github.com/juce-framework/JUCE
- Extract it to a `JUCE` folder in the project root

### 2. Configure with CMake

```bash
mkdir build
cd build
cmake ..
```

### 3. Build

**Windows (Visual Studio):**
```bash
cmake --build . --config Release
```

**macOS/Linux:**
```bash
cmake --build . --config Release
```

### 4. Install the Plugin

The built VST3 plugin will be located in:
- **Windows**: `build/FuzzPedal_artefacts/Release/VST3/FuzzPedal.vst3`
- **macOS**: `build/FuzzPedal_artefacts/Release/VST3/FuzzPedal.vst3`
- **Linux**: `build/FuzzPedal_artefacts/Release/VST3/FuzzPedal.vst3`

Copy the `.vst3` file to your VST3 plugin directory:
- **Windows**: `C:\Program Files\Common Files\VST3\`
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/`
- **Linux**: `~/.vst3/` or `/usr/local/lib/vst3/`

## Usage

1. Load the plugin in your DAW (Digital Audio Workstation)
2. Adjust the three knobs:
   - **Mix**: Blend between original and fuzzed signal
   - **Compression**: Control dynamic range (higher = more compressed)
   - **Fuzz Character**: 
     - 0-33%: Soft clipping (smooth, warm fuzz)
     - 33-66%: Asymmetric clipping (classic fuzz tone)
     - 66-100%: Hard clipping (aggressive, distorted)

## Technical Details

- **Plugin Format**: VST3
- **Sample Rate**: Supports all standard sample rates
- **Channels**: Stereo (mono compatible)
- **Latency**: Zero latency

## License

This project is provided as-is for educational and personal use.

