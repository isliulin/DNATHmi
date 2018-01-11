#include "qrthread.h"
#include "dnathmi.h"

QRThread::QRThread(QObject *parent)
	: QThread(parent)
{
	m_pApp = (DNATHmi *)parent;
	m_bQuit = false;
}

QRThread::~QRThread()
{

}

void QRThread::run()
{
	m_sDecode = QString::null;
	m_frame.data = NULL; //�������ʱ���ε�¼
	while(!m_bQuit)
	{
		if(!m_frame.data) 
		{
			QThread::sleep(1); //ÿ1����һ������ͼ��
			continue;
		}

		IplImage imgTmp = m_frame;
		IplImage *srcImage = cvCloneImage(&imgTmp);
		m_frame.data = 0;
		IplImage *Grayimage = cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_8U, 1); //ת��Ϊ�Ҷ�ͼ
		cvCvtColor(srcImage,Grayimage,CV_BGR2GRAY);

		cv::Mat im = cv::cvarrToMat(Grayimage); //����ͼƬ����Ϊ�Ҷ�ͼ
		std::string ret = GetQR(im);
		m_sDecode = Utf8ToGb2312(ret);
		if(!m_sDecode.isEmpty())
		{
			cvReleaseImage(&srcImage);  
			cvReleaseImage(&Grayimage);
			break;
		}

		cvReleaseImage(&srcImage);  
		cvReleaseImage(&Grayimage);

		QThread::msleep(500); //����CPU����
	}
}

QString QRThread::Utf8ToGb2312(std::string &strUtf8)
{
	QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
	QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

	QString strUnicode=utf8Codec->toUnicode(strUtf8.c_str());
	QByteArray ByteGb2312=gb2312Codec->fromUnicode(strUnicode);

	return ByteGb2312.data();
}

string QRThread::GetQR(cv::Mat img)
{
	cv::Mat binImg;

	int thre = threshold(img, binImg, 0, 255, cv::THRESH_OTSU); //��otsu��ֵ����Ļ����ϣ�����������ֵ������ʶ��ģ��ͼ��
	string result;
	while(result.empty() && thre < 255)
	{
		threshold(img, binImg, thre, 255, cv::THRESH_BINARY);
		result = GetQRInBinImg(binImg);
		thre += 20; //��ֵ������Ϊ20������Խ��ʶ����Խ�ͣ��ٶ�Խ��
	}

	return result;
}

string QRThread::GetQRInBinImg(cv::Mat binImg)
{
	//�Զ�ֵͼ�����ʶ�����ʧ����������ж���ʶ��
	string result = ZbarDecoder(binImg);
	if (result.empty())
	{
		cv::Mat openImg;
		cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		morphologyEx(binImg, openImg, cv::MORPH_OPEN, element);
		result = ZbarDecoder(openImg);
	}

	return result;
}

string QRThread::ZbarDecoder(cv::Mat img)
{
	string result;
	zbar::ImageScanner scanner;
	const void *raw = (&img)->data;

	scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1); // configure the reader
	zbar::Image image(img.cols, img.rows, "Y800", raw, img.cols * img.rows); // wrap image data
	int n = scanner.scan(image); // scan the image for barcodes
	result = image.symbol_begin()->get_data(); // extract results
	image.set_data(NULL, 0);

	return result;
}
