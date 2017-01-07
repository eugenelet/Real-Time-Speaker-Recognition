#include <QTextEdit>
#include <QString>
#include <QLineEdit>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QPushButton>
#include <QFont>
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QImage>
#include <QLabel>
#include <QThread>
#include <QApplication>

#include <AL/al.h> 
#include <AL/alc.h> 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string> 
#include <iostream>
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


/*Testing Thread*/
class Testing : public QThread {
	Q_OBJECT
	void run() Q_DECL_OVERRIDE;
signals:
    void updateTestResult(QString info);
};

/*Main UI*/
class recognitionWidget: public QWidget{
	Q_OBJECT
	QThread testingThread;
     	public:
	       recognitionWidget();


	 public slots:
		void add();
		void del();
		void startTesting();
		void ok();
		void ok_del();
		void initializeSil();
		void onUpdateTestResult(QString info);
			
	private:
		int m=0,n,available[5]={1,1,1,1,1};
		QTimer *timer;
		QPushButton	*btn_add,
					*btn_del,
					*btn_start,
					*btn_ok,
					*btn_ok_del,
					*btn_record;
		QLabel		*title,
					*online, 
					*online2,
					*online_n[5];
		QLineEdit *lineEdit_Add;
		QLineEdit *lineEdit_Del;
		QString content[5],temp;
};
