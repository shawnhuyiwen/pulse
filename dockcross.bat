:: Call me from a directory that contains both the pulse source directory and your build directory
:: You might need to add your wsl user to the sudo group
::   https://docs.microsoft.com/en-us/windows/wsl/wsl-config

echo off

if %1.==. goto USAGE_ERROR
if %2.==. goto USAGE_ERROR
if %3.==. goto USAGE_ERROR

set container=%1
echo Using container: %container%

set pulseDir=%2
if exist %pulseDir% (
  echo Using Pulse source code directory: %pulseDir%
) else (
  goto USAGE_ERROR
)

set rootBuildDir=%3
set buildDir= %rootBuildDir%\pulse-engine-%container%
if exist %rootBuildDir%\ (
  echo  Building pulse in: %buildDir%
  if exist %buildDir%\ (
    echo Deleting existing %buildDir% directory...
    rmdir /q/s %buildDir%
  )
  echo Creating build directory: %buildDir%
  mkdir %buildDir%
) else ( 
  goto USAGE_ERROR
)

set nixBuildDir=%buildDir:\=/%
set nixPulseDir=%pulseDir:\=/%

:: Run this command from a directory that contains both your sourc and your build directories
call wsl docker run --rm dockcross/%container% > ./%container%
:: This creates a file we will use to invoke our container
:: Next, set execution permissions
call wsl chmod +x ./%container%
:: Run CMake
:: -B is the relative path to my build directory
:: -H is the relative path to my Pulse source directory
call wsl ./%container% cmake -DPulse_JAVA_API:BOOL=OFF -B%nixBuildDir% -H%nixPulseDir% -GNinja
:: Use ninja to build (provided in the docker)
call wsl ./%container% ninja -C%nixBuildDir%
:: The PulseC.so for the target platform will be in the %buildDir%/install/bin directory

exit /b 0

:USAGE_ERROR

echo Usage: dockcross [pulse-source-directory] [build-directory] [container-name]
echo.
echo Example Usage: dockcross .\Pulse\engine .\builds linux-x64-clang
echo.
echo NOTE: You must use backslashes (\) in your argument paths
echo.
echo A new directory will be created under the provided build directory to build in
echo The directory name will be 'pulse-engine-' and the container name 
echo For example: pulse-engine-linux-x64-clang
echo.
echo Options:
echo   pulse-soure-directory The relative directory of the pulse source code
echo.
echo   build-directory       The relative directory to build pulse
echo.
echo   container-name       This is the name of the container to build in
echo                        https://hub.docker.com/u/dockcross
echo                        Only provide the string after 'dockcross/`
echo                        example : linux-x64-clang


exit /b 1