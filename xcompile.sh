#!/bin/bash

# This file builds Pulse for many of the systems needed for our Pulse Unity Asset
# Run this script from your Pulse source directory
# It will create a new directory next to your source directory called 'dockcross-builds'
# All builds will be placed here, and the binaries you need will be in each install directory
# Note that for Android builds, you need to already have a completed build
# The android protoc compiler cannot run on our linux based docker host
# So we need to get those autogenerated files from a different build


cwd=$PWD
srcDir=${PWD##*/}
rootBldDir="dockcross-builds"

cd ..
if [ ! -d $rootBldDir ]
then
  mkdir "$rootBldDir"
fi

$cwd/dockcross.sh manylinux2014-x64 ./$srcDir ./$rootBldDir
$cwd/dockcross.sh android-arm ./$srcDir ./$rootBldDir ./$rootBldDir/pulse-engine-manylinux2014-x64
./android-arm cp /usr/arm-linux-androideabi/sysroot/usr/lib/arm-linux-androideabi/libc++_shared.so /work/$rootBldDir/pulse-engine-android-arm/install/bin
$cwd/dockcross.sh android-arm64 ./$srcDir ./$rootBldDir ./$rootBldDir/pulse-engine-manylinux2014-x64
./android-arm64 cp /usr/aarch64-linux-android/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so /work/$rootBldDir/pulse-engine-android-arm64/install/bin
$cwd/dockcross.sh windows-arm64 ./$srcDir ./$rootBldDir ./$rootBldDir/pulse-engine-manylinux2014-x64
$cwd/dockcross.sh windows-static-x64 ./$srcDir ./$rootBldDir ./$rootBldDir/pulse-engine-manylinux2014-x64
$cwd/dockcross.sh windows-static-x86 ./$srcDir ./$rootBldDir ./$rootBldDir/pulse-engine-manylinux2014-x64
#$cwd/dockcross.sh linux-x64-clang ./$srcDir ./$rootBldDir

cd $cwd