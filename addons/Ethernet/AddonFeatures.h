// -------------------------------------------------------------------------------------------------------
// Handle Auxiliary Features on the AddOn (Teensy 3.2)
// @DS
// LX200-command to read a DIGITAL_IN pin: :GXZn#   for n:1-9 or A-F"
// LX200-command to read a SWITCH pin: :SXZn,v#   for n:1-8 or A-F and c=0 or 1"
#pragma once

#include "Accessories.h"

//@DS
#if ADDON_FEATURE1_PURPOSE != OFF || ADDON_FEATURE2_PURPOSE != OFF || ADDON_FEATURE3_PURPOSE != OFF || ADDON_FEATURE4_PURPOSE != OFF || ADDON_FEATURE5_PURPOSE != OFF || ADDON_FEATURE6_PURPOSE != OFF || ADDON_FEATURE7_PURPOSE != OFF || ADDON_FEATURE8_PURPOSE != OFF || ADDON_FEATURE9_PURPOSE != OFF || ADDON_FEATUREA_PURPOSE != OFF || ADDON_FEATUREB_PURPOSE != OFF || ADDON_FEATUREC_PURPOSE != OFF || ADDON_FEATURED_PURPOSE != OFF || ADDON_FEATUREE_PURPOSE != OFF || ADDON_FEATUREF_PURPOSE != OFF
  #define ADDON_FEATURES_PRESENT
#endif

#ifdef ADDON_FEATURES_PRESENT

#ifndef ADDON_FEATURE1_ACTIVE_STATE
  #define ADDON_FEATURE1_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURE2_ACTIVE_STATE
  #define ADDON_FEATURE2_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURE3_ACTIVE_STATE
  #define ADDON_FEATURE3_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURE4_ACTIVE_STATE
  #define ADDON_FEATURE4_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURE5_ACTIVE_STATE
  #define ADDON_FEATURE5_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURE6_ACTIVE_STATE
  #define ADDON_FEATURE6_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURE7_ACTIVE_STATE
  #define ADDON_FEATURE7_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURE8_ACTIVE_STATE
  #define ADDON_FEATURE8_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURE9_ACTIVE_STATE
  #define ADDON_FEATURE9_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATUREA_ACTIVE_STATE
  #define ADDON_FEATUREA_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATUREB_ACTIVE_STATE
  #define ADDON_FEATUREB_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATUREC_ACTIVE_STATE
  #define ADDON_FEATUREC_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATURED_ACTIVE_STATE
  #define ADDON_FEATURED_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATUREE_ACTIVE_STATE
  #define ADDON_FEATUREE_ACTIVE_STATE 1
#endif
#ifndef ADDON_FEATUREF_ACTIVE_STATE
  #define ADDON_FEATUREF_ACTIVE_STATE 1
#endif

//char addon_feature_1 = 0;
typedef struct AddonFeatures {
   const char* name;
   const int purpose;
   const int64_t pin;
   int value;
   int active;
} addonFeatures;

addonFeatures addonFeature[15] = {
  {ADDON_FEATURE1_NAME,ADDON_FEATURE1_PURPOSE,ADDON_FEATURE1_PIN,ADDON_FEATURE1_DEFAULT_VALUE,ADDON_FEATURE1_ACTIVE_STATE},
  {ADDON_FEATURE2_NAME,ADDON_FEATURE2_PURPOSE,ADDON_FEATURE2_PIN,ADDON_FEATURE2_DEFAULT_VALUE,ADDON_FEATURE2_ACTIVE_STATE},
  {ADDON_FEATURE3_NAME,ADDON_FEATURE3_PURPOSE,ADDON_FEATURE3_PIN,ADDON_FEATURE3_DEFAULT_VALUE,ADDON_FEATURE3_ACTIVE_STATE},
  {ADDON_FEATURE4_NAME,ADDON_FEATURE4_PURPOSE,ADDON_FEATURE4_PIN,ADDON_FEATURE4_DEFAULT_VALUE,ADDON_FEATURE4_ACTIVE_STATE},
  {ADDON_FEATURE5_NAME,ADDON_FEATURE5_PURPOSE,ADDON_FEATURE5_PIN,ADDON_FEATURE5_DEFAULT_VALUE,ADDON_FEATURE5_ACTIVE_STATE},
  {ADDON_FEATURE6_NAME,ADDON_FEATURE6_PURPOSE,ADDON_FEATURE6_PIN,ADDON_FEATURE6_DEFAULT_VALUE,ADDON_FEATURE6_ACTIVE_STATE},
  {ADDON_FEATURE7_NAME,ADDON_FEATURE7_PURPOSE,ADDON_FEATURE7_PIN,ADDON_FEATURE7_DEFAULT_VALUE,ADDON_FEATURE7_ACTIVE_STATE},
  {ADDON_FEATURE8_NAME,ADDON_FEATURE8_PURPOSE,ADDON_FEATURE8_PIN,ADDON_FEATURE8_DEFAULT_VALUE,ADDON_FEATURE8_ACTIVE_STATE},
  {ADDON_FEATURE9_NAME,ADDON_FEATURE9_PURPOSE,ADDON_FEATURE9_PIN,ADDON_FEATURE9_DEFAULT_VALUE,ADDON_FEATURE9_ACTIVE_STATE},
  {ADDON_FEATUREA_NAME,ADDON_FEATUREA_PURPOSE,ADDON_FEATUREA_PIN,ADDON_FEATUREA_DEFAULT_VALUE,ADDON_FEATUREA_ACTIVE_STATE},
  {ADDON_FEATUREB_NAME,ADDON_FEATUREB_PURPOSE,ADDON_FEATUREB_PIN,ADDON_FEATUREB_DEFAULT_VALUE,ADDON_FEATUREB_ACTIVE_STATE},
  {ADDON_FEATUREC_NAME,ADDON_FEATUREC_PURPOSE,ADDON_FEATUREC_PIN,ADDON_FEATUREC_DEFAULT_VALUE,ADDON_FEATUREC_ACTIVE_STATE},
  {ADDON_FEATURED_NAME,ADDON_FEATURED_PURPOSE,ADDON_FEATURED_PIN,ADDON_FEATURED_DEFAULT_VALUE,ADDON_FEATURED_ACTIVE_STATE},
  {ADDON_FEATUREE_NAME,ADDON_FEATUREE_PURPOSE,ADDON_FEATUREE_PIN,ADDON_FEATUREE_DEFAULT_VALUE,ADDON_FEATUREE_ACTIVE_STATE},
  {ADDON_FEATUREF_NAME,ADDON_FEATUREF_PURPOSE,ADDON_FEATUREF_PIN,ADDON_FEATUREF_DEFAULT_VALUE,ADDON_FEATUREF_ACTIVE_STATE}
};

#define FEATURE1_GET_CMD ":GXZ1#"		//LX200 command to get the feature value from the main board

char feature1_previous_value = 0;

// set the pin to the new value
void AddonfeaturesSetCommand(int feature_nr, int parameter) {
  if(addonFeature[feature_nr].purpose == SWITCH) {
    addonFeature[feature_nr].value = parameter == 0 ? 0 : addonFeature[feature_nr].active;
    digitalWrite(addonFeature[feature_nr].pin,addonFeature[feature_nr].value==addonFeature[feature_nr].active?HIGH:LOW);
  }
}

// get the pin to the new value
void AddonfeaturesGetCommand(int feature_nr) {
  if(addonFeature[feature_nr].purpose == DIGITAL_IN) {
    if (digitalRead(addonFeature[feature_nr].pin) == HIGH) {
      addonFeature[feature_nr].value = addonFeature[feature_nr].active; 
    } else {
      addonFeature[feature_nr].value = 0; 
    } 
  }
}

// Init the IO pins
void AddonfeaturesInit() {
  for (int i=0; i < 15; i++) {
    if (addonFeature[i].purpose == SWITCH) {
      pinMode(addonFeature[i].pin,OUTPUT);
      VF("WEM: setup feature OUTPUT "); V(i); V(": "); VL(addonFeature[i].pin);
          
      AddonfeaturesSetCommand(i,addonFeature[i].value);
    } else if (addonFeature[i].purpose == DIGITAL_IN) {
      pinMode(addonFeature[i].pin,INPUT);
      AddonfeaturesGetCommand(i);
      VF("WEM: setup feature INPUT "); V(i); V(": "); VL(addonFeature[i].pin);
    }
  } 
}

// Check the status of the feature on the main board (T3.6) and get/set the pin accordingly
void AddonfeaturesPoll() {
  for (int i=0; i < 15; i++) {
    if (addonFeature[i].purpose == SWITCH) {
      char s[40] = "";
      char c[40] = "";
      if(i<9) {
        sprintf(c,":GXZ%c#",i+'1');
      } else {
        sprintf(c,":GXZ%c#",i-9+'A');
      }
      if (!command(c,s) || s[0]==0) { return; }
      if (s[0]=='0' || s[0]=='1'){
        int new_value = s[0]-'0' == 0 ? 0 : addonFeature[i].active;
        if( addonFeature[i].value != new_value) {
          VF("WEM: Write feature "); V(i); V(": "); VL(s);
          AddonfeaturesSetCommand(i,new_value);
        }
      }
    } else if (addonFeature[i].purpose == DIGITAL_IN) {
      int previous_value = addonFeature[i].value;
      AddonfeaturesGetCommand(i);
      if( addonFeature[i].value != previous_value) {
        char s[40] = "";
        char c[40]="";
        char v = addonFeature[i].value == 0 ? '0' : '1';
        sprintf(c,":SXZ%c,%c#",i+'1',v);
        VF("WEM: Read feature "); V(i); V(": "); VL(v);
        command(c,s);
      }
    }
  }
}


#endif
