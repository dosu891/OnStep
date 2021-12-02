// -------------------------------------------------------------------------------------------------------
// Handle Auxiliary Features on the AddOn (Teensy 3.2)
// @DS
#pragma once

#include "Accessories.h"

#ifdef ADDON_FEATURES_PRESENT

#define FEATURE1_PIN 13  				//LED
#define FEATURE1_DEFAULT_VALUE 0		//Initial value at startup
#define FEATURE1_ACTIVE_VALUE 1			//value to represents an active pin (0 or 1)
#define FEATURE1_GET_CMD ":SXZ1#"		//LX200 command to get the feature value from the main board

char feature1_previous_value = 0;

// Init the IO pins
void AddonfeaturesInit() {
	pinMode(FEATURE1_PIN,OUTPUT); 
	digitalWrite(FEATURE1_PIN,FEATURE1_DEFAULT_VALUE==FEATURE1_ACTIVE_VALUE?HIGH:LOW);
}

// set the pin to the new value
void AddonfeaturesSetCommand(char parameter) {
  digitalWrite(FEATURE1_PIN,parameter==FEATURE1_ACTIVE_VALUE?HIGH:LOW);
}

// Check the status of the feature on the main board (T3.6) and set the pin accordingly
void AddonfeaturesPoll() {
	char s[40] = "";
	if (!command(FEATURE1_GET_CMD,s) || s[0]==0) { return; }
	if (s[0]=='0' || s[0]=='1'){
		char p = s[0]-'0';
		if( feature1_previous_value != p) {
			AddonfeaturesSetCommand(p);
			feature1_previous_value = p;
		}
	}
}


#endif
