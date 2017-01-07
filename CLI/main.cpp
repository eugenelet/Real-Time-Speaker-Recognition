#include "speaker_detect.h"

void UI();

vector<string> users;
uint32_t num_users;

string WAVdir = "Wav/";
string MFCCdir = "MFCC/";
string hmmTrainDir = "hmm0/";
string HTKdir = "HTKscripts/";

int main(int argc, char** argv){
    UI();
    return 0;
}

void UI(){

    cout << "Enter Number of Users: ";
    cin >> num_users;
    string user_tmp;
    for(int i=1; i<=num_users; i++){
        cout << "Enter name of user " << i <<": ";
        cin >> user_tmp;
        users.push_back(user_tmp);
        cout <<"Record 3 Instance of User's Speech" << endl;
        for(int j=1; j<=3; j++){
            cout << "Recording No." << j << "..." << endl;
            string dirName = WAVdir + to_string(j) + "_" + user_tmp + ".wav";
            record(1.5, dirName, true);
        }

    }

    users.push_back("sil");
    for(int i=i; i<=3; i++){
        string dirName = WAVdir + to_string(i) + "_" + "sil.wav";
        record(1.5, dirName, false);
    }

    /*TRAINING*/
    trainData();
    /*TESTING*/

    while(1){
        string dirName = WAVdir + "test.wav";
        record(.3, dirName, false);
        testData();
        string speaker = extractTestSpeaker();
        // cout << speaker << endl;
        printf("Speaker: %s   \r", speaker.c_str()); 
    }





}
