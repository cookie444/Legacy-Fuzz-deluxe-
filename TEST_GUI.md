# Testing the GUI Without a DAW

You have two options to test the GUI:

## Option 1: Use the Standalone Build (Easiest)

The plugin already builds a standalone executable that you can run directly:

**Windows:**
```
build\FuzzPedal_artefacts\Release\Standalone\Legacy Fuzz (Deluxe).exe
```

Just double-click this file to run it and test the GUI without loading it into a DAW!

## Option 2: Build the GUI Test App

If you want a dedicated test application, you can build it using:

**Windows:**
```bash
build_guitest.bat
```

Then run:
```
build_guitest\Release\GUITest.exe
```

## Building the Standalone

The standalone is automatically built when you build the plugin:

```bash
cd build
cmake ..
cmake --build . --config Release
```

The standalone executable will be in:
- `build/FuzzPedal_artefacts/Release/Standalone/Legacy Fuzz (Deluxe).exe`

