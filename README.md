CSC3022H - Assignment 5

Jacques Botha - BTHJAC013

Instructions:
- Arguments and program invocation:
	samp -r sampleRateInHz -b bitCount -c noChannels [-o outFileName ] [ops] soundFile1 [soundFile2]

- Input Format
	Description
	+ -r Specifies the number of samples per second of the audio file(s) (usually
	44100)
	+ -b Specifies the size (in bits) of each sample. Only 8bit and 16bit should be
	supported in your program. More on this later on.
	+ -c Number of channels in the audio file(s). Your program will only support 1
	(mono) or 2 (stereo).
	+ “outFileName” is the name of the newly created sound clip (should default
	to “out”).
	+ ops is ONE of the following:
		* add: add soundFile1 and soundFile2
		* cut r1 r2: remove samples over range [r1,r2] (inclusive) (assumes
	one sound file)
	+ -radd r1 r2 s1 s2 : add soundFile1 and soundFile2 over sub-ranges indicated (in seconds). The ranges must be equal in length.
	+ -cat: concatenate soundFile1 and soundFile2
	+ -v r1 r2: volume factor for left/right audio (def=1.0/1.0) (assumes
	one sound file)
	+ -rev: reverse sound file (assumes one sound file only)
	+ -rms: Prints out the RMS of the soundfile (assumes one sound file
	only). More details will be given later on.
	+ -norm r1 r2: normalize file for left/right audio (assumes one sound file only and that r1 and r2 are floating point RMS values)
	+ [extra credit] -fadein n: n is the number of seconds (floating point	number) to slowly increase the volume (from 0) at the start of	soundFile1 (assumes one sound file).
	+ [extra credit] -fadeout n: n is the number of seconds (floating point	number) to slowly decrease the volume (from 1.0 to 0) at the end of soundFile1 (assumes one sound file). “soundFile1” is the name of the input .raw file. A second sound file is required for some operations as indiceted above.

	+ Eg: "play -r 44100 -e signed -b 16 -c 2 Run_44100_16bit_stereo.raw"



Files:

	src/Audio.h: Header file. Manages Templating and operator overloading.
	src/main.cpp: The driver file. Argument parsing happens here.

	NOTE:
	Did do the unit testing but it is currentlly throwing the make file out of wack. currently resides in "src/Unit testing.tar.gz". Folder also contains a make file for it.
	src/Unit testing.tar.gz/Unittests.cpp: This is where the catch unit tests are defined.
	src/Unit testing.tar.gz/catch.hpp: This runs the unit tests.
	
	build/: This is where the built files go. Audio clips for testing can be found inside.



