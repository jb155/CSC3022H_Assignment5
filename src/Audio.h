
#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>

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
		
		//Operators
		//********************************************************************************************
		//Copy Assignment Operator
		Audio &operator=(const Audio<T,1> & rhs){
			Audio<T,1>temp(rhs);			//Saw this on an online thread. Quite a nice shorthand...
			*this = std::move(rhs);			//but I'm wondering waht effect ths will have on efficiency....
			return *this;				//dont have time to worry bout that right now...but would be interesting to see.
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
		
		//Pipe operator. Implements concatination
		Audio &operator|(const Audio & rhs){
			audioData.insert(audioData.end(), rhs.audioData.begin(), rhs.audioData.end());
			return *this;
		}
		
		//* operator. Multiplies L&R channel with number as given (Think of is as audio panning)
		Audio & operator*(const std::pair<float, float> & volumeChange){
			for (int i = 0; i < audioData.size(); i++){	//Step through of all audio data
				audioData[i] *= volumeChange.first;	//this is mono....so only one of the two is needed
			}
			return *this;
		}
		
		//+ Operator. Adds 2 audio samples of same size (same tSize and #Samples)
		Audio &operator+(const Audio & rhs){
			for (int i = 0; i < audioData.size(); i++){	//Step through all the audio data
				audioData[i] = clampSampleMax(audioData[i] + rhs.audioData[i]);
			}
		}
		
		//^ Operator. Cut segment from audio sample.
		Audio &operator^(const std::pair<int,int> & cutOutTimeSegment){
			std::vector<T> resultingAudioSample;
			
			//Step through audioData and excl. data from the cutOutSegment
			for (int i = 0; i < audioData.size(); i++){
				if ((i < cutOutTimeSegment.first) || (i > cutOutTimeSegment.second)){
					resultingAudioSample.push_back(audioData[i]);
				}
			}
			
			//set current AudioData to new resulting data
			audioData = resultingAudioSample;
			
			return *this;
		}
		
		
		
		//Other fuctions
		//******************************************************************************
		//Read in file data from specified file (Handles the mono)
		void readInFromFile (std::string inFileName){
			std::ifstream infile(inFileName);
			
			//checks if file is open (ie exists)
			if(infile.is_open()){
				infile.seekg (0, infile.end); 	//Sets the position of the next character to be extracted from the input stream.
				long fileLength = infile.tellg();	//Returns the position of the current character in the input stream.
				infile.seekg (0, infile.beg);
				
				//Calculate the numbers of samples that will be found in the file
				int numberOfSamples = (int) (fileLength / (tSize * numChannels));
				
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
				std::cout << "Could not open file." << std::endl;
			}
			infile.close();
		}
		
		//Write to file	(Mono)
		void writeToFile (std::string outFileName){
			//File name as stated in brief
			std::string fileName = outFileName + "_" + std::to_string(sampleRate) + "_" + std::to_string(tSize * 8) + "_mono.raw";
			
			std::ofstream outfile(fileName, std::ios::binary | std::ios::out);
			
			//checks if file is open (ie has been created) else, throws toys out of pram
			if (outfile.is_open()){
				outfile.write((char *)&audioData[0], audioData.size() * tSize);
			}else{
				std::cout << "Could not save file" << std::endl;
			}
			outfile.close();
		}
		
		//Clamp sample Max. Adding two very loud files together may result in saturation.
		T clampSampleMax(T audioSample){
			if (tSize == 1){
				return (audioSample > INT8_MAX) ? INT8_MAX : audioSample; 
			}else{
				return (audioSample > INT16_MAX) ? INT16_MAX : audioSample;
			}
		}
		
		//reverses audio data (essentially just rewrites the data back to front)
		void reverse(){
			std::reverse(audioData.begin(), audioData.end());
		}
		
		//Select and return a specified segment of the audioData
		Audio & selectRange(int rangeStart, int rangeEnd){
			Audio * resultingAudioSampleObject = new Audio(*this);	//essentially make a duplicate of the current audio file
			
			resultingAudioSampleObject->audioData.clear();		//Clear out data stored within object
			resultingAudioSampleObject->audioData.resize((unsigned long)(rangeEnd-rangeStart));	//Resize audioData to new range
			
			//was trying nested for loops and a ton of if statements....turns out...c++ has a convenient copy funct. Nice
			//Copies audio data from start of the range to the end range into the new resulting audio sample 
			std::copy(audioData.begin() + rangeStart, audioData.begin() + rangeEnd, resultingAudioSampleObject->audioData.begin());
			
			return *resultingAudioSampleObject;
		}
		
		//returns the audio data within this object
		std::vector <T> & getAudioData(){
			return audioData;
		}
		
		//Calculate the RMS of the audio file (could be seen as the average of the audioFile)
		float calcRMS(){
			float audioDataSizeInverse = 1/((float)audioData.size());
			return std::sqrt(audioDataSizeInverse * std::accumulate(audioData.begin(), audioData.end(), 0,[] (T accum, T x) {return (accum + pow(x, 2));}));	
		}
		
		//normalizes audio data to given rms value
		Audio & normalize(float rms){
			NormalFunction normalF (calcRMS(), rms);
			Audio * tempAudio = new Audio(*this);
			std::transform(audioData.begin(), audioData.end(), tempAudio->audioData.begin(), normalF);
			return * tempAudio;
		}
		
		//Inner function that is used to calculate the normal values. Someone in the lab had this bright idea, can't remeber who...but kudo's to that person.
		class NormalFunction{
		public:
			float currentRMS;
			float targetRMS;
			
			NormalFunction(float curRMS, float tarRMS):currentRMS(curRMS), targetRMS(tarRMS){}
			
			T operator()(T inputAmpl){
				return (T) (inputAmpl*(targetRMS/currentRMS));
			}
		};
	};
        
	//Now for the stereo stuffs
	template <typename T> class Audio<T, 2> {
        private:
		std::vector<std::pair<T, T>> audioData;
		int tSize;
		int sampleRate;
		double duration;
	public:
		Audio(std::string fileName, int sampleRate) {
			readInFromFile(fileName);
			this->sampleRate = sampleRate;
			this->tSize = (int)sizeof(T);
		}
		
		~Audio() = default;
		
		//Copy Constructor
		Audio(const Audio & rhs){
			this->audioData = rhs.audioData;
			this->tSize = rhs.tSize;
			this->sampleRate = rhs.sampleRate;
			this->duration = rhs.duration;
		}
		
		//Move Constructor
		Audio (Audio && rhs){
			this->audioData = std::move(rhs.audioData);
			this->sampleRate = rhs.sampleRate;
			this->tSize = rhs.tSize;
			this->duration = rhs.duration;
		}
		
		
		//Operators
		//***********************************************************************
		//Copy assign operator
		Audio &operator=(const Audio & rhs){
			Audio<T, 2> tempAudio(rhs);
			*this = std::move(tempAudio);
			return *this;
		}
		
		//Move assign operator
		Audio &operator=(Audio && rhs){
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
		
		//Pipe operator. Implements concatination
		Audio &operator|(const Audio & rhs){
			audioData.insert(audioData.end(), rhs.audioData.begin(), rhs.audioData.end());
			return *this;
		}
		
		//* operator. Multiplies L&R channel with number as given (Think of is as audio panning)
		Audio &operator*(const std::pair <float, float> & volumeChange){
			for (int i = 0; i < audioData.size(); i++){	//Step through of all audio data
				audioData[i] = std::make_pair(audioData[i].first * volumeChange.first, audioData[i].second * volumeChange.second);
			}
			return *this;
		}
		
		//+ Operator. Adds 2 audio samples of same size (same tSize and #Samples)
		Audio &operator+(const Audio & rhs){
			for (int i = 0; i < audioData.size(); i++){	//Step through all the audio data
				T leftClamp = clampSampleMax(audioData[i].first + rhs.audioData[i].first);
				T rightClamp = clampSampleMax(audioData[i].second + rhs.audioData[i].second);

				audioData[i] = std::make_pair(leftClamp, rightClamp);
			}
		}
		
		//^ Operator. Cut segment from audio sample.
		Audio &operator^(const std::pair<int,int> & cutOutTimeSegment){
			std::vector<std::pair<T, T>> resultingAudioSample;
			
			//Step through audioData and excl. data from the cutOutSegment
			for (int i = 0; i < audioData.size(); i++){
				if ((i < cutOutTimeSegment.first) || (i > cutOutTimeSegment.second)){
					resultingAudioSample.push_back(audioData[i]);
				}
			}
			
			//set current AudioData to new resulting data
			audioData = resultingAudioSample;
			
			return *this;
		}
		
		
		//Functions
		//************************************************************************
		//Read in file data from specified file (Handles the Stereo)
		void readInFromFile (std::string inFileName){
			std::ifstream infile(inFileName);
			
			//checks if file is open (ie exists)
			if(infile.is_open()){
				infile.seekg (0, infile.end); 	//Sets the position of the next character to be extracted from the input stream.
				long fileLength = infile.tellg();	//Returns the position of the current character in the input stream.
				infile.seekg (0, infile.beg);
				
				//Calculate the numbers of samples that will be found in the file
				int numberOfSamples = (int) (fileLength / (tSize * 2));
				
				//calculate duration of the file
				duration = numberOfSamples/(float)sampleRate;
				
				audioData.resize((unsigned long)(numberOfSamples));	//resize data storage too fit the incoming data
				//go through the file data and store it in data storage
				for (int i = 0; i < audioData.size(); i++){
					char charBuffLeft[tSize];
					infile.read (charBuffLeft, tSize);
					char charBuffRight[tSize];
					infile.read (charBuffRight,tSize);
					
					audioData[i] = std::make_pair(*(T *)charBuffLeft, *(T *)charBuffRight);
				}
				
			}else{
				std::cout << "Could not open file." << std::endl;
			}
			infile.close();
		}
		
		//Write to file	(Stereo)
		void writeToFile (std::string outFileName){
			//File name as stated in brief
			std::string fileName = outFileName + "_" + std::to_string(sampleRate) + "_" + std::to_string(tSize * 8) + "_mono.raw";
			
			std::ofstream outfile(fileName, std::ios::binary | std::ios::out);
			
			//checks if file is open (ie has been created) else, throws toys out of pram
			if (outfile.is_open()){
				for (auto data : audioData) {
					outfile.write((char *)&data.first, tSize);
					outfile.write((char *)&data.second, tSize);
				}
			}else{
				std::cout << "Could not save file" << std::endl;
			}
			outfile.close();
		}
		
		//Clamp sample Max. Adding two very loud files together may result in saturation.
		T clampSampleMax(T audioSample){
			if (tSize == 1){
				return (audioSample > INT8_MAX) ? INT8_MAX : audioSample; 
			}else{
				return (audioSample > INT16_MAX) ? INT16_MAX : audioSample;
			}
		}
		
		//reverses audio data (essentially just rewrites the data back to front). Again...a funtion I was writing from scratc...untill someone pointed out it already exists
		void reverse(){
			std::reverse(audioData.begin(), audioData.end());
		}
		
		//Select and return a specified segment of the audioData
		Audio & selectRange(int rangeStart, int rangeEnd){
			Audio<T,2> * resultingAudioSampleObject = new Audio(*this);	//essentially make a duplicate of the current audio file
			
			resultingAudioSampleObject->audioData.clear();		//Clear out data stored within object
			resultingAudioSampleObject->audioData.resize((unsigned long)(rangeEnd-rangeStart));	//Resize audioData to new range
			
			//was trying nested for loops and a ton of if statements....turns out...c++ has a convenient copy funct. Nice
			//Copies audio data from start of the range to the end range into the new resulting audio sample 
			std::copy(audioData.begin() + rangeStart, audioData.begin() + rangeEnd, resultingAudioSampleObject->audioData.begin());
			
			return *resultingAudioSampleObject;
		}
		
		//returns the audio data within this object
		std::vector <T> & getAudioData(){
			return audioData;
		}
		
		//Calculate the RMS of the audio file (could be seen as the average of the audioFile)
		std::pair<float,float> calcRMS(){
			std::pair<float, float> rms;
			float audioDataSizeInverse = 1 / (float) audioData.size();

			rms = std::accumulate(audioData.begin(), audioData.end(), std::pair<float, float>(0, 0),[] (std::pair<float, float> accum, std::pair<T, T> x) {
				accum = std::make_pair(accum.first + (std::pow(x.first, 2)), accum.second + (std::pow(x.second, 2)));
				return accum;
			});		//Had someone explain this to me....I still dont full comprehend how this actually works.
			rms = std::make_pair(std::sqrt(rms.first * audioDataSizeInverse), std::sqrt(rms.second * audioDataSizeInverse));
			return rms;		
		}
		
		//normalizes audio data to given rms value
		Audio & normalize(float leftRMS, float rightRMS){
			NormalFunction normalF (calcRMS(), std::make_pair(leftRMS, rightRMS));
			Audio<T,2> * tempAudio = new Audio(*this);
			std::transform(audioData.begin(), audioData.end(), tempAudio->audioData.begin(), normalF);
			return * tempAudio;
		}
		
		//Inner function that is used to calculate the normal values. Someone in the lab had this bright idea, can't remeber who...but kudo's to that person.
		class NormalFunction{
		public:
			std::pair<float, float> currentRMS;
			std::pair<float, float> targetRMS;
			
			NormalFunction(std::pair<float, float> curRMS, std::pair<float, float> tarRMS):currentRMS(curRMS), targetRMS(tarRMS){}
			
			std::pair<T, T> operator()(std::pair<float, float> inputAmpl){
				return std::make_pair((T) (inputAmpl.first * (targetRMS.first / currentRMS.first)), (T) (inputAmpl.second * (targetRMS.second / currentRMS.second)));
			}
		};
		
	};
}
#endif
