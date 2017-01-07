#include "speaker_detect.h"

// Destination Directories
string WAVdir = "Wav/";
string MFCCdir = "MFCC/";
string hmmTrainDir = "hmm0/";
string HTKdir = "HTKscripts/";

// Users and info
vector<string> users;
uint32_t num_users;
int people=0;

// Notifications
QLabel *notifications;
QLabel *record_notifications;

// Check for running Test Thread
bool isTesting = false;

/*Testing Thread run definition*/
void Testing::run(){
    QString result;
    while(isTesting) {
		// Notify main thread:
        string dirName = WAVdir + "test.wav";
        record(.3, dirName, false);
        testData();
        string speaker = extractTestSpeaker();
        emit updateTestResult(QString::fromStdString(speaker));
    }
    notifications->setText("");
}


/*Update Test result from testing Thread*/
void recognitionWidget::onUpdateTestResult(QString info) {
    // Processing code
    if(info != "sil")
	    notifications->setText(info);
	else
	    notifications->setText("");

}

/*Add users to Database*/
void recognitionWidget::add()
{
    lineEdit_Add->show();
	lineEdit_Add->setText("Enter User name");    
	lineEdit_Add->setCursorPosition(0);
	QTimer::singleShot(0, lineEdit_Add, SLOT(setFocus()));
	lineEdit_Add->selectAll(); 
}

/*Delete users from database*/
void recognitionWidget::del()
{
	lineEdit_Del->show();
	lineEdit_Del->setText(tr("Enter User Name"));    
	lineEdit_Del->setCursorPosition(0);
	QTimer::singleShot(0, lineEdit_Del, SLOT(setFocus()));
	lineEdit_Del->selectAll();   
}

/*Confirm addition of user*/
void recognitionWidget::ok()
{
	if(people==5)
		notifications->setText("Only Limited to 5 Users!");
	else{
		// Prevent Addition of multiple user
		for(int i=0; i<users.size(); i++)
			if(lineEdit_Add->text().toStdString()==users[i] || lineEdit_Add->text().toStdString()=="sil"){
				notifications->setText("User Exists!");
				notifications->repaint();
				usleep(100000);
				notifications->setText("");
				return;
			}
		// Add user as usual
		for(int i=0; i<5; i++)
			if(available[i]){
				content[i] = lineEdit_Add->text();  //Obtain contents from lineEdit
				people++;
				online_n[i]->setText(content[i]);
				online_n[i]->show();
				online2->setText(QString::number(people));
				available[i]=0;

				/*Notify addition of new user*/
				string notifications_tmp = content[i].toStdString() + "added to database";
				notifications->setText(QString::fromStdString(notifications_tmp));
				notifications->repaint();
				usleep(100000);

				/*Add user to database*/
				users.push_back(content[i].toStdString());
				for(int j=1; j<=3; j++){
					notifications_tmp = "Recording No." + to_string(j) + "...";
					notifications->setText(QString::fromStdString(notifications_tmp));
					notifications->repaint();
		            string dirName = WAVdir + to_string(j) + "_" + content[i].toStdString() + ".wav";
		            record(1.5, dirName, true);
		        }

		        usleep(10000);
		        notifications->setText("");
		        record_notifications->setText("");


				break;
			}
	}
	
	lineEdit_Add->hide();
}

/*Confirm deletion of user*/
void recognitionWidget::ok_del()
{
	if (people==0)
	{
		notifications->setText("There is no one in the meeting!");
	}
	else
	{

		for (int i=0;i<5;i++)
		{	
			if (lineEdit_Del->text()==content[i] && lineEdit_Del->text()!="sil")
			{
				online_n[i]->setText(" ");
				people--;
				available[i]=1;
				online2->setText(QString::number(people));
				notifications->setText("Someone left this meeting!");
				for(int j=0; j<users.size(); j++)
					if(content[i].toStdString()==users[j])
						users.erase(users.begin() + j);
				break;
			}
			notifications->setText("Can't find this name");
		}
	}
	
	lineEdit_Del->hide();
}

/*Prepare training dataset for background silence*/
void recognitionWidget::initializeSil(){
	for(int i=0; i<users.size(); i++)
		if(users[i] == "sil")
			return;

	notifications->setText("Loading...");
	notifications->repaint();
    users.push_back("sil");
    for(int i=i; i<=3; i++){
        string dirName = WAVdir + to_string(i) + "_" + "sil.wav";
        record(1.5, dirName, false);
    }
    notifications->setText(" ");
}

/*Begin testing/conference*/
void recognitionWidget::startTesting()
{
	/*Stop conference if conference is in progress*/
	if(isTesting){
		isTesting = false;
		btn_start->setText("Start Conference");
		btn_add->show();
		btn_del->show();
		return;
	}

	/*Start conference as usual*/
	btn_start->setText("Stop Conference");
	btn_add->hide();
	btn_del->hide();

	initializeSil();

    /*Train data of new users*/
    trainData();	

    /*Initialize testing thread*/
    Testing *testThread = new Testing;
    connect(testThread, &Testing::updateTestResult, this, &recognitionWidget::onUpdateTestResult);
    connect(testThread, &Testing::finished, testThread, &QObject::deleteLater);
    testThread->start();
    isTesting = true;
}

/*Constructor of main UI*/
recognitionWidget::recognitionWidget()
{


//Button..........................................
	btn_add = new QPushButton(this);
	btn_del = new QPushButton(this);
	btn_start = new QPushButton(this);
	
	btn_add->setText("Add User");
	btn_del->setText("Delete User");
	btn_start->setText("Start Conference");
	
	btn_add->setFont(QFont("Courier", 18, QFont::Bold));
	btn_del->setFont(QFont("Courier", 18, QFont::Bold));
	btn_start->setFont(QFont("Times", 18, QFont::Bold));
	
	btn_add->setGeometry(50,100,200,100);
	btn_del->setGeometry(550,100,200,100);
	btn_start->setGeometry(275,120,250,60);
	
	
//Label..........................................
	
	title = new QLabel(this);
	notifications = new QLabel(this);
	record_notifications = new QLabel(this);
	online = new QLabel(this);
	online2 = new QLabel(this);
	online_n[0] = new QLabel(this);
	online_n[1] = new QLabel(this);
	online_n[2] = new QLabel(this);
	online_n[3] = new QLabel(this);
	online_n[4] = new QLabel(this);

	title->setText("Speaker Recognition System");
	title->setFont(QFont("Courier",20, QFont::Bold));
	title->setGeometry(0,0,800,100);
	title->setAlignment(Qt::AlignCenter);
	notifications->setText(" ");
	notifications->setFont(QFont("Times",16, QFont::Bold));
	notifications->setGeometry(0,300,800,100);	
	notifications->setAlignment(Qt::AlignCenter);
	record_notifications->setText(" ");
	record_notifications->setFont(QFont("Times",16));
	record_notifications->setGeometry(0,350,800,100);	
	record_notifications->setAlignment(Qt::AlignCenter);
	online->setText("Now online: ");
	online->setFont(QFont("Courier",13, QFont::Bold));
	online->setGeometry(30,400,110,50);
	online2->setText(QString::number(people));
	online2->setFont(QFont("Courier",13, QFont::Bold));
	online2->setGeometry(150,400,20,50);
	
	online_n[0]->setText(" ");
	online_n[0]->setFont(QFont("Courier",13, QFont::Bold));
	online_n[0]->setGeometry(100,430,100,50);
	online_n[1]->setText(" ");
	online_n[1]->setFont(QFont("Courier",13, QFont::Bold));
	online_n[1]->setGeometry(200,430,100,50);
	online_n[2]->setText(" ");
	online_n[2]->setFont(QFont("Courier",13, QFont::Bold));
	online_n[2]->setGeometry(300,430,100,50);
	online_n[3]->setText(" ");
	online_n[3]->setFont(QFont("Courier",13, QFont::Bold));
	online_n[3]->setGeometry(400,430,100,50);
	online_n[4]->setText(" ");
	online_n[4]->setFont(QFont("Courier",13, QFont::Bold));
	online_n[4]->setGeometry(500,430,100,50);

	
//LineEdit.................................
    lineEdit_Add = new QLineEdit(this);
    lineEdit_Add->setGeometry(300,250,200,50);  
	lineEdit_Add->hide();
    lineEdit_Del = new QLineEdit(this);
    lineEdit_Del->setGeometry(300,250,200,50);  
	lineEdit_Del->hide();


// Connect..........................................
	QObject::connect(btn_add, SIGNAL(clicked()), this, SLOT(add()));
	QObject::connect(btn_del, SIGNAL(clicked()), this, SLOT(del()));
	QObject::connect(btn_start, SIGNAL(clicked()), this, SLOT(startTesting()));
	QObject::connect(lineEdit_Add,SIGNAL(returnPressed()), this,SLOT(ok()));
	QObject::connect(lineEdit_Del,SIGNAL(returnPressed()), this,SLOT(ok_del()));


}
