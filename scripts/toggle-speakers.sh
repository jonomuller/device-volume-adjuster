#!/bin/bash
# Mapped to Command-Shift-Plus/Minus Sign (±)
export PATH=/usr/local/bin:/usr/bin

if SwitchAudioSource -c | grep -Fxq "Built-in Output"
then
	SwitchAudioSource -s "Logitech Z600"
else
	SwitchAudioSource -s "Built-in Output"
fi
