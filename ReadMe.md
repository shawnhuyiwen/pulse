<img src="./docs/Images/pulse_logo_512.png" alt="Pulse Physiology Engine" width="256" height="106">

The Pulse Physiology engine is a C++ based simulation engine for human and animal physiology.
It is intended to provide accurate and consistent physiology data to medical education, research, and training technologies. 
The libraries built (*.dll/*.so) can be integrated with standalone applications, hardware simulators and sensor interfaces, and other physiology models of all fidelities.

All files are released under the Apache 2.0 license

Along with the build instrcutions below, we provide more technical discussions using Pulse on our <a href="https://gitlab.kitware.com/physiology/engine/wikis/home">wiki</a>

For detailed information about our design and methodology read our <a href="https://physiology.kitware.com/">documentation</a>

Use this <a href="https://discourse.kitware.com/c/pulse-physiology-engine">discourse channel</a> 
to ask or share anything and everything about building, using, or understanding the Pulse engine and welcome to our community!!

## Build Environment

The code provided utilizes C++11, here is a list of popular compilers and their initial version to implement all of C++11 :
- GCC 4.8.1 and later
- Clang 3.3 and later
- MSVC 2015 and later

If you have any questions\comments, don't hesitate to email kitware@kitware.com.

While the provided cmake superbuild automatically pulls many libraries it needs to compile, 
you will still need to have the following tools installed (along with your choice of C++ compiler) :

### CMake
Currently, the code requires CMake 3.7 or greater to properly build
Go to the cmake website, `https://cmake.org/download`, and download the appropriate distribution.
Ensure that cmake bin is on your PATH and available in your cmd/bash shell.

Note on Linux systems, make sure to have curl before you build/install cmake for it to behave properly when downloading 3rd party libraries.
~~~~~~~~~~~~~~~~~~~~~~~~
sudo apt-get install curl
~~~~~~~~~~~~~~~~~~~~~~~~

### Java JDK

The test suite and data generate tools used are written in Java.
While there is no dependency on Java when integrating with your application, it is currently required to build/develop/contribute to the code base.

Add a JAVA_HOME environment variable to point to the Java installation and add it to the system PATH.<br>
There are many ways to do this, here is a simple walk through to get you going with a JDK.

#### Windows
- Download the Windows x64 JDK <a href="http://www.oracle.com/technetwork/java/javase/downloads/index.html">here.</a>
- Run the installer.
- Goto your Control Panel->System and click Advanced system settings on the left. <br>
- Click the 'Environment Variables' button and add JAVA_HOME as a new 'System variables'.<br>
- Set the Value to something like: C:\Program Files\Java\jdk1.8.0_121<br>
    - It's a good idea to add the JDK to the system PATH by adding this to the beginning: %JAVA_HOME%/bin;
- Make sure to start a new cmd window.<br>

#### Linux
- You can find where java is by running `update-alternatives --list java <br>
 - If you don't have a Java SDK, I recommend using an installer like Synaptic
 - Search for 'jdk' by name and install the 'openjdk-8-jdk' 
- You can then add the JAVA_HOME variable to a bash shell by typing
    - export JAVA_HOME=(a path listed by a call to updata-alternatives --list java)
    - For example : export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
- You can also add it to your ~/.bash_profile, or related file (.bashrc, .zshrc, .cshrc, setenv.sh), to get the path in all shells

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
JAVA_HOME='/usr/lib/jvm/java-8-openjdk-amd64'
export JAVA_HOME
PATH="$JAVA_HOME/bin:$PATH"
export PATH
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Building

The build is directed by CMake to ensure it can be built on various platforms. 
The code is build by a CMake 'superbuild', meaning as part of the build, CMake will download any
dependent libraries and compile those before it builds. 
The build is also out of source, meaning the code base is seperated from the build files.
This means you will need two folders, one for the source code and one for the build files.
Generally, I create a single directory to house these two folders.
Here is the quickest way to pull and build via a cmd/bash shell:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
mkdir physiology
cd physiology
git clone https://gitlab.kitware.com/physiology/engine src
mkdir builds
cd builds
# Feel free to make subfolders here, like msvc2017x64 or something
# Generate a make file/msvc solution for the external dependencies
# Note you need to provide cmake the source directory at the end (relative or absolute)
# Run CMake (it will use the system default compiler if you don't provide options or use the CMake GUI)
cmake -DCMAKE_BUILD_TYPE:STRING=Release ../src
# If you want the build to pull the V&V scenarios and baselines run this (or check the PULSE_DOWNLOAD_BASELINES option in the CMake GUI)
# You can always pull these later if you want (See Running and Testing)
cmake -DCMAKE_BUILD_TYPE:STRING=Release -DPULSE_DOWNLOAD_BASELINES:BOOL=ON ../src
# Build the install target/project
# On Linux/OSX/MinGW 
make install 
# For MSVC
# Open the OuterBuild.sln and build the INSTALL project (It will build everything!)
# When the build is complete, MSVC users can close the OuterBuild solution, and open the Pulse.sln located in the Pulse directory.
# Unix based systems should also cd into this directory for building any changes to the Pulse code base
cd Pulse
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Using Pulse

With the code built, visit our [wiki](https://gitlab.kitware.com/physiology/engine/wikis/home) to learn how to execute Pulase and use our SDK to use Pulse in your own application with Pulse.

