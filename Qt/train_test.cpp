#include "speaker_detect.h"


extern string WAVdir;
extern string MFCCdir;
extern string hmmTrainDir;
extern string HTKdir;

extern vector<string> users;
extern uint32_t num_users;


/*Wrap up of test data training*/
void trainData(){
    generateGrammar();
    generateMFCCConverter_train();
    generateTrainingData();
    generateHmmdefs();
    generatePhonemodels();
    generateSpeakerTrainModel();

    system("HParse HTKscripts/gram.txt HTKscripts/wdnet");
    system("HCopy -T 1 -C HTKscripts/config_wav2mfc -S HTKscripts/convert.scp");
    system("HCompV -C HTKscripts/config_mfc -f 0.01 -m -S HTKscripts/training.scp -M hmm0 HTKscripts/proto");
    system("HERest -C HTKscripts/config_mfc -I HTKscripts/speakertrainmodels0.mlf -S HTKscripts/training.scp -H hmm0/macros -H hmm0/hmmdefs -M hmm1 HTKscripts/phonemodels0");
    system("HERest -C HTKscripts/config_mfc -I HTKscripts/speakertrainmodels0.mlf -S HTKscripts/training.scp -H hmm1/macros -H hmm1/hmmdefs -M hmm2 HTKscripts/phonemodels0");
    system("HERest -C HTKscripts/config_mfc -I HTKscripts/speakertrainmodels0.mlf -S HTKscripts/training.scp -H hmm2/macros -H hmm2/hmmdefs -M hmm3 HTKscripts/phonemodels0");
}

/*Wrap up of test data comparison*/
void testData(){
    generateMFCCConverter_test();
    generateTestingData();
    generateDict();
    generateHmmList();

    system("HCopy -T 1 -C HTKscripts/config_wav2mfc -S HTKscripts/convert_test.scp");
    system("HVite -H hmm3/macros -H hmm3/hmmdefs -S HTKscripts/testing.scp -i HTKscripts/results_speaker.mlf -w HTKscripts/wdnet HTKscripts/dict HTKscripts/HmmList");
}

void generateGrammar(){
    string grammarFile = HTKdir + "gram.txt";
    ofstream fout;
    fout.open(grammarFile);

    fout << "$speaker = " ;

    for (int i = 0; i < users.size() - 1; ++i)
        fout << users[i] << " | ";
    fout << users[users.size() - 1] << ";" << endl;

    fout << "($speaker)" << endl;

    fout.close();
}

void generateMFCCConverter_train(){
    string convertFile = HTKdir + "convert.scp";
    ofstream fout;
    fout.open(convertFile);


    for(int i = 0; i < users.size(); i++)
        for(int j= 1; j <= 3; j++)
            fout << WAVdir << j << "_" << users[i] << ".wav " <<
                MFCCdir << j << "_" << users[i] << ".mfc" << endl;

    fout.close(); 
}

void generateTrainingData(){
    string trainingFile = HTKdir + "training.scp";
    ofstream fout;
    fout.open(trainingFile);


    for(int i = 0; i < users.size(); i++)
        for(int j= 1; j <= 3; j++)
            fout << MFCCdir << j << "_" << users[i] << ".mfc" << endl;

    fout.close(); 
}

void generateHmmdefs(){
    ofstream fout;
    string hmmdefsDest = hmmTrainDir + "hmmdefs";
    fout.open(hmmdefsDest);


    for(int i = 0; i < users.size(); i++)
    {
        fout << "~h \"" << users[i]  << "\"" << endl;
        fout << "<BEGINHMM>" << endl;
        fout << "<NUMSTATES> 3" << endl;
        fout << "<STATE> 2" << endl;
        fout << "<MEAN> 39" << endl;
        fout << " -5.082225e+00 3.982367e+00 -7.197697e-01 -9.173648e+00 2.086836e+00 -8.998974e+00 -8.199940e+00 1.284767e+00 5.328773e-01 3.061137e-01 -2.405128e+00 -1.716521e+00 6.856225e+01 6.154441e-03 1.597052e-02 1.556204e-03 9.779976e-04 -1.404641e-02 5.529255e-04 -7.100846e-03 -1.468378e-03 -7.061228e-03 -9.534549e-03 1.364146e-02 -1.074023e-03 -4.794824e-03 -1.610471e-03 2.255383e-03 -2.175921e-03 3.696258e-03 1.946870e-03 1.241731e-03 1.770806e-03 1.211543e-03 -1.569174e-03 3.639033e-03 4.749371e-03 6.422922e-03 -6.666430e-03" << endl;
        fout << "<VARIANCE> 39" << endl;
        fout << " 1.187131e+02 3.335015e+01 6.030525e+01 1.019203e+02 7.328522e+01 9.592013e+01 1.188274e+02 7.114619e+01 7.180795e+01 2.405059e+01 3.853842e+01 2.804758e+01 6.348481e+01 4.175172e+00 2.155601e+00 3.586910e+00 5.071088e+00 3.691230e+00 4.211236e+00 6.917050e+00 3.800553e+00 3.854606e+00 2.441062e+00 3.509262e+00 2.654112e+00 3.045118e+00 5.569248e-01 3.578149e-01 5.311546e-01 7.420131e-01 5.628281e-01 7.021801e-01 1.012596e+00 6.230080e-01 6.531282e-01 4.835335e-01 6.156138e-01 4.826650e-01 4.058960e-01" << endl;
        fout << "<GCONST> 1.346720e+02" << endl;
        fout << "<TRANSP> 3" << endl;
        fout << " 0.000000e+00 1.000000e+00 0.000000e+00" << endl;
        fout << " 0.000000e+00 9.000000e-01 9.999999e-02" << endl;
        fout << " 0.000000e+00 0.000000e+00 0.000000e+00" << endl;
        fout << "<ENDHMM>" << endl;
    }
    fout.close(); 
}


void generatePhonemodels(){
    string phonemodelsFile = HTKdir + "phonemodels0";
    ofstream fout;
    fout.open(phonemodelsFile);


    for(int i = 0; i < users.size(); i++)
        fout << users[i] << endl;

    fout.close(); 
}

void generateSpeakerTrainModel(){
    string trainModelFile = HTKdir + "speakertrainmodels0.mlf";
    ofstream fout;
    fout.open(trainModelFile);
    fout << "#!MLF!#" << endl;
    

    for(int i = 0; i < users.size(); i++)
        for(int j= 1; j <= 3; j++){
            fout << "\"*/" << j << "_" << users[i] << ".lab\"" << endl;
            fout << users[i] << endl;
            fout << "." << endl;
        }

    fout.close(); 
}


void generateMFCCConverter_test(){
    string convertFile = HTKdir + "convert_test.scp";
    ofstream fout;
    fout.open(convertFile);


    fout << WAVdir << "test.wav " << MFCCdir << "test.mfc" << endl;

    fout.close(); 
}

void generateTestingData(){
    string testingFile = HTKdir + "testing.scp";
    ofstream fout;
    fout.open(testingFile);

    fout << MFCCdir << "test.mfc" << endl;

    fout.close(); 
}

void generateDict(){
    string dictFile = HTKdir + "dict";
    ofstream fout;
    fout.open(dictFile);

    for(int i = 0; i < users.size(); i++)
        fout << users[i] << " " << users[i] << endl;

    fout.close(); 
}

void generateHmmList(){
    string HmmListFile = HTKdir + "HmmList";
    ofstream fout;
    fout.open(HmmListFile);

    for(int i = 0; i < users.size(); i++)
        fout << "\"" << users[i] << "\"" << endl;

    fout.close(); 
}


void read_line_as_token(const string &line, vector<string> &tokens)
{
    tokens.clear();
    string token;
    for(int g=0;g<line.size();g++)
    {
        if(line[g]=='(' || line[g]==')' || line[g]==',' || line[g]=='=' || line[g]==' ' || line[g]==';'  || line[g]=='/' || line[g]=='  ')
        {
            if(token.size()!=0 &&token[0]!=' ')
            {
                tokens.push_back(token);
                token.clear();
            }   
        }
        else
        {
            token.push_back(line[g]);
        }
    }
    if(token.size()!=0 && token[0]!=' ')
    tokens.push_back(token);
}

string extractTestSpeaker(){
    string results_speaker_file = HTKdir + "results_speaker.mlf";
    ifstream fin(results_speaker_file);

    string line[5];
    vector<string> tokens;
    int i = 0;
    while(!fin.eof()){
        getline(fin,line[i]);
        i++;       
    }
    read_line_as_token(line[2],tokens);

    fin.close();

    return tokens[2];
}
