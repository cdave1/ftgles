#!/bin/sh

xcodebuild -configuration Release -sdk iphoneos4.3 -alltargets clean
xcodebuild -configuration Release -sdk iphoneos4.3 -alltargets build
cp build/Release-iphoneos/libFreetype2.a ./libFreetype2-arm.a

xcodebuild -configuration Release -sdk iphonesimulator4.3 -alltargets clean
xcodebuild -configuration Release -sdk iphonesimulator4.3 -alltargets build
cp build/Release-iphonesimulator/libFreetype2.a ./libFreetype2-sim.a

lipo -output libFreetype2.a -create libFreetype2-arm.a -arch i386 libFreetype2-sim.a
