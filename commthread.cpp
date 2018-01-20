#include "commthread.h"
#include "dnathmi.h"

CommThread::CommThread(QObject *parent)
	: QThread(parent)
{
	m_pApp = (DNATHmi *)parent;

	m_bQuit = false;
	m_isQuit = true;
	m_sDevice = QString::null;
	m_iFeeder = -1;

	measure_test = 0.0;
	digital_test = true;
}

CommThread::~CommThread()
{
	ClearAllData();

	foreach (stuDev *dev, m_listDevice)
	{
		if (dev) 
			delete dev; 
	}
	m_listDevice.clear();

	foreach (stuDevCheck *dev, m_listDeviceCheck)
	{
		if (dev) 
			delete dev; 
	}
	m_listDeviceCheck.clear();
}

void CommThread::run()
{
	eDevRet ret;
	while (!m_bQuit)
	{
		m_isQuit = false;
		m_mutex.lock();
		switch (m_eCommand)
		{
		case CMD_EMPTY:
			break;
		case CMD_DEVLIST:
			CmdDevList();
			emit SigCmdDevList();
			break;
		case CMD_DEVCHECK:
			CmdDevCheck();
			emit SigCmdDevCheck();
			break;
		case CMD_FEEDER:
			CmdFeeder();
			emit SigCmdFeeder();
			break;
		case CMD_SET_ALLDATA:
			CmdSetAllData();
			emit SigCmdSetAllData();
			break;
		case CMD_REF_ALLDATA:
			CmdRefAllData();
			emit SigCmdRefAllData();
			break;
		case CMD_STRAP_READ:
			CmdStrapRead();
			emit SigCmdStrapRead();
			break;
		case CMD_OFFSTRAP_OFF:
			ret = CmdOffStarpOff();
			emit SigCmdOffStrapOff((qint32)ret);
			break;
		case CMD_OFFSTRAP_ON:
			ret = CmdOffStarpOn();
			emit SigCmdOffStrapOn((qint32)ret);
			break;
		case CMD_ONSTRAP_OFF:
			ret = CmdOnStarpOff();
			emit SigCmdOnStrapOff((qint32)ret);
			break;
		case CMD_ONSTRAP_ON:
			ret = CmdOnStarpOn();
			emit SigCmdOnStrapOn((qint32)ret);
			break;
		case CMD_OFFCTRL_SELECT:
			ret = CmdOffCtrlSelect();
			emit SigCmdOffCtrlSelect((qint32)ret);
			break;
		case CMD_OFFCTRL_EXEC:
			ret = CmdOffCtrlExec();
			emit SigCmdOffCtrlExec((qint32)ret);
			break;
		case CMD_OFFCTRL_CANCEL:
			ret = CmdOffCtrlCancel();
			emit SigCmdOffCtrlCancel((qint32)ret);
			break;
		case CMD_ONCTRL_SELECT:
			ret = CmdOnCtrlSelect();
			emit SigCmdOnCtrlSelect((qint32)ret);
			break;
		case CMD_ONCTRL_EXEC:
			ret = CmdOnCtrlExec();
			emit SigCmdOnCtrlExec((qint32)ret);
			break;
		case CMD_ONCTRL_CANCEL:
			ret = CmdOnCtrlCancel();
			emit SigCmdOnCtrlCancel((qint32)ret);
			break;
		case CMD_SIGNALRESET:
			ret = CmdSignalReset();
			emit SigCmdSignalReset((qint32)ret);
			break;
		}

		m_mutex.unlock();
		m_eCommand = CMD_EMPTY;
		QThread::msleep(200);
	}

	m_isQuit = true;
}

void ClearMeausre(stuMeausre *mea)
{
	if (mea)
	{
		delete mea;
		mea = NULL;
	}
}

void ClearDigital(stuDigital *dgt)
{
	if (dgt)
	{
		delete dgt;
		dgt = NULL;
	}
}

void CommThread::ClearAllData()
{
	foreach (stuMeausre *mea, m_listMeausreCol1)
		ClearMeausre(mea);
	foreach (stuMeausre *mea, m_listMeausreCol2)
		ClearMeausre(mea);
	foreach (stuMeausre *mea, m_listMeausreCol3)
		ClearMeausre(mea);
	foreach (stuMeausre *mea, m_listPowerCol1)
		ClearMeausre(mea);
	foreach (stuMeausre *mea, m_listPowerCol2)
		ClearMeausre(mea);
	foreach (stuDigital *dgt, m_listDigital)
		ClearDigital(dgt);
	foreach (stuSoe *soe, m_listSoe)
		delete soe;

	m_listFeeder.clear();
	m_listMeausreCol1.clear();
	m_listMeausreCol2.clear();
	m_listMeausreCol3.clear();
	m_listPowerCol1.clear();
	m_listPowerCol2.clear();
	m_listDigital.clear();
	m_listSoe.clear();
}

void CommThread::CmdDevList()
{
	//�˴���ȡ�豸�б����ݣ�����Ϊģ������
	for (int i = 0; i < 15; i++)
	{
		stuDev *dev = new stuDev(tr("F320000000000%1").arg(i),"���ս��ǿƼ��ɷ����޹�˾",tr("�����ն�%1").arg(i),"PACS-5612F",tr("192.168.1.%1").arg(i)); 
		m_listDevice.append(dev);
	}
}

void CommThread::CmdDevCheck()
{
	//�˴���ȡ�豸�����б����ݣ�����Ϊģ������
	stuDevCheck *dev;
	for (int i = 0; i < 15; i++)
	{
		if (i > 5)
			dev = new stuDevCheck(tr("F320000000000%1").arg(i),"���ս��ǿƼ��ɷ����޹�˾",tr("�����ն�%1").arg(i),"PACS-5612F",tr("192.168.1.%1").arg(i),CHK_NO); 
		else
			dev = new stuDevCheck(tr("F320000000000%1").arg(i),"���ս��ǿƼ��ɷ����޹�˾",tr("�����ն�%1").arg(i),"PACS-5612F",tr("192.168.1.%1").arg(i),CHK_YES); 
		
		m_listDeviceCheck.append(dev);
	}
}

void CommThread::CmdFeeder()
{
	//�˴���ȡ�����������Ƶ����ݣ�����Ϊģ������
	m_listFeeder.clear();
	for (int i = 1; i < 32; i++)
		m_listFeeder.append(tr("#%1 ����").arg(i));
}

void CommThread::CmdSetAllData()
{
	//�˴����ò��������ƺͳ�ʼ���ݣ����ߺż�m_iFeeder������Ϊģ������(��������ʽչʾ)
	stuMeausre *mea = new stuMeausre(0xf1c0,"Ia",0.00,"A");
	m_listMeausreCol1.append(mea);
	mea = new stuMeausre(0xf1c0,"Ib",0.00,"A");
	m_listMeausreCol1.append(mea);
	mea = new stuMeausre(0xf1c0,"Ic",0.00,"A");
	m_listMeausreCol1.append(mea);
	mea = new stuMeausre(0xf1c0,"Ua",0.00,"V");
	m_listMeausreCol1.append(mea);
	mea = new stuMeausre(0xf1c0,"Ub",0.00,"V");
	m_listMeausreCol1.append(mea);
	mea = new stuMeausre(0xf1c0,"Uc",0.00,"V");
	m_listMeausreCol1.append(mea);

	mea = new stuMeausre(0xf1c0,"A�ౣ������",0.00,"����");
	m_listMeausreCol2.append(mea);
	mea = new stuMeausre(0xf1c0,"B�ౣ������",0.00,"����");
	m_listMeausreCol2.append(mea);
	mea = new stuMeausre(0xf1c0,"C�ౣ������",0.00,"����");
	m_listMeausreCol2.append(mea);
	mea = new stuMeausre(0xf1c0,"A�ౣ����ѹ",0.00,"����");
	m_listMeausreCol2.append(mea);
	mea = new stuMeausre(0xf1c0,"B�ౣ����ѹ",0.00,"����");
	m_listMeausreCol2.append(mea);
	mea = new stuMeausre(0xf1c0,"C�ౣ����ѹ",0.00,"����");
	m_listMeausreCol2.append(mea);

	mea = new stuMeausre(0xf1c0,"Ƶ��",0.00,"����");
	m_listMeausreCol3.append(mea);
	mea = new stuMeausre(0xf1c0,"�¶�",0.00,"���϶�");
	m_listMeausreCol3.append(mea);

	stuMeausre *power = new stuMeausre(0xf1c0,"�����й�1",0.00,"KW");
	m_listPowerCol1.append(power);
	power = new stuMeausre(0xf1c0,"�����й�1",0.00,"KVar");
	m_listPowerCol1.append(power);

	power = new stuMeausre(0xf1c0,"�����й�2",0.00,"KW");
	m_listPowerCol2.append(power);
	power = new stuMeausre(0xf1c0,"�����й�2",0.00,"KVar");
	m_listPowerCol2.append(power);

	for (int i = 0; i < 30; i++)
	{
		stuDigital *dgt;
		dgt = new stuDigital(LED_GREED,tr("ң��״̬�ź� %1").arg(i));
		m_listDigital.append(dgt);
	}
}


void CommThread::CmdRefAllData()
{
	measure_test += 1.0;
	digital_test = !digital_test;

	foreach (stuMeausre *mea, m_listMeausreCol1)
		mea->value = measure_test;
	foreach (stuMeausre *mea, m_listMeausreCol2)
		mea->value = measure_test;
	foreach (stuMeausre *mea, m_listMeausreCol3)
		mea->value = measure_test;

	foreach (stuMeausre *mea, m_listPowerCol1)
		mea->value = measure_test;
	foreach (stuMeausre *mea, m_listPowerCol2)
		mea->value = measure_test;

	foreach (stuDigital *dgt, m_listDigital)
		digital_test == true ? dgt->state = LED_GREED : dgt->state = LED_RED;

	QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	stuSoe *soe = new stuSoe(time,"����SOE","����");
	m_listSoe.insert(0,soe);
}

void CommThread::CmdStrapRead()
{
	//�˴����зֺ�բ��ѹ�壬Զ��/�͵أ�Ӳѹ��״̬��ȡ
	m_bOffStrap = true;
	m_bOnStrap = false;
	m_bRemoteLocate = false;
	m_bHardStrap = true;
}

eDevRet CommThread::CmdOffStarpOff()
{
	eDevRet ret = RET_SUCCESS;

	//�˴����з�բ��ѹ��,�˳�����,����Ϊģ������
	m_bOffStrap = false;
	ret = RET_SUCCESS;
	
	return ret;
}

eDevRet CommThread::CmdOffStarpOn()
{
	eDevRet ret = RET_SUCCESS;

	//�˴����з�բ��ѹ��,Ͷ�����,����Ϊģ������
	ret = RET_TIMEOUT;
	QThread::sleep(5);

	//m_bOffStrap = true;
	//ret = RET_SUCCESS;

	return ret;
}

eDevRet CommThread::CmdOnStarpOff()
{
	eDevRet ret = RET_SUCCESS;

	//�˴����к�բ��ѹ��,�˳�����,����Ϊģ������
	ret = RET_FAILED;

	//m_bOnStrap = false;
	//ret = RET_SUCCESS;

	return ret;
}

eDevRet CommThread::CmdOnStarpOn()
{
	eDevRet ret = RET_SUCCESS;

	//�˴����к�բ��ѹ��,Ͷ�����,����Ϊģ������
	m_bOnStrap = true;
	ret = RET_SUCCESS;

	return ret;
}

eDevRet CommThread::CmdOffCtrlSelect()
{
	eDevRet ret = RET_SUCCESS;

	//�˴�����ң�ط�բ,ѡ�����,����Ϊģ������
	ret = RET_SUCCESS;

	return ret;
}

eDevRet CommThread::CmdOffCtrlExec()
{
	eDevRet ret = RET_SUCCESS;

	//�˴�����ң�ط�բ,ִ�в���,����Ϊģ������
	ret = RET_SUCCESS;

	return ret;
}

eDevRet CommThread::CmdOffCtrlCancel()
{
	eDevRet ret = RET_SUCCESS;

	//�˴�����ң�ط�բ,ȡ������,����Ϊģ������
	ret = RET_SUCCESS;

	return ret;
}

eDevRet CommThread::CmdOnCtrlSelect()
{
	eDevRet ret = RET_SUCCESS;

	//�˴�����ң�غ�բ,ѡ�����,����Ϊģ������
	ret = RET_SUCCESS;

	return ret;
}

eDevRet CommThread::CmdOnCtrlExec()
{
	eDevRet ret = RET_SUCCESS;

	//�˴�����ң�غ�բ,ִ�в���,����Ϊģ������
	ret = RET_TIMEOUT;
	QThread::sleep(5);

	return ret;
}

eDevRet CommThread::CmdOnCtrlCancel()
{
	eDevRet ret = RET_SUCCESS;

	//�˴�����ң�غ�բ,ȡ������,����Ϊģ������
	ret = RET_FAILED;

	return ret;
}

eDevRet CommThread::CmdSignalReset()
{
	eDevRet ret = RET_SUCCESS;;

	//�˴������źŸ���������ȴ�������Ϣ������Ϊ������ʱ
	ret = RET_TIMEOUT;
	QThread::sleep(5);

	return ret;
}
