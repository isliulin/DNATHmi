#include "msgbox.h"
#include <QMutex>
#include "iconhelper.h"

#define topIcoWidth		18
#define topTitleSize	18
#define topBtnWidth		45
#define charSize		20
#define iconSize		40
#define iconWidth		40
#define iconHeight		40
#define btnHeight		40

MsgBox *MsgBox::self = 0;
MsgBox *MsgBox::Instance()
{
	if (!self) 
	{
		QMutex mutex;
		QMutexLocker locker(&mutex);
		if (!self) 
		{
			self = new MsgBox;
		}
	}

	return self;
}

void MsgBox::DestoryInstance()
{
	if (self != NULL) 
	{
		delete self;
		self = NULL;
	}
}

MsgBox::MsgBox(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	Init();
	InitUi();
	InitSlot();
}

MsgBox::~MsgBox()
{
	
}

void MsgBox::Init()
{
	QStringList qss;
	qss.append(QString("*{outline:0px;}"));
	qss.append(QString("QWidget#widget_main{background-color:#f2eada;border:1px solid #242424;}"));
	qss.append(QString("QLabel#label_title{font:%1px;color:#FEFEFE;}").arg(topTitleSize));
	qss.append(QString("QLabel#label_text{font:%1px;color:#383838;}").arg(charSize));
	qss.append(QString("QWidget#widget_top QAbstractButton{background:none;border:none;min-width:%1px;max-width:%1px;color:#FEFEFE;}").arg(topBtnWidth));
	this->setStyleSheet(qss.join(""));
}

void MsgBox::InitUi()
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	this->setFixedSize(500, 400);
	ui.widget_top->setFixedHeight(40);

	move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height())/2);
}

void MsgBox::InitSlot()
{
	connect(ui.btnYes, SIGNAL(clicked()), this, SLOT(SlotYes()));
	connect(ui.btnNo, SIGNAL(clicked()), this, SLOT(SlotNo()));
}

void MsgBox::information(QString text)
{
	ui.label_title->setText(tr("Information"));
	ui.label_text->setText(text);
	ui.widget_top->setStyleSheet(QString("QWidget#widget_top"
		"{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}").arg("#00688B").arg("#093746"));

	QPixmap iconNormal = IconHelper::Instance()->getPixmap(QColor(255,255,255).name(), 0xf00c, iconSize, iconWidth, iconHeight);
	ui.btnYes->setText(tr("  Affirm"));
	ui.btnYes->setFixedHeight(btnHeight);
	ui.btnYes->setIcon(QIcon(iconNormal));
	ui.btnYes->setStyleSheet(QString("QPushButton#btnYes{border:1px solid #242424;border-radius:5px;color:#DCDCDC;padding:8px;"
		"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #1d953f,stop:1 #1d953f);font:%1px;}").arg(charSize));

	ui.btnYes->setVisible(true);
	ui.btnNo->setVisible(false);
	exec();
}

void MsgBox::warning(QString text)
{
	ui.label_title->setText(tr("Warning"));
	ui.label_text->setText(text);
	ui.widget_top->setStyleSheet(QString("QWidget#widget_top"
		"{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}").arg("#c37e00").arg("#c88400"));

	QPixmap iconNormal = IconHelper::Instance()->getPixmap(QColor(255,255,255).name(), 0xf00c, iconSize, iconWidth, iconHeight);
	ui.btnYes->setText(tr("  Affirm"));
	ui.btnYes->setFixedHeight(btnHeight);
	ui.btnYes->setIcon(QIcon(iconNormal));
	ui.btnYes->setStyleSheet(QString("QPushButton#btnYes{border:1px solid #242424;border-radius:5px;color:#DCDCDC;padding:8px;"
		"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #1d953f,stop:1 #1d953f);font:%1px;}").arg(charSize));

	ui.btnYes->setVisible(true);
	ui.btnNo->setVisible(false);
	exec();
}

void MsgBox::critical(QString text)
{
	ui.label_title->setText(tr("Critical"));
	ui.label_text->setText(text);
	ui.widget_top->setStyleSheet(QString("QWidget#widget_top"
		"{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}").arg("#ef4136").arg("#c63c26"));

	QPixmap iconNormal = IconHelper::Instance()->getPixmap(QColor(255,255,255).name(), 0xf00c, iconSize, iconWidth, iconHeight);
	ui.btnYes->setText(tr("  Affirm"));
	ui.btnYes->setFixedHeight(btnHeight);
	ui.btnYes->setIcon(QIcon(iconNormal));
	ui.btnYes->setStyleSheet(QString("QPushButton#btnYes{border:1px solid #242424;border-radius:5px;color:#DCDCDC;padding:8px;"
		"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #1d953f,stop:1 #1d953f);font:%1px;}").arg(charSize));

	ui.btnYes->setVisible(true);
	ui.btnNo->setVisible(false);
	exec();
}

int MsgBox::question(QString text)
{
	ui.label_title->setText(tr("Question"));
	ui.label_text->setText(text);
	ui.widget_top->setStyleSheet(QString("QWidget#widget_top"
		"{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}").arg("#00688B").arg("#093746"));

	QPixmap iconNormal = IconHelper::Instance()->getPixmap(QColor(255,255,255).name(), 0xf00c, iconSize, iconWidth, iconHeight);
	ui.btnYes->setText(tr("  Yes"));
	ui.btnYes->setFixedHeight(btnHeight);
	ui.btnYes->setIcon(QIcon(iconNormal));
	ui.btnYes->setStyleSheet(QString("QPushButton#btnYes{border:1px solid #242424;border-radius:5px;color:#DCDCDC;padding:8px;"
		"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #1d953f,stop:1 #1d953f);font:%1px;}").arg(charSize));

	iconNormal = IconHelper::Instance()->getPixmap(QColor(255,255,255).name(), 0xf00d, iconSize, iconWidth, iconHeight);
	ui.btnNo->setText(tr("  No"));
	ui.btnNo->setFixedHeight(btnHeight);
	ui.btnNo->setIcon(QIcon(iconNormal));
	ui.btnNo->setStyleSheet(QString("QPushButton#btnNo{border:1px solid #242424;border-radius:5px;color:#DCDCDC;padding:8px;"
		"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #3e4145,stop:1 #3e4145);font:%1px;}").arg(charSize));

	ui.btnYes->setVisible(true);
	ui.btnNo->setVisible(true);

	int ret = exec();
	if (ret == Accepted)
		return RET_YES;
	else if (ret == Rejected)
		return RET_NO;

	return -1;
}

void MsgBox::SlotYes()
{
	accept();
}

void MsgBox::SlotNo()
{
	reject();
}