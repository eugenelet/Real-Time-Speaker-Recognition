
#include "speaker_detect.h"



int main(int argc, char *argv[]) {

//.......................................................
	QApplication app(argc,argv);
	recognitionWidget *widget = new recognitionWidget();

	widget->resize(800,480);
	widget->setWindowTitle("Interactive Project");
	widget->show();

	return app.exec();
}


