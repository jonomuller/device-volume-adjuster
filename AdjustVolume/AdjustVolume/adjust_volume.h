//
//  adjust_volume.h
//  AdjustVolume
//
//  Created by Jono Muller on 29/12/2017.
//  Copyright © 2017 Jonathan Muller. All rights reserved.
//

#ifndef adjust_volume_h
#define adjust_volume_h

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <CoreAudio/CoreAudio.h>
#include <AudioToolbox/AudioToolbox.h>

#endif /* adjust_volume_h */

typedef enum {
  kIncrementVolume = 1,
  kDecrementVolume = 2,
  kSetVolume = 3,
  kMuteVolume = 4,
  kGetVolume = 5,
  kCopyVolume = 6,
} FunctionType;

int startVolumeAdjuster(int argc, const char * argv[]);
void showHelpMenu(void);
int multipleOptionError(void);
AudioDeviceID getDeviceID(char * deviceName);
int isMuted(AudioDeviceID deviceID, AudioObjectPropertyAddress propertyAddress);
void mute(AudioDeviceID deviceID, int muteValue, AudioObjectPropertyAddress propertyAddress);
Float32 getNewVolume(AudioDeviceID deviceID, FunctionType type, AudioObjectPropertyAddress propertyAddress);
Float32 getVolume(AudioDeviceID deviceID, FunctionType type, AudioObjectPropertyAddress propertyAddress);
void setVolume(AudioDeviceID deviceID, Float32 volumeLevel, AudioObjectPropertyAddress propertyAddress);
void copyVolume(AudioDeviceID deviceID, Float32 volumeLevel, AudioObjectPropertyAddress propertyAddress);
AudioDeviceID getCurrentlySelectedOutputID(AudioObjectPropertyAddress propertyAddress);
