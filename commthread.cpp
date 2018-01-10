#include "commthread.h"
#include "dnathmi.h"

CommThread::CommThread(QObject *parent)
	: QThread(parent)
{
	m_pApp = (DNATHmi *)parent;

	m_bQuit = false;
	m_sDevice = QString::null;
	m_iFeeder = -1;

	measure_test = 0.0;
	digital_test = true;
}

CommThread::~CommThread()
{
	ClearAllData();
}

void CommThread::run()
{
	eDevRet ret;
	while (!m_bQuit)
	{
		m_mutex.lock();
		switch (m_eCommand)
		{
		case CMD_EMPTY:
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
		case CMD_REMOTE_CONTROL:
			ret = CmdRemoteControl();
			emit SigCmdRmoteControl((qint32)ret);
			break;
		case CMD_SOFT_STRAP:
			ret = CmdSoftStrap();
			emit SigCmdSoftStrap((qint32)ret);
			break;
		case CMD_HARD_STRAP:
			ret = CmdHardStrap();
			emit SigCmdHardStrap((qint32)ret);
			break;
		case CMD_DISTANT_LOCAL:
			ret = CmdDistantLocal();
			emit SigCmdDistantLocal((qint32)ret);
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

	m_listMeausreCol1.clear();
	m_listMeausreCol2.clear();
	m_listMeausreCol3.clear();
	m_listPowerCol1.clear();
	m_listPowerCol2.clear();
	m_listDigital.clear();
	m_listSoe.clear();
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

eDevRet CommThread::CmdRemoteControl()
{
	eDevRet ret = RET_SUCCESS;;

	//�˴�����ң�ز������ȴ�������Ϣ������Ϊ�����ɹ�
	ret = RET_SUCCESS;

	return ret;
}

eDevRet CommThread::CmdSoftStrap()
{
	eDevRet ret = RET_SUCCESS;;

	//�˴�������ѹ��������ȴ�������Ϣ������Ϊ����ʧ��
	ret = RET_FAILED;

	return ret;
}

eDevRet CommThread::CmdHardStrap()
{
	eDevRet ret = RET_SUCCESS;;

	//�˴�����Ӳѹ��������ȴ�������Ϣ������Ϊ������ʱ
	ret = RET_TIMEOUT;
	QThread::sleep(5);

	return ret;
}

eDevRet CommThread::CmdDistantLocal()
{
	eDevRet ret = RET_SUCCESS;;

	//�˴�����Զ���͵ز������ȴ�������Ϣ������Ϊ������ʱ
	ret = RET_TIMEOUT;
	QThread::sleep(5);

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
