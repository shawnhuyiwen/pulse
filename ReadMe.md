<img src="./docs/Images/pulse_logo_512.png" alt="Pulse Physiology Engine" width="256" height="106">

The Pulse Physiology engine is a C++ based simulation engine for human and animal physiology.
It is intended to provide accurate and consistent physiology data to medical education, research, and training technologies. 
The built libraries are static and can be integrated with standalone applications, hardware simulators, sensor interfaces, and other physiology models of all fidelities.

Along with the build instructions below, we provide more technical discussions using Pulse on our <a href="https://gitlab.kitware.com/physiology/engine/wikis/home">wiki</a>

For detailed information about our design and methodology read our <a href="https://physiology.kitware.com/">documentation</a>

Use this <a href="https://discourse.kitware.com/c/pulse-physiology-engine">discourse channel</a> 
to ask or share anything and everything about building, using, or understanding the Pulse engine and welcome to our community!!

## Build Environment

The code provided utilizes C++11. Here is a list of popular compilers and their initial version to implement all of C++11 :
- GCC 4.8.1 and later
- Clang 3.3 and later
- MSVC 2015 and later

If you have any questions\comments, don't hesitate to email kitware@kitware.com.

While the provided cmake superbuild automatically pulls many libraries it needs to compile, 
you will still need to have the following tools installed (along with your choice of C++ compiler) :

### CMake
Currently, the code requires CMake 3.12 or greater to properly build
Go to the cmake website, <a href="https://cmake.org/download">https://cmake.org/download</a>, and download the appropriate distribution.
Ensure that cmake bin is on your PATH and available in your cmd/bash shell.

#### On Linux

If you are on a Debian/Ubuntu system, please install the latest cmake on your system by <a href="https://apt.kitware.com/">following thes instructions</a>.

##### Building CMake

If you need to build CMake for the latest version, the <a href="https://cmake.org/download/">CMake Download page</a> provides source releases.  
The following should get you through the build process for CMake:

~~~bash
# Set up some dependent libraries you will need
$ sudo apt-get install zlib1g-dev
$ sudo apt-get install libcurl4-openssl-dev
# If you want to build the CMake GUI, you will need
$ sudo apt-get install libqt4-dev qt4-dev-tools libncurses5-dev
# Get the code link for the cmake download
# I used 'Copy Link Address' from my browser on the tar.gz source download link...
# Run the following commands from the directory you wish to build CMake
$ wget https://github.com/Kitware/CMake/releases/download/v3.17.3/cmake-3.17.3-Linux-x86_64.tar.gz
# Extract the source
$ tar -zxvf cmake-3.17.3-Linux-x86_64.tar.gz
$ cd cmake-3.17.3-Linux-x86_64.tar.gz
# Run the bootstrap, build and install the code
$ ./bootstrap --system-curl --qt-gui
# Remove the --qt-gui if you do not want to build the CMake GUI
$ make
$ sudo make install
~~~

### Java JDK

The test suite and data generate tools used are written in Java.
While there is no dependency on Java when integrating with your application, it is currently required to build/develop/contribute to the code base.

A JAVA_HOME environment variable needs to exist pointing to the Java installation.<br>
There are many ways to do this, here is a simple walk through to get you going with a JDK:

#### Windows
The JAVA_HOME environment variable pointing to the Java installation needs to be added to the system PATH.<br>

- Download the Windows x64 JDK <a href="https://docs.aws.amazon.com/corretto/latest/corretto-8-ug/downloads-list.html">here.</a>
- Run the installer.
- Goto your Control Panel->System and click Advanced system settings on the left. <br>
- Click the 'Environment Variables' button and add JAVA_HOME as a new 'System variables'.<br>
- Set the Value to something like: N:/Programming/Tools/jdk1.8.0_212<br>
    - It's a good idea to add the JDK to the system PATH by adding this to the beginning: %JAVA_HOME%/bin;
- Make sure to start a new cmd window.<br>

#### Linux

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
# Check to see if you have the JDK
$ update-alternatives --list java
# Note: Make sure you don't add the jre in the path of the java listed
# for example you may see '/usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java'
# you will want the truncated '/usr/lib/jvm/java-8-openjdk-amd64'
# If you do not have the Java SDK
$ sudo apt-get install openjdk-8-jdk
# Set the JAVA_HOME environment variable
# NOTE your Java path may vary!
# Use the update-alternatives to find your system's Java path
export JAVA_HOME='/usr/lib/jvm/java-8-openjdk-amd64'
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can also add it to your ~/.bash_profile, or related file (.bashrc, .zshrc, .cshrc, setenv.sh), to get the path in all shells.

## Building

The build is directed by CMake to ensure it can be built on various platforms. 
The code is build by a CMake 'superbuild', meaning as part of the build, CMake will download any
dependent libraries and compile those before it builds. 
The build is also out of source, meaning the code base is seperated from the build files.
This means you will need two folders, one for the source code and one for the build files.
Generally, I create a single directory to house these two folders.

Visual Studio users, our <a href="https://gitlab.kitware.com/physiology/engine/wikis/Using%20MSVC">wiki provides detailed steps for building with MSVC</a>

Here is the quickest way to pull and build via a bash shell:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
$ mkdir Pulse
$ cd Pulse
$ git clone https://gitlab.kitware.com/physiology/engine
# This will put the source in an 'engine' folder
$ mkdir builds
$ cd builds
# Feel free to make subfolders here, like debug or release
# Generate a make file solution for the external dependencies
# Note you need to provide cmake the source directory at the end (relative or absolute)
# Run CMake (it will use the system default compiler if you don't provide options or use the CMake GUI)
$ cmake -DCMAKE_BUILD_TYPE:STRING=Release ../engine
# If you want the build to pull the Verification and Validation (V&V) scenarios and baselines run this (or check the Pulse_DOWNLOAD_BASELINES option in the CMake GUI)
# You can always pull these later if you want (See Running and Testing)
$ cmake -DCMAKE_BUILD_TYPE:STRING=Release -DPulse_DOWNLOAD_BASELINES:BOOL=ON ../engine
#
# Build the install target/project
# On Linux/OSX/MinGW 
$ make -j4
# cd into the 'Pulse' directory for building any changes specific to the Pulse code base
# Such as if you pull new Pulse code or make changes, run make from the Pulse directory
# See below for more details in pulling and rebuilding
$ cd Pulse
# To run Pulse, cd to the install/bin directory
$ cd install/bin
# In this example that would be ~/Pulse/builds/install/bin
# Now you can run something
$ ./PulseScenarioDriver VitalsMonitor.json
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Docker Support

Pulse provides a docker file that, by default, builds a linux based docker container with Python support.

To build this docker container, pull the source code and run the docker build command from the source root.
Kitware provides Pulse containers on [dockerhub](https://hub.docker.com/r/kitware/pulse).

Here is an example of building a docker container and pushing it to an organization repository in dockerhub:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
# From the Pulse source directory
$ docker build -t kitware/pulse:3.0.0 .
$ docker push kitware/pulse:3.0.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Single Board Computers Support 

Note that on small boards, like the Beagle Board Black and DragonBoard 410c, you may need to allocate a <a href="https://www.cyberciti.biz/faq/linux-add-a-swap-file-howto/">temporary swap</a> file if your build runs out of memory

It boils down to:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
su - root
fallocate -l 1G tmpswap
mkswap tmpswap
swapon tmpswap
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Read the article if you want to make this change permanent, it contains some valuable hints regarding permissions and fstab.
### Cross Compiling

CMake provides the ability to easily cross compile.
In general, when we cross compile, we are doing so to get out a PulseC library.
The PulseC library provides an interface used by our C#/Unity and Python API's to access and control the C++ based engine.
Unity is C# based and any C# managed libraries built on any platform, will automatically run on other platforms.
But the PulseC library is a native library and a specific platform version must be provided for the platform Pulse is being run on.
The following sections will highlight specific build instructions for cross compiling for these target platforms.

#### Native Platform Build

The first thing you will want to do is build Pulse on the host system. I use Visual Studio when building natively.
During the Pulse build, it will build and run the Protobuf protoc compiler to generate C++ classes needed for I/O.
These files are needed to build, but when you cross compile, you will build a protoc executable for your target platform, and it will not run on the native host system.
Doing a native build will ensure these files are generated. The cross compile build will see the files exist and not run the non native protoc compiler.
Once you have a native build, create a new empty build directory for your cross compile build.

#### Dockcross

To generate Pulse libraries for android, linux, and web assembly, we use docker containers provided by [dockcross](https://hub.docker.com/u/dockcross). Dockcross provides both a build environment and a cmake toolchain file that simplifies the cross platform build process. 
Simply pull the target docker container, run cmake, and build.
I am using docker on windows via the WSL 2 based engine, from an [Ubuntu 20](https://www.microsoft.com/en-us/p/ubuntu-2004-lts/9n6svws3rx71?activetab=pivot:overviewtab) terminal.

Here is an example bulding Pulse for Android armv8a.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
# Run this command from a directory that contains both your sourc and your build directories
$ docker run --rm dockcross/android-arm64 > ./armv8a
# This creates a file we will use to invoke our container
# Next, set execution permissions
$ chmod +x ./armv8a  
# Run CMake
# -B is the relative path to my build directory
# -H is the relative path to my Pulse source directory
$ ./armv8a cmake -DPulse_JAVA_API:BOOL=OFF -B./Builds/pulse-engine-armv8a -H./Pulse/engine -GNinja
# Use ninja to build (provided in the docker)
$ ./armv8a ninja -C./Builds/pulse-engine-armv8a
# The PulseC.so for the target platform will be in the pulse-engine-armv8a/install/bin directory
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Magic Leap

Magic Leap provides a Lumin SDK for use in compiling native C++ code for use on the Magic Leap platform.
You will need to download and install this SDK. 
In this example we are building on a windows machine, using the Lumin SDK v0.24.1.
Please follow the [Lumin instructions on generating a cmake toolchain file](https://developer.magicleap.com/en-us/learn/guides/sdk-mabu-ref-build-and-link-libraries#build-libraries-with-cmake) needed for cross compiling.

I had to modify the toolchain file, adding '-llog' for protobuf's use of the android log methods, as well as '-g -std=c++11 -Wall -pedantic' to add C++11 support.
Here is a copy of the [modified toolchain file](https://data.kitware.com/api/v1/file/5f29b9489014a6d84e57067a/download)

To generate a Makefile and build, run :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
# Where -H specifies the Pulse source, and -B is the build directory
cmake -DPulse_JAVA_API:BOOL=OFF -DCMAKE_TOOLCHAIN_FILE:FILEPATH=C:/Programming/magicleap.0.24.1.toolchain.cmake -BC:/Programming/Builds/pulse-engine-lumin -HC:/Programming/Pulse/engine -G "Unix Makefiles"
# Run make from within your build directory
N:/Tools/MagicLeap/dev/mlsdk/v0.24.1/tools/mabu/tools/MinGW/bin/mingw32-make.exe
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will provide a PulseC.so in the build/install/bin that works on the Magic Leap platform!


## Updating Pulse

The Pulse repository is always changing as we add improvements and features to the code base.
These changes will require that you rebuild the source code when you pull the latest changes.
Here are a few tips for keeping your source code up to date when you pull:

<b>Please ensure CMake is on your path!</b>

### Rebuilding the schema

Pulse uses Google Protocol Buffers for data files and to communicate data between various languages and networking protocols.
After you pull the latest, you will need to run the Protoc compiler to adjust the data model to any changes made to the proto files describing the Pulse Data Model.

From a command/terminal in your pulse/build/install/bin directory : 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
# On Windows
> run protoc
# On Linux/Mac
$ ./run.sh protoc
# Note you may need to do a 
$ chmod +x run.sh
# To run this script on your machine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note: If there were no changes to the protobuf files, the proto compile will not run and return this message 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
-- Not generating bindings, nothing has changed since last build
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you are getting linking errors about missing protobuf functions,
such as missing something in the cdm:: namespace,
you can force the protoc compiler to run by deleting this timestamp file:

<b>pulse/build/Pulse/schema/schema_last_built</b>

### Rebuild the code base

With the Protocol Buffer bindings updated, you can now build the updated code base.

### Regenerate State Data

Any changes to the code base can change various aspects of the data our engine calculates.
This in-turn has can have an effect on the various state files used in our HowTo examples
(located in the pulse/install/bin/states directory).
If the state files no longer load, or you want to ensure they are up to date with your version of the engine code, regenerate these state files:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
# On Windows
> run genStates
# On Linux/Mac
$ ./run.sh genStates
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Update Baseline Data

Changes to the code base can also change the Test Suite baseline files.
You will need these new baseline files to pass any tests and ensure any changes do not affect the code base in any negative ways.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~bash
# On Windows
> run updateBaselines
# On Linux/Mac
$ ./run.sh updateBaselines
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Using Pulse

With the code built, visit our [wiki](https://gitlab.kitware.com/physiology/engine/wikis/home) to learn how to execute Pulse and use our SDK to use Pulse in your own application with Pulse.

