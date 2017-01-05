#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

vector<string> users;

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
string extractTestSpeaker();


string WAVdir = "Wav/";
string MFCCdir = "MFCC/";
string hmmTrainDir = "hmm0/";

int main(int argc, char** argv){

    /*if ( (argc < 2) || (strcmp(argv[1],"-h") == 0) ) { 
          cerr << "usage: ./output [cell library] [input set] " << endl;
          exit(1);
    }*/ 

    /*TRAINING*/
    users.push_back("Eugene");
    users.push_back("Mike");
    generateGrammar();
    generateMFCCConverter_train();
    generateTrainingData();
    generateHmmdefs();
    generatePhonemodels();
    generateSpeakerTrainModel();

    system("HParse gram.txt wdnet");
    system("HCopy -T 1 -C config_wav2mfc -S convert.scp");
    system("HCompV -C config_mfc -f 0.01 -m -S training.scp -M hmm0 proto");
    system("HERest -C config_mfc -I speakertrainmodels0.mlf -S training.scp -H hmm0/macros -H hmm0/hmmdefs -M hmm1 phonemodels0");
    system("HERest -C config_mfc -I speakertrainmodels0.mlf -S training.scp -H hmm1/macros -H hmm1/hmmdefs -M hmm2 phonemodels0");
    system("HERest -C config_mfc -I speakertrainmodels0.mlf -S training.scp -H hmm2/macros -H hmm2/hmmdefs -M hmm3 phonemodels0");

    /*TESTING*/
    generateMFCCConverter_test();
    generateTestingData();
    generateDict();
    generateHmmList();

    system("HCopy -T 1 -C config_wav2mfc -S convert_test.scp");
    system("HVite -H hmm3/macros -H hmm3/hmmdefs -S testing.scp -i results_speaker.mlf -w wdnet dict HmmList");

    string speaker = extractTestSpeaker();

    cout << speaker << endl;

    return 0;
}

void generateGrammar(){
    ofstream fout;
    fout.open("gram.txt");

    fout << "$speaker = " ;

    for (int i = 0; i < users.size() - 1; ++i)
        fout << users[i] << "_ | ";
    fout << users[users.size() - 1] << "_;" << endl;

    fout << "($speaker)" << endl;

    fout.close();
}

void generateMFCCConverter_train(){
    ofstream fout;
    fout.open("convert.scp");


    for(int i = 0; i < users.size(); i++)
        for(int j= 1; j <= 3; j++)
            fout << WAVdir << j << "_" << users[i] << ".wav " <<
                MFCCdir << j << "_" << users[i] << ".mfc" << endl;

    fout.close(); 
}

void generateTrainingData(){
    ofstream fout;
    fout.open("training.scp");


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
    ofstream fout;
    fout.open("phonemodels0");


    for(int i = 0; i < users.size(); i++)
        fout << users[i] << endl;

    fout.close(); 
}

void generateSpeakerTrainModel(){
    ofstream fout;
    fout.open("speakertrainmodels0.mlf");
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
    ofstream fout;
    fout.open("convert_test.scp");


    fout << WAVdir << "test.wav " << MFCCdir << "test.mfc" << endl;

    fout.close(); 
}

void generateTestingData(){
    ofstream fout;
    fout.open("testing.scp");

    fout << MFCCdir << "test.mfc" << endl;

    fout.close(); 
}

void generateDict(){
    ofstream fout;
    fout.open("dict");

    for(int i = 0; i < users.size(); i++)
        fout << users[i] << "_ " << users[i] << endl;

    fout.close(); 
}

void generateHmmList(){
    ofstream fout;
    fout.open("HmmList");

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
    ifstream fin("results_speaker.mlf");

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
