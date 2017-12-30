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
#include <CoreAudio/CoreAudio.h>

#endif /* adjust_volume_h */

typedef enum {
  kIncrementVolume = 1,
  kDecrementVolume = 2,
  kSetVolume = 3,
} FunctionType;

int startVolumeAdjuster(int argc, const char * argv[]);
AudioDeviceID getDeviceID(char * deviceName);
void adjustVolume(AudioDeviceID deviceID, FunctionType type);
