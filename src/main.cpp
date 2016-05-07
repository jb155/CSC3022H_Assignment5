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
		int bitCount = srd::stoi(argv[4]);
		if((bitCount!=8)&&(bitCount!=16)){
			std::cout << "only bit counts of 8 and 16 are currently supported by this software" << std::endl;
			return 0;
		}
		//Clips will be 1-channel (mono) or 2-channel (stereo).
		int numberOfChannels = std::stoi(argv[6]);
		if((numberOfChannels!=1)&&(numberOfChannels!=2)){
			std::cout << "Unsupported channels count (Currently only supports mono (1) and stereo (2))" << std::cout;
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
		if (op == "-add") {
		
		} else if (op == "-cut") {
		
		} else if (op == "-radd") {
		
		} else if (op == "-cat") {
		
		} else if (op == "-v") {
		
		} else if (op == "-rev") {
		
		} else if (op == "-rms") {
		
		} else if (op == "-norm") {
		
		} else if (op == "-fadein") {
		
		} else if (op == "-fadeout") {
		
		}
	}catch (exception& e){
		std::cout << "There is an error in the argument list" << std::endl;
	}
	return 0;
}	
