#include <string>

#include "Audio.h"

int main (int argc, const char * argv[]){
	if(argc<7){									//Checks if there are enough arguments given
		std::cout << "To few arguments given" << std::endl;
		return 0;
	}
	
	//Arguments will be as followiing
	//samp -r sampleRateInHz -b bitCount -c noChannels [-o outFileName ] [<ops>] soundFile1 [soundFile2]
	
	try{
		//setup
		//---------------------------------------------------------------------------------------------------------
		int sampleRate = std::stoi(argv[2]);
		//We will only use 8-bit int (signed int) and 16-bit int (signed int) sound clips
		int bitCount = std::stoi(argv[4]);
		if((bitCount!=8)&&(bitCount!=16)){
			std::cout << "Only bit counts of 8 and 16 are currently supported by this software" << std::endl;
			return 0;
		}
		//Clips will be 1-channel (mono) or 2-channel (stereo).
		int numberOfChannels = std::stoi(argv[6]);
		if((numberOfChannels!=1)&&(numberOfChannels!=2)){
			std::cout << "Unsupported channels count (Currently only supports mono (1) and stereo (2))" << std::endl;
			return 0;
		}
		//outfile
		std::string outFile = "out";
		int outFileExistsStep = 0;
		if(argv[7] == std::string("-o")){
			outFile = argv[8];
			outFileExistsStep = 2;
		}
		//operation
		std::string opType = argv[7 + outFileExistsStep];
		if (opType == "-add") { //add soundFile1 and soundFile2
			//the 2 sample audios
			std::string infile1 = argv[8 + outFileExistsStep];
			std::string infile2 = argv[9 + outFileExistsStep];
			
			//seeing as we check that numberOfChannels is 1 (mono) or 2(stereo) before this, it is safe to assume that 
			//we can just hand the objects numberOfChannels and it will do the rest
			
			if (bitCount == 8){
				BTHJAC013::Audio<int8_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
				BTHJAC013::Audio<int8_t,numberOfChannels> sample2 (infile2, sampleRate);//Create audio Sample #2,8bit
				(sample1 + sample2).writeToFile(outFile);			//Save audio (write to file)
			} else if (bitCount == 16){
				BTHJAC013::Audio<int16_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,16bit
				BTHJAC013::Audio<int16_t,numberOfChannels> sample2 (infile2, sampleRate);//Create audio Sample #2,16bit
				(sample1 + sample2).writeToFile(outFile);			//Save audio (write to file)
			} else {				//some extra resundency
				std::cout << "Don't know how this slipped through, but only 8 and 16 bit counts are currrently supported." << std::endl;
			}
			
		} else if (opType == "-cut") { //remove samples over range [r1,r2] (inclusive) (assumes
one sound file)
			int startPos = std::stoi(argv[8 + outFileExistsStep]);
			int endPos = std::stoi(argv[9 + outFileExistsStep]);
			std::pair<int, int> excludeSegment = std::make_pair(startPos, endPos);	//create a paint (int) that has start and end positions of the segment that will be cut out of the audio.
			
			std::string infile = argv[10 + outFileExistsSte];
			
			//seeing as we check that numberOfChannels is 1 (mono) or 2(stereo) before this, it is safe to assume that 
			//we can just hand the objects numberOfChannels and it will do the rest
			
			if (bitCount == 8){
				BTHJAC013::Audio<int8_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
				(sample1^excludeSegment).writeToFile(outFile);			//Save audio (write to file)
			} else if (bitCount == 16){
				BTHJAC013::Audio<int16_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,16bit
				(sample1^excludeSegment).writeToFile(outFile);			//Save audio (write to file)
			} else {				//some extra resundency
				std::cout << "Don't know how this slipped through, but only 8 and 16 bit counts are currrently supported." << std::endl;
			}
			
			
		} else if (opType == "-radd") {	//add soundFile1 and soundFile2 over sub-ranges indicated (in seconds). The ranges must be equal in length
		
			int range1 = std::stoi (argv[8 + outFileExistsStep]);
			int range2 = std::stoi (argv[9 + outFileExistsStep]);
			int subRange1 = std::stoi (argv[10 + outFileExistsStep]);
			int subRange2 = std::stoi (argv[11 + outFileExistsStep]);
			
			std::string infile1 = argv[12 + outFileExistsStep];
			std::string infile2 = argv[13 + outFileExistsStep];
			
			if (bitCount == 8){
				BTHJAC013::Audio<int8_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
				BTHJAC013::Audio<int8_t,numberOfChannels> sample2 (infile2, sampleRate);//Create audio Sample #2,8bit
				(sample1.extractRange(r1 * sampleRate, r2 * sampleRate) + sample2.extractRange(s1, s2)).writeToFile(outFile);
			} else if (bitCount == 16){
				BTHJAC013::Audio<int16_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,16bit
				BTHJAC013::Audio<int16_t,numberOfChannels> sample2 (infile2, sampleRate);//Create audio Sample #2,16bit
				(sample1.extractRange(r1 * sampleRate, r2 * sampleRate) + sample2.extractRange(s1, s2)).writeToFile(outFile);
			} else {				//some extra resundency
				std::cout << "Don't know how this slipped through, but only 8 and 16 bit counts are currrently supported." << std::endl;
			}		
		} else if (opType == "-cat") {	//concatenate soundFile1 and soundFile2
			
			std::infile1 = argv[8 + outFileExistsStep];
			std::infile2 = argv[9 + outFileExistsStep];
			
			if (bitCount == 8){
				BTHJAC013::Audio<int8_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
				BTHJAC013::Audio<int8_t,numberOfChannels> sample2 (infile2, sampleRate);//Create audio Sample #2,8bit
				(sample1 | sample2).writeToFile(outFile);			//Save audio (write to file)
			} else if (bitCount == 16){
				BTHJAC013::Audio<int16_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
				BTHJAC013::Audio<int16_t,numberOfChannels> sample2 (infile2, sampleRate);//Create audio Sample #2,8bit
				(sample1 | sample2).writeToFile(outFile);			//Save audio (write to file)
			} else {				//some extra resundency
				std::cout << "Don't know how this slipped through, but only 8 and 16 bit counts are currrently supported." << std::endl;
			}
		} else if (opType == "-v") {	//volume factor for left/right audio (def=1.0/1.0) (assumes one sound file)
			float leftVol = std::stof(argv[8+outFileExistsStep];
			float rightVol = std::stof(argv[9+outFileExistsStep];
			
			std::string infile = argv[10+outFileExistsStep];
			
			if (bitCount == 8){
				BTHJAC013::Audio<int8_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
				(sample1 * std::make_pair(leftVol, rightVol)).writeToFile(outFile);			//Save audio (write to file)
			} else if (bitCount == 16){
				BTHJAC013::Audio<int16_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,16bit
				(sample1 * std::make_pair(leftVol, rightVol)).writeToFile(outFile);			//Save audio (write to file)
			} else {				//some extra resundency
				std::cout << "Don't know how this slipped through, but only 8 and 16 bit counts are currrently supported." << std::endl;
			}
		} else if (opType == "-rev") {	//reverse sound file (assumes one sound file only)
			
			std::string infile = argv[8+outFileExistsStep];
			
			if (bitCount == 8){
				BTHJAC013::Audio<int8_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
				sample1.reverse();
				sample1.writeToFile(outFile);			//Save audio (write to file)
			} else if (bitCount == 16){
				BTHJAC013::Audio<int16_t,numberOfChannels> sample1 (infile1, sampleRate);//Create audio Sample #1,16bit
				sample1.reverse();
				sample1.writeToFile(outFile);			//Save audio (write to file)
			} else {				//some extra resundency
				std::cout << "Don't know how this slipped through, but only 8 and 16 bit counts are currrently supported." << std::endl;
			}
		} else if (opType == "-rms") {	//Prints out the RMS of the soundfile (assumes one sound file only).
			
			
			std::string infile = argv[8+outFileExistsStep];
			if(numbberOfChannels==1){	//mono
				if (bitCount == 8){
					BTHJAC013::Audio<int8_t,1> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
					std::cout << "RMS : " << infile << " = " << sample1.calculateRMS() << std::endl;
				} else if (bitCount == 16){
					BTHJAC013::Audio<int16_t,1> sample1 (infile1, sampleRate);//Create audio Sample #1,16bit
					std::cout << "RMS : " << infile << " = " << sample1.calculateRMS() << std::endl;
				} else {				//some extra resundency
					std::cout << "Don't know how this slipped through, but only 8 and 16 bit counts are currrently supported." << std::endl;
				}
			}else{				//Stereo
				if (bitCount == 8){
					BTHJAC013::Audio<int8_t,2> sample1 (infile1, sampleRate);//Create audio Sample #1,8bit
					std::cout << "Left RMS : " << infile << " = " << sample1.calculateRMS().first << std::endl;
					std::cout << "Right RMS : " << infile << " = " << sample1.calculateRMS().second << std::endl;
				} else if (bitCount == 16){
					BTHJAC013::Audio<int16_t,2> sample1 (infile1, sampleRate);//Create audio Sample #1,16bit
					std::cout << "Left RMS : " << infile << " = " << sample1.calculateRMS().first << std::endl;
					std::cout << "Right RMS : " << infile << " = " << sample1.calculateRMS().second << std::endl;
				} else {				//some extra resundency
					std::cout << "Don't know how this slipped through, but only 8 and 16 bit counts are currrently supported." << std::endl;
				}
			}

		
		} else if (opType == "-norm") {	//normalize file for left/right audio (assumes one sound file only and that r1 and r2 are floating point RMS values)
			int range1 = std::stoi(argv[8 + mod]);
			int range2 = std::stoi(argv[9 + mod]);
			std::string infile = argv[10 + mod];
			
			if (numChannels == 1) {
				if (bitCount == 16) {
					BTHJAC013::Audio<int16_t, 1> sample1(infile, sampleRate);
					sample1.normalize(range1);
					sample1.saveFwriteToFileile(outFile);
				} else {
					BTHJAC013::Audio<int8_t, 1> sample1(infile, sampleRate);
					sample1.normalize(range1);
					sample1.writeToFile(outFile);
				}
			} else {
				if (bitCount == 16) {
					BTHJAC013::Audio<int16_t, 2> sample1(infile, sampleRate);
					sample1.normalize(r1, r2);
					sample1.writeToFile(outFile);
				} else {
					BTHJAC013::Audio<int8_t, 2> sample1(infile, sampleRate);
					sample1.normalize(range1, range2);
					sample1.writeToFile(outFile);
				}
			}
		} else if (opType == "-fadein") {	//[extra credit]
			std::cout << "This feature is currently not supported in the trail version.\nPlease purchase the Gold premium edidtion for full access." << std::endl;
		} else if (opType == "-fadeout") {	//[extra credit]
			std::cout << "This feature is currently not supported in the trail version.\nPlease purchase the Gold premium edidtion for full access." << std::endl;
		}
	}catch (int e) {
		std::cout << "There is an error in the argument list" << std::endl;
	}
	return 0;
}	
