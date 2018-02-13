#include "cdevlistcheck.h"
#include "dnathmi.h"

CDevListCheck::CDevListCheck(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_pApp = (DNATHmi *)parent;

	Init();
	InitUi();
	InitSlot();

	SetDevices();
}

CDevListCheck::~CDevListCheck()
{

}

#define COLUMN_ID			0
#define COLUMN_MANUFACTURE	1
#define COLUMN_NAME			2
#define COLUMN_TYPE			3
#define COLUMN_IPADDR		4
#define COLUMN_CHECK		5
void CDevListCheck::Init()
{
	QStringList table_header;
	table_header << tr("ID") << tr("Manufacture") << tr("Name") << tr("Type") << tr("IP Address") << tr("Check State");
	ui.tableWidget_dev->setColumnCount(6);
	ui.tableWidget_dev->setHorizontalHeaderLabels(table_header);
	ui.tableWidget_dev->setColumnWidth(COLUMN_ID,250);
	ui.tableWidget_dev->setColumnWidth(COLUMN_MANUFACTURE,250);
	ui.tableWidget_dev->setColumnWidth(COLUMN_NAME,250);
	ui.tableWidget_dev->setColumnWidth(COLUMN_TYPE,200);
	ui.tableWidget_dev->setColumnWidth(COLUMN_IPADDR,200);
	ui.tableWidget_dev->setColumnWidth(COLUMN_CHECK,100);
	ui.tableWidget_dev->setSelectionBehavior(QAbstractItemView::SelectRows);		//����ѡ��ģʽ
	ui.tableWidget_dev->setFocusPolicy(Qt::NoFocus);								//ȥ�����㣬�����
	ui.tableWidget_dev->verticalHeader()->setDefaultSectionSize(35);				//�����и�
	ui.tableWidget_dev->verticalHeader()->setVisible(false);						//ȥ����ǰ��
	ui.tableWidget_dev->horizontalHeader()->setVisible(false);						//ȥ����ͷ
	ui.tableWidget_dev->setFrameShape(QFrame::NoFrame);								//�����ޱ߿�
	ui.tableWidget_dev->horizontalHeader()->setStretchLastSection(true);			//���ó�������
	ui.tableWidget_dev->horizontalHeader()->setHighlightSections(false);			//�����ʱ���Ա�ͷ�й���
	ui.tableWidget_dev->setShowGrid(false);											//���ò���ʾ������
	ui.tableWidget_dev->setSelectionMode(QAbstractItemView::ExtendedSelection);		//�ɶ�ѡ��Ctrl��Shift��  Ctrl+A�����ԣ�
	ui.tableWidget_dev->setStyleSheet("selection-background-color:lightblue;");
	ui.tableWidget_dev->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#2D9191;color:#ffffff}"); //���ñ�ͷ����ɫ
	//ui.tableWidget_dev->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//����ˮƽ����ֱ��������ʽ
	ui.tableWidget_dev->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:15px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	ui.tableWidget_dev->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 15px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
}

void CDevListCheck::InitUi()
{
	QStringList qss;
	qss.append(QString("QWidget#widget_main{background-color:rgb(255,255,255,150);}"));
	qss.append(QString("QTableWidget#tableWidget_dev{background:transparent;}"));
	this->setStyleSheet(qss.join(""));
}

void CDevListCheck::InitSlot()
{
	connect(m_pApp->m_pCommThread, SIGNAL(SigCmdDevCheck()), this, SLOT(SlotCmdDevCheck()));
}

void CDevListCheck::SlotCmdDevCheck()
{
	m_pApp->m_pCommThread->m_mutex.lock();
	ShowDevices();
	m_pApp->m_pCommThread->m_mutex.unlock();
}

void CDevListCheck::SetDevices()
{
	foreach (stuDevCheck *dev, m_pApp->m_pCommThread->m_listDeviceCheck)
		delete dev;
	m_pApp->m_pCommThread->m_listDeviceCheck.clear();

	m_pApp->m_pCommThread->m_mutex.lock();
	m_pApp->m_pCommThread->SetCommand(CMD_DEVCHECK);
	m_pApp->m_pCommThread->m_mutex.unlock();
}

void CDevListCheck::ShowDevices()
{
	ui.tableWidget_dev->clearContents();
	ui.tableWidget_dev->setRowCount(m_pApp->m_pCommThread->m_listDeviceCheck.count());

	QFont m_font;
	m_font.setFamily("Microsoft YaHei");
	m_font.setPixelSize(16);
	m_font.setBold(false);

	int row = 0;
	QTableWidgetItem *item;
	foreach (stuDevCheck *dev, m_pApp->m_pCommThread->m_listDeviceCheck)
	{
		item = new QTableWidgetItem(dev->id);
		//item->setBackgroundColor(dev->check == CHK_YES ? QColor(127,184,14,100) : QColor(217,58,73,100));
		item->setTextColor(dev->check == CHK_YES ? QColor("#1d953f") : QColor("#ed1941"));
		item->setTextAlignment(Qt::AlignCenter);
		item->setFont(m_font);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget_dev->setItem(row,COLUMN_ID,item);

		item = new QTableWidgetItem(dev->manufacture);
		//item->setBackgroundColor(dev->check == CHK_YES ? QColor(127,184,14,100) : QColor(217,58,73,100));
		item->setTextColor(dev->check == CHK_YES ? QColor("#1d953f") : QColor("#ed1941"));
		item->setTextAlignment(Qt::AlignCenter);
		item->setFont(m_font);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget_dev->setItem(row,COLUMN_MANUFACTURE,item);

		item = new QTableWidgetItem(dev->name);
		//item->setBackgroundColor(dev->check == CHK_YES ? QColor(127,184,14,100) : QColor(217,58,73,100));
		item->setTextColor(dev->check == CHK_YES ? QColor("#1d953f") : QColor("#ed1941"));
		item->setTextAlignment(Qt::AlignCenter);
		item->setFont(m_font);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget_dev->setItem(row,COLUMN_NAME,item);

		item = new QTableWidgetItem(dev->type);
		//item->setBackgroundColor(dev->check == CHK_YES ? QColor(127,184,14,100) : QColor(217,58,73,100));
		item->setTextColor(dev->check == CHK_YES ? QColor("#1d953f") : QColor("#ed1941"));
		item->setTextAlignment(Qt::AlignCenter);
		item->setFont(m_font);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget_dev->setItem(row,COLUMN_TYPE,item);

		item = new QTableWidgetItem(dev->ip);
		//item->setBackgroundColor(dev->check == CHK_YES ? QColor(127,184,14,100) : QColor(217,58,73,100));
		item->setTextColor(dev->check == CHK_YES ? QColor("#1d953f") : QColor("#ed1941"));
		item->setTextAlignment(Qt::AlignCenter);
		item->setFont(m_font);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget_dev->setItem(row,COLUMN_IPADDR,item);

		item = new QTableWidgetItem(dev->check == CHK_YES ? tr("Checked") : tr("UnCheck"));
		//item->setBackgroundColor(dev->check == CHK_YES ? QColor(127,184,14,100) : QColor(217,58,73,100));
		item->setTextColor(dev->check == CHK_YES ? QColor("#1d953f") : QColor("#ed1941"));
		item->setTextAlignment(Qt::AlignCenter);
		item->setFont(m_font);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget_dev->setItem(row,COLUMN_CHECK,item);

		row ++;
	}
}
