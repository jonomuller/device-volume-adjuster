-- Mapped to Command-Section Sign (¤)
tell application "Airfoil"
	-- Make sure the audio transmitted is from Spotify
	set theSource to make new application source
	set application file of theSource to "/Applications/Spotify.app"
	set current audio source to theSource
	
	set theSpeaker to first speaker whose name is "Logitech Z600"
	
	-- Toggle speaker connection
	if (connected of theSpeaker) then
		disconnect from theSpeaker
	else
		connect to theSpeaker
	end if
end tell