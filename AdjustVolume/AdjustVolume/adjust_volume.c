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
  int selected = 0;
  int functionType = 0;
  char deviceName[256];
  Float32 newVolume = -1.0;

  // No arguments found, show help muni
  if (argc < 2) {
    showHelpMenu();
    return 1;
  }

  static struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"device-name", required_argument, NULL, 'n'},
    {"get", no_argument, NULL, 'g'},
    {"increment", no_argument, NULL, 'i'},
    {"decrement", no_argument, NULL, 'd'},
    {"set", required_argument, NULL, 's'},
    {"copy", required_argument, NULL, 'c'},
    {"mute", no_argument, NULL, 'm'},
    {NULL, 0, NULL, 0}
  };

  while ((opt = getopt_long(argc, (char **) argv, "hn:gids:m", long_options, NULL)) != -1) {
    switch (opt) {
      case 'h':
        showHelpMenu();
        return 0;
        break;
      case 'n':
        strcpy(deviceName, optarg);
        break;
      case 'g':
        if (selected == 1) return multipleOptionError();
        selected = 1;
        functionType = kGetVolume;
        break;
      case 'i':
        if (selected == 1) return multipleOptionError();
        selected = 1;
        functionType = kIncrementVolume;
        break;
      case 'd':
        if (selected == 1) return multipleOptionError();
        selected = 1;
        functionType = kDecrementVolume;
        break;
      case 's':
        if (selected == 1) return multipleOptionError();
        selected = 1;
        functionType = kSetVolume;
        newVolume = atof(optarg);
        break;
      case 'c':
        if (selected == 1) return multipleOptionError();
        selected = 1;
        functionType = kCopyVolume;
        newVolume = atof(optarg);
        break;
      case 'm':
        if (selected == 1) return multipleOptionError();
        selected = 1;
        functionType = kMuteVolume;
        newVolume = 0.0;
        break;
      default:
        break;
    }
  }

  // No device name found
  if (strlen(deviceName) == 0) {
    printf("-n flag is required, please enter a device name.\n");
    return 1;
  }

  // No function argument given
  if (functionType == 0) {
    printf("No function selected. Please select one function to adjust the volume (-i, -d, -s, -g, -m).\n");
    return 1;
  }

  AudioDeviceID deviceID = getDeviceID(deviceName);

  if (deviceID == kAudioDeviceUnknown) {
    printf("Device name not found.\n");
    return 1;
  }

  // Property address of volume level, used for getting/setting volume
  AudioObjectPropertyAddress volumeAddress =  {
    kAudioHardwareServiceDeviceProperty_VirtualMasterVolume,
    kAudioDevicePropertyScopeOutput,
    kAudioObjectPropertyElementMaster
  };

  // Property address of mute value
  AudioObjectPropertyAddress muteAddress = {
    kAudioDevicePropertyMute,
    kAudioDevicePropertyScopeOutput,
    kAudioObjectPropertyElementMaster
  };

  int muted = isMuted(deviceID, muteAddress);

  if (functionType == kIncrementVolume || functionType == kDecrementVolume) {
    newVolume = getNewVolume(deviceID, functionType, volumeAddress);
  }

  if (functionType == kGetVolume) {
//    AudioObjectPropertyAddress deviceIdAddress =  {
//      kAudioHardwareServiceDeviceProperty_VirtualMasterVolume,
//      kAudioDevicePropertyScopeOutput,
//      kAudioObjectPropertyElementMaster
//    };
    // AudioDeviceID currentDeviceID = getCurrentlySelectedOutputID(deviceIdAddress);

    getVolume(deviceID, functionType, volumeAddress);
    return 0;
  }

  if (functionType == kMuteVolume) {
    mute(deviceID, !muted, muteAddress);
  } else {
    setVolume(deviceID, newVolume, volumeAddress);

    if (muted && newVolume > 0) {
      mute(deviceID, 0, muteAddress);
    }

    if (!muted && newVolume <= 0) {
      mute(deviceID, 1, muteAddress);
    }
  }

  return 0;
}

void showHelpMenu() {
  printf("Usage: AdjustVolume [-h] [-i] [-d] [-s] [-m] -n device-name\n"
         "  Options:\n"
         "    -h --help               Show this screen.\n"
         "    -i --increment          Increment volume by one step.\n"
         "    -d --decrement          Decrement volume by one step.\n"
         "    -s --set volume-level   Set volume to desired level (value between 0 and 1).\n"
         "    -g --get                Get the volume of the device (value between 0 and 1)."
         "    -m --mute               Mutes/unmutes device.\n"
         "    -n --device-name        The device you would like to adjust the volume of.\n");
}

int multipleOptionError() {
  printf("Please select only one option out of -i, -d, -s and -m.\n");
  return 1;
}

AudioDeviceID getDeviceID(char * deviceName) {
  UInt32 propertySize;
  int numberOfDevices = 0;
  AudioObjectID defaultID = kAudioObjectSystemObject;

  // Get address for all hardware devices
  AudioObjectPropertyAddress devicesAddress = {
    kAudioHardwarePropertyDevices,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMaster };

  // Retreive property size within devices address
  AudioObjectGetPropertyDataSize(defaultID, &devicesAddress, 0, nil, &propertySize);

  numberOfDevices = propertySize / sizeof(AudioObjectID);
  AudioDeviceID ids[numberOfDevices];

  // Populate ids from devices address
  AudioObjectGetPropertyData(defaultID, &devicesAddress, 0, nil, &propertySize, ids);

  for (int i = 0; i < numberOfDevices; i++) {
    UInt32 nameSize = 256;
    char name[nameSize];

    AudioObjectPropertyAddress deviceNameAddress = {
      kAudioDevicePropertyDeviceName,
      kAudioObjectPropertyScopeGlobal,
      kAudioObjectPropertyElementMaster };

    // Get device name from id
    AudioObjectGetPropertyData(ids[i], &deviceNameAddress, 0, nil, &nameSize, (char *) name);

    // Check if names match
    //printf("%s\n", name);
    if (strcmp(deviceName, (char *) name) == 0) {
      return ids[i];
    }
  }

  return kAudioDeviceUnknown;
}

int isMuted(AudioDeviceID deviceID, AudioObjectPropertyAddress propertyAddress) {
  UInt32 muted = 0;
  UInt32 muteSize = sizeof(muted);

  // Get current mute value
  AudioObjectGetPropertyData(deviceID, &propertyAddress, 0, nil, &muteSize, &muted);

  return muted;
}

void mute(AudioDeviceID deviceID, int muteValue, AudioObjectPropertyAddress propertyAddress) {
  UInt32 muteSize = sizeof(muteValue);

  // Set new mute value
  AudioObjectSetPropertyData(deviceID, &propertyAddress, 0, nil, muteSize, &muteValue);
}

Float32 getNewVolume(AudioDeviceID deviceID, FunctionType type, AudioObjectPropertyAddress propertyAddress) {
  Float32 volume;
  UInt32 volumeSize = sizeof(volume);
  Float32 incLevel = 1.0 / 16.0;

  // Get current volume level for requested device
  AudioObjectGetPropertyData(deviceID, &propertyAddress, 0, nil, &volumeSize, &volume);

  switch (type) {
    case kIncrementVolume:
      return volume + incLevel;
      break;
    case kDecrementVolume:
      return volume - incLevel;
    default:
      return -1.0;
      break;
  }
}

Float32 getVolume(AudioDeviceID deviceID, FunctionType type, AudioObjectPropertyAddress propertyAddress) {
  Float32 volume;
  UInt32 volumeSize = sizeof(volume);

  // Get current volume level for requested device
  AudioObjectGetPropertyData(deviceID, &propertyAddress, 0, nil, &volumeSize, &volume);
  printf("%f", volume);
  return volume;
}

void setVolume(AudioDeviceID deviceID, Float32 volume, AudioObjectPropertyAddress propertyAddress) {
  printf("Setting volume for reasons");
  UInt32 volumeSize = sizeof(volume);

  // Set new volume level
  AudioObjectSetPropertyData(deviceID, &propertyAddress, 0, nil, volumeSize, &volume);
}

AudioDeviceID getCurrentlySelectedOutputID(AudioObjectPropertyAddress propertyAddress) {
  UInt32 propertySize;
  AudioDeviceID deviceID = kAudioDeviceUnknown;
  UInt32 deviceIdSize = sizeof(deviceID);

  // get the default output device
  propertySize = sizeof(deviceID);
  AudioObjectGetPropertyData(kAudioHardwarePropertyDefaultOutputDevice, &propertyAddress, 0, nil, &deviceIdSize, &deviceID);

  return deviceID;
}
