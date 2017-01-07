#include <stdio.h> 
#include <stdlib.h> 
#include <string> 
#include <iostream>
#include <unistd.h>
#include <AL/al.h> 
#include <AL/alc.h> 
#include <sys/time.h>
#include <ctime> 
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

/*TRAINING*/
void generateGrammar();
void generateMFCCConverter_train();
void generateTrainingData();
void generateHmmdefs();
void generatePhonemodels();
void generateSpeakerTrainModel();

/*TESTING*/
void generateMFCCConverter_test();
void generateTestingData();
void generateDict();
void generateHmmList();

/*Wrap up of Train and Test*/
void trainData();
void testData();
/*Extract Current Speaker*/
string extractTestSpeaker();

/*Generate wav file based on record*/
void record(double record_time, string wavFile, bool timer_on);
