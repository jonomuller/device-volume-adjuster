//
//  adjust_volume.c
//  AdjustVolume
//
//  Created by Jono Muller on 29/12/2017.
//  Copyright © 2017 Jonathan Muller. All rights reserved.
//

#include "adjust_volume.h"

int startVolumeAdjuster(int argc, const char * argv[]) {
  int opt;
  int functionType = 0;
  char deviceName[256];
  int volumeLevel;
  
  while ((opt = getopt(argc, (char **) argv, "n:ids:")) != -1) {
    switch (opt) {
      case 'n':
        strcpy(deviceName, optarg);
        break;
      case 'i':
        functionType = kIncrementVolume;
        break;
      case 'd':
        functionType = kDecrementVolume;
        break;
      case 's':
        functionType = kSetVolume;
        volumeLevel = atoi(optarg);
        break;
      default:
        break;
    }
  }
  
  if (strlen(deviceName) == 0) {
    printf("-n is required, please enter a device name.");
    return 1;
  }
  
  if (functionType == 0) {
    printf("No function selected. Please select one function to adjust the volume (-i, -d, -s).");
    return 1;
  }
  
  AudioDeviceID deviceID = getDeviceID(deviceName);
  adjustVolume(deviceID, functionType);
  return 0;
}

AudioDeviceID getDeviceID(char deviceName[]) {
  return 0;
}

void adjustVolume(AudioDeviceID deviceID, FunctionType type) {
  
}
