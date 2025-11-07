// -----------------------------------------------------------------------------------
// Auxiliary Features

#ifdef ADDON_FEATURES_PRESENT
void addonFeaturesInit() {
  for (int i=0; i < 8; i++) {
    if (addonFeature[i].value == ON) addonFeature[i].value=1; else addonFeature[i].value=0;
  }
}

void addonFeaturesPoll() {
  // Nothing to do 
  // Note changes in the DIGITAL_IN pins are captured by T3.2 and send via a command
}

// :GXZn#
void addonFeaturesGetCommand(char *parameter, char *reply, bool &boolReply) {
  //check the feature number
  int i=parameter[1]-'1';
  int j=parameter[1]-'A';//@DS
  //if (i < 0 || i > 7)  { commandError=CE_PARAM_FORM; return; }  //@DS replaced by following lines
  if (i < 0 )  { commandError=CE_PARAM_FORM; return; }
  if (i > 8 && j < 0 )  { commandError=CE_PARAM_FORM; return; }
  if (j > 4)  { commandError=CE_PARAM_FORM; return; }
  if (j >= 0 )  { i = j + 9; }

  char s[255];
  if (addonFeature[i].purpose == SWITCH || addonFeature[i].purpose == DIGITAL_IN) {
    //reply[0] = addon_feature_1 + '0';
    //reply[1]=0;
    sprintf(s,"%d",addonFeature[i].value); 
    strcat(reply,s);
  } else { commandError=CE_CMD_UNKNOWN; return; }
  boolReply=false;
}

// :SXZn,v#
// for example :SXZ1,0#
void addonFeaturesSetCommand(char *parameter) {
  //check the feature number
  int i=parameter[1]-'1'; 
  int j=parameter[1]-'A'; VL("j="); VL(j); //@DS
  V("i="); VL(i);
  V("j="); VL(j);
 //if (i < 0 || i > 7)  { commandError=CE_PARAM_FORM; return; }  //@DS replaced by following lines
  if (i < 0 )  { commandError=CE_PARAM_FORM; return; }
  if (i > 8 && j < 0 )  { commandError=CE_PARAM_FORM; return; }
  if (j > 5)  { commandError=CE_PARAM_FORM; return; }
  if (j >= 0 )  { i = j + 9; }
  V("i(final)="); VL(i);
  VL("PURPOSE");
  VL(addonFeature[i].purpose);
  if (addonFeature[i].purpose == SWITCH || addonFeature[i].purpose == DIGITAL_IN) {
    if(parameter[2]!=',') { commandError=CE_PARAM_FORM; return; }
    if (parameter[3] != '0' && parameter[3] != '1')  { commandError=CE_PARAM_RANGE; return; }
    int v = parameter[3] - '0';
    //addon_feature_1 = parameter[3] - '0';
    addonFeature[i].value = v;
  } else { commandError=CE_CMD_UNKNOWN; return; }
}

#endif
