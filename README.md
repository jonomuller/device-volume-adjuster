# device-volume-adjuster

Command line tool to change the volume of a particular output device in macOS. Useful for changing a device's volume without having to switch to it first (especially when mapped to a keyboard shortcut).

## Installation

* Download <code>AdjustVolume.zip</code> from the latest release [here](https://github.com/jonomuller/device-volume-adjuster/releases/latest)
* Unzip and copy the executable to <code>/usr/local/bin</code>

## Usage

```
AdjustVolume [-h] [-i] [-d] [-s] [-m] -n device-name

Options:
  -h --help               Show help screen.
  -i --increment          Increment volume by one step.
  -d --decrement          Decrement volume by one step.
  -s --set volume-level   Set volume to desired level (value between 0 and 1).
  -m --mute               Mutes/unmutes device.
  -n --device-name        The device you would like to adjust the volume of (required).
```

## Examples

* Increase volume of bluetooth speakers:
```
AdjustVolume -i -n "Logitech Z600"
```
* Set volume of Built-in Output to 20%:
```
AdjustVolume -s 0.2 -n "Built-in Output"
```

## My Configuration

I use [Airfoil](https://rogueamoeba.com/airfoil/) to route audio from Spotify to my Bluetooth speakers, with all other audio outputted from my laptop. I map increase/decrease commands from this program to keyboard shortcuts using [BetterTouchTool](https://www.boastr.net) (can also be done by creating a Service in Automator), meaning I can quickly adjust my speaker volume without having to switch it.

The scripts I use to do this, as well as other scripts I use to toggle audio routing, can be found in the [scripts]( https://github.com/jonomuller/device-volume-adjuster/tree/master/scripts) directory.
