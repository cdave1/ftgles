#!/bin/sh

xcodebuild -configuration Release -sdk iphoneos7.1 -alltargets clean
xcodebuild -configuration Release -sdk iphoneos7.1 -alltargets build
cp build/Release-iphoneos/libFTGLES.a ./libFTGLES-arm.a

xcodebuild -configuration Release -sdk iphonesimulator7.1 -alltargets clean
xcodebuild -configuration Release -sdk iphonesimulator7.1 -alltargets build
cp build/Release-iphonesimulator/libFTGLES.a ./libFTGLES-sim.a

lipo -output libFTGLES.a -create libFTGLES-arm.a libFTGLES-sim.a
