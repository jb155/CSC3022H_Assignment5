#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <string>
#include <fstream>

namespace BTHJAC013{
	template <typename T, int numChannels> class Audio {
	private:
		std::vector<T> audioData;
		int tSize;
		int sampleRate;
		double duration;
	public:
		//Constructor
		Audio (std::string inFile, int sampleRate){
			this->sampleRate = sampleRate;
			this->tSize = (int)sizeof(T);
			
			//read in the file
			readInFromFile(inFile);
		}
		 	
		//Copy Constructor
		Audio(const Audio & rhs){
			this->audioData = rhs.audioData;
			this->tSize = rhs.tSize;
			this->sampleRate = rhs.sampleRate;
			this->duration = rhs.duration;
		}
		
		//Move constructor
		Audio(Audio && rhs){
			this->audioData = std::move(rhs.audioData);
			this->tSize = rhs.tSize;
			this->sampleRate = rhs.sampleRate;
			this->duration = rhs.duration;
		}
		
		//Destructor (Default)
		~Audio() = default;
		
		//Copy Assignment Operator
		Audio &operator=(const Audio<T,1> & rhs){
			Audio<T,1>temp(rhs);
			*this = std::move(rhs);
			return *this;
		}
		
		//Move Assignment Operator
		Audio  &operator=(Audio<T,1> && rhs){
			//Aquire
			this->audioData = std::move(rhs.audioData);
			this->tSize = rhs.tSize;
			this->sampleRate = rhs.sampleRate;
			this->duration = rhs.duration;
			
			//release
			rhs.tSize = 0;
			rhs.sampleRate = 0;
			rhs.duration = 0;
			
			return *this;
		}
		
		
		//Read in file data from specified file (Handles the mono)
		void readInFromFile (std::string inFileName){
			std::ifstream infile(inFileName);
			
			//checks if file is open (ie exists)
			if(infile.is_open()){
				infile.seekg (0, infile.end); 	//Sets the position of the next character to be extracted from the input stream.
				long fileLength = infile.tellg();	//Returns the position of the current character in the input stream.
				infile.seekg (0, infile.beg);
				
				//Calculate the numbers of samples that will be found in the file
				int numberOfSamples = (int) (length / (tSize * numChannels));
				
				//calculate duration of the file
				duration = numberOfSamples/(float)sampleRate;
				
				audioData.resize((unsigned long)(numberOfSamples));	//resize data storage too fit the incoming data
				//go through the file data and store it in data storage
				for (int i = 0; i < audioData.size(); i++){
					char charBuff[tSize];
					infile.read (charBuff, tSize);
					
					audioData[i] = *(T *)charBuff;
				}
				
			}else{
				std::cout << "Could not open file." << std::cout;
			}
			infile.close();
		}
	}
}
#endif
