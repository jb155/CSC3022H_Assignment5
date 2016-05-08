#define CATCH_CONFIG_MAIN

//includes
#include "Audio.h"

#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

#include "catch.hpp"

//Basic setup
//************************************************************************
int sampleRate = 44100;

//Base items
//Mono
vector<int8_t> mono8 = {1,2,3,4,5};
vector<int16_t> mono16 = {1,2,3,4,5};

BTHJAC013::Audio<int8_t, 1> mono8one (mono8, sampleRate);
BTHJAC013::Audio<int8_t, 1> mono8two (mono8, sampleRate);

BTHJAC013::Audio<int16_t, 1> mono16one (mono16, sampleRate);
BTHJAC013::Audio<int16_t, 1> mono16two (mono16, sampleRate);

pair<int8_t,int8_t> int8pair = make_pair(5,5);
pair<int16_t,int16_t> int16pair = make_pair(5,5);

//Setreo
vector<pair<int8_t, int8_t>> stereo8 = {int8pair, int8pair, int8pair, int8pair, int8pair};
vector<pair<int16_t, int16_t>> stereo16 = {int16pair, int16pair, int16pair, int16pair, int16pair};

BTHJAC013::Audio<int8_t, 2> stereo8one(stereoData8, sampleRate);
BTHJAC013::Audio<int8_t, 2> stereo8two(stereoData8, sampleRate);

BTHJAC013::Audio<int16_t, 2> stereo16one(stereo16, stereo16);
BTHJAC013::Audio<int16_t, 2> stereo16two(stereo16, stereo16);

//Base test units for testing (allows us to mes around and affect the base items)
BTHJAC013::Audio<int8_t, 1> mono8sampleTest = mono8one;
BTHJAC013::Audio<int16_t, 1> mono16sampleTest = mono16one;
BTHJAC013::Audio<int8_t, 2> stereo8sampleTest = stereo8one;
BTHJAC013::Audio<int16_t, 2> stereo16sampleTest = stereo16one;

//Comence Testing
//************************************************************************
//Mono Testing
//-----------------------------------
//Copy Construct
//==================
//8bit
TEST_CASE("Mono - 8bit: Copy Construct", "[mono] [8bit] [constructor]") {
	mono8sampleTest = BTHJAC013::Audio<int8_t, 1>(mono8one);

	REQUIRE(mono8one.getAudioData()[0] == mono8sampleTest.getAudioData()[0]);
	REQUIRE(mono8one.getAudioData().size() == mono8sampleTest.getAudioData().size());
}
//16bit
TEST_CASE("Mono - 16bit: Copy Construct", "[mono] [16bit] [constructor]") {
	mono16sampleTest = BTHJAC013::Audio<int16_t, 1>(mono16one);

	REQUIRE(mono16one.getAudioData()[0] == mono16sampleTest.getAudioData()[0]);
	REQUIRE(mono16one.getAudioData().size() == mono16sampleTest.getAudioData().size());
}

//| Operator
//=================
//8bit
TEST_CASE("Mono - 8bit: | Operator", "[mono] [8bit] [operator]") {
	vector<int8_t> temp = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};			//resulting vector
	mono8sampleTest = mono8one | mono8two;

	REQUIRE(temp.size() == mono8sampleTest.getAudioData().size());

	bool equal = true;
	for (int i = 0; i < temp.size(); ++i) {
		if (mono8sampleTest.getAudioData()[i] != temp[i]) {
		equal = false;
		}
	}
	REQUIRE(equal);
}
//16bit
TEST_CASE("Mono - 16bit: | Operator", "[mono] [16bit] [operator]") {
	vector<int16_t> temp = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
	mono16sampleTest = mono16one | mono16two;

	REQUIRE(temp.size() == mono16sampleTest.getAudioData().size());

	bool equal = true;
	for (int i = 0; i < temp.size(); ++i) {
		if (mono16sampleTest.getAudioData()[i] != temp[i]) {
			equal = false;
		}
	}
	REQUIRE(equal);
}

//* operator
//==================
//8bit
TEST_CASE("Mono - 8bit: * Opertator", "[mono] [8bit] [operator]") {
	mono8one = mono8one * pair<float, float>(3, 1);
	REQUIRE(mono8one.getAudioData()[0] == 3);
	mono8one.getAudioData()[0] = 1;
}
//16bit
TEST_CASE("Mono - 16bit: * Opertator", "[mono] [16bit] [operator]") {
	mono16one = mono16one * pair<float, float>(3, 1);
	REQUIRE(mono16one.getAudioData()[0] == 3);

	mono16one = mono16one * pair<float, float>(0.4, 1);
	REQUIRE(mono16one.getAudioData()[0] == 1);
}

//+ Operator
//==================
//8 bit
TEST_CASE("Mono - 8bit: + Operator", "[mono] [8bit] [operator]") {
	mono8one + mono8two;

	REQUIRE(mono8one.getAudioData()[0] == 2);
	mono8one = mono8one * pair<float, float>(0.5, 1);
}
//16bit
TEST_CASE("Mono - 16bit: + Operator", "[mono] [16bit] [operator]") {
	mono16one + mono16two;

	REQUIRE(mono16one.getAudioData()[0] == 2);
	mono16one = mono16one * pair<float, float>(0.5, 1);
}

//^ Operator
//==================
//8bit
TEST_CASE("Mono - 8bit: ^ Operator", "[mono] [8bit] [operator]") {
  	  mono8sampleTest = mono8sampleTest ^ pair<int, int>(0, 1);
	REQUIRE(mono8sampleTest.getAudioData().size() == 8);
}
//16bit
TEST_CASE("Mono - 16bit: ^ Operator", "[mono] [16bit] [operator]") {
	mono16sampleTest = mono16sampleTest ^ pair<int, int>(0, 1);
	REQUIRE(mono16sampleTest.getAudioData().size() == 8);
}




//Reverse
//===================
//8bit
TEST_CASE("Mono - 8bit: Reverse Function", "[mono] [8bit] [function]") {
   	 mono8two.reverse();
	REQUIRE(mono8one.getAudioData()[0] == mono8two.getAudioData()[4]);

  	  mono8two.reverse();
	REQUIRE(mono8one.getAudioData()[0] == mono8two.getAudioData()[0]);
}
//16bit
TEST_CASE("Mono - 16bit: Reverse Function", "[mono] [16bit] [function]") {
	mono16two.reverse();
	REQUIRE(mono16one.getAudioData()[0] == mono16two.getAudioData()[4]);

	mono16two.reverse();
	REQUIRE(mono16one.getAudioData()[0] == mono16two.getAudioData()[0]);
}

//RMS
//==================
//8bit
TEST_CASE("Test RMS Function - Mono - 8bit", "[mono] [8bit] [function]") {
	REQUIRE(mono8one.calculateRMS() == mono8sampleTest.calculateRMS());
}
//16bit
TEST_CASE("Test RMS Function - Mono - 16bit", "[mono] [16bit] [function]") {
	REQUIRE(mono16one.calculateRMS() == mono16sampleTest.calculateRMS());
}
