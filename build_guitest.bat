@echo off
echo Building GUI Test Application...
set PATH=%PATH%;C:\Program Files\CMake\bin
mkdir build_guitest 2>nul
cd build_guitest
cmake -f ..\CMakeLists_GUITest.txt -S .. -B .
cmake --build . --config Release
echo.
echo Build complete! Run the executable from: build_guitest\Release\GUITest.exe
echo.
echo OR use the Standalone build that's already available:
echo build\FuzzPedal_artefacts\Release\Standalone\Legacy Fuzz (Deluxe).exe
pause

