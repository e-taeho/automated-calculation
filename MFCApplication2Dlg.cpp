// MFCApplication2Dlg.cpp : ���� ����
//
#pragma warning(disable:4819)


#include "stdafx.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include "CvvIamge.h"
#include "BlobLabeling.h"


#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

#define _DEF_WEBCAM 1000

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)	
{

}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMFCApplication2Dlg ��ȭ ����

CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication2Dlg::IDD, pParent)
	, m_Preprocess(FALSE)	//��ó��
	, m_Labeling(FALSE)		//���̺� 
	, m_bDeNoise(FALSE)		//��Ī
	, m_operation(FALSE)	//����
	, m_pImage(0)
	, m_pCapture(0)
	//�簢��ũ�� �ʱ⼳��
	, widthvalue(270)
	, heightvalue(120)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_DISPLAY, m_stDisplay);
	DDX_Control(pDX, IDC_ST_DISPLAY2, m_stDisplay2);
	DDX_Check(pDX, IDC_BUTTON2, m_bDeNoise);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication2Dlg::OnBnClickedChkErode)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication2Dlg::OnBnClickedChkDilate)

	ON_BN_CLICKED(IDC_BUTTON_height_M, &CMFCApplication2Dlg::OnClickedButtonHeightMinus)
	ON_BN_CLICKED(IDC_BUTTON_height_P, &CMFCApplication2Dlg::OnClickedButtonHeightPlus)
	ON_BN_CLICKED(IDC_BUTTON_width_P, &CMFCApplication2Dlg::OnClickedButtonWidthPlus)
	ON_BN_CLICKED(IDC_BUTTON_width_M, &CMFCApplication2Dlg::OnClickedButtonWidthMinus)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication2Dlg::OnBnClickedButton2)
	ON_COMMAND(ID_Fileopen, &CMFCApplication2Dlg::OnFileopen)
	ON_COMMAND(ID_OnCam, &CMFCApplication2Dlg::OnOncam)
	ON_COMMAND(ID_OffCam, &CMFCApplication2Dlg::OnOffcam)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication2Dlg::OnBnClickedButton7)
END_MESSAGE_MAP()
// CMFCApplication2Dlg �޽��� ó����

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
 

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	
	//ħ�� ��â ī��Ʈ �ʱ�ȭ
	Erodecount = 0;
	Dilatecount = 0;

	//ħ��,��â ó���� ���� �̹���


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}
void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if( m_pImage )
		{
			// ���� �̹���
			CDC*	pDC;
			CRect	rect;

			pDC = m_stDisplay.GetDC();
			m_stDisplay.GetClientRect(&rect);

			m_cImage.CopyOf(m_pImage);
			m_cImage.DrawToHDC(pDC->m_hDC, rect);

			ReleaseDC(pDC);
		}


		CDialogEx::OnPaint();
	}
}
// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CMFCApplication2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	DWORD dwStartTime = timeGetTime();
	if( nIDEvent == _DEF_WEBCAM )
	{
		if( m_pCapture )	m_pImage = cvQueryFrame( m_pCapture );
		

		Upload load = Upload();
		IplImage* binaryImage = cvCreateImage( cvGetSize(m_pImage), 8, 1 );

		//��ó�� �Լ�
		if(m_Preprocess)
		Binary(m_pImage, binaryImage); 

		cx = binaryImage->width/2;
		cy = binaryImage->height/2;
		CvPoint pt1 = cvPoint( cx - widthvalue, cy - heightvalue );
		CvPoint pt2 = cvPoint( cx + widthvalue, cy + heightvalue );
		CvScalar red   = cvScalar( 0, 0, 255 );
		cvDrawRect( m_pImage, pt1, pt2, red, 2 );

		int rx = cx - widthvalue;
		int ry = cy - heightvalue;
		int rw = widthvalue * 2;
		int rh = heightvalue *2;
		cvSetImageROI(binaryImage, cvRect(rx, ry, rw, rh));

		IplImage* labelingImage = cvCreateImage( cvSize(binaryImage->roi->width, binaryImage->roi->height), 8, 3);	
		IplImage* outcome = cvCreateImage(cvSize(binaryImage->roi->width, binaryImage->roi->height),IPL_DEPTH_8U,1);
		IplImage* img;

		//ħ��, ��â�Լ� (��ư)
		ED(binaryImage,m_bErode,m_bDilate); 

		//������������ �簢�� ����
		img = (IplImage*)cvClone(binaryImage);
		cvSetImageROI(img,cvRect(binaryImage->roi->xOffset,
			binaryImage->roi->yOffset, binaryImage->roi->width, binaryImage->roi->height) );
		cvResize(img, outcome, CV_INTER_NN);			
		cvResetImageROI(img);

		//���̺�
		if(m_Labeling)
		labelingImage = load.Labeling(outcome, labelingImage, 8, m_bDeNoise);
		if(m_operation)
		load.Operation_Run(m_pImage, rx, ry, rw, rh);


		if(m_Labeling)
		DrawCDC(labelingImage);
		else
		DrawCDC(binaryImage);

		//�޸� ����
		cvReleaseImage(&labelingImage);
		cvReleaseImage(&outcome);
		cvReleaseImage(&img);
		cvReleaseImage(&binaryImage);

		load.~Upload();

		Invalidate(FALSE);

		//���� ���� +, - ��ư ����, ���� ���� ���� Ȱ��ȭ
		if(heightvalue < 220)
			GetDlgItem(IDC_BUTTON_height_P)->EnableWindow(TRUE);
		if(heightvalue > 30)
			GetDlgItem(IDC_BUTTON_height_M)->EnableWindow(TRUE);
		if(widthvalue < 280)
			GetDlgItem(IDC_BUTTON_width_P)->EnableWindow(TRUE);
		if(widthvalue > 30)
			GetDlgItem(IDC_BUTTON_width_M)->EnableWindow(TRUE);

		CDialog::OnTimer(nIDEvent);
		
	}
	DWORD dwEndTime = timeGetTime();


	DWORD abc = dwEndTime - dwStartTime;
	
}
//�ݱ�
BOOL CMFCApplication2Dlg::DestroyWindow()
{
	KillTimer( _DEF_WEBCAM );
	m_pCapture = cvCreateCameraCapture(3);
	if(m_pCapture)
	{
		cvReleaseCapture(&m_pCapture);
	}
	
	cvDestroyAllWindows();
	return CDialog::DestroyWindow();
}
//ħ�� ��ư
void CMFCApplication2Dlg::OnBnClickedChkErode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bErode = TRUE;
	UpdateData(FALSE);
	Erodecount++;
}
//��â ��ư
void CMFCApplication2Dlg::OnBnClickedChkDilate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bDilate = TRUE;
	UpdateData(FALSE);
	Dilatecount++;
}
//DC�� �׸���
void CMFCApplication2Dlg ::DrawCDC(IplImage* Image)
{	
	CDC*	pDC;
	CRect	rect;
	pDC = m_stDisplay2.GetDC();
	m_stDisplay2.GetClientRect(&rect);
	m_cImage.CopyOf(Image);
	m_cImage.DrawToHDC(pDC->m_hDC, rect);
	ReleaseDC(pDC);

}
//��ó��
void CMFCApplication2Dlg ::Binary(IplImage* m_pImage,IplImage* binaryImage)
{
	cvCvtColor( m_pImage, binaryImage, CV_BGR2GRAY);
	cvThreshold( binaryImage, binaryImage, 70, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvErode(binaryImage, binaryImage, NULL, 2);
}
//ħ�� ��â
void CMFCApplication2Dlg ::ED(IplImage* Image, BOOL Erode, BOOL Dilate)
{
	if(Erode=TRUE)//ħ�� 
	{
		cvDilate(Image,Image,NULL,Erodecount);
	}
	if(Dilate=TRUE)//��â
	{
		cvErode(Image,Image,NULL,Dilatecount);
	}
}
//���� + ��ư
void CMFCApplication2Dlg::OnClickedButtonHeightPlus()
{
	if(heightvalue > 200)
	{
		AfxMessageBox(_T("�ִ� ũ���Դϴ�"));
		GetDlgItem(IDC_BUTTON_height_P)->EnableWindow(FALSE);
	}
	else
		heightvalue+=25;
}
//���� - ��ư
void CMFCApplication2Dlg::OnClickedButtonHeightMinus()
{
	if(heightvalue < 30)
	{
		AfxMessageBox(_T("�ּ� ũ���Դϴ�"));
		GetDlgItem(IDC_BUTTON_height_M)->EnableWindow(FALSE);
	}
	else
		heightvalue-=25;
}
//���� + ��ư
void CMFCApplication2Dlg::OnClickedButtonWidthPlus()
{
	if(widthvalue > 275)
	{
		AfxMessageBox(_T("�ִ� ũ���Դϴ�"));
		GetDlgItem(IDC_BUTTON_width_P)->EnableWindow(FALSE);
	}
	else
		widthvalue+=25;
}
//���� - ��ư
void CMFCApplication2Dlg::OnClickedButtonWidthMinus()
{
	if(widthvalue < 30)
	{
		AfxMessageBox(_T("�ּ� ũ���Դϴ�"));
		GetDlgItem(IDC_BUTTON_width_M)->EnableWindow(FALSE);
	}
	else
		widthvalue-=25;
}

//��Ī
void CMFCApplication2Dlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_bDeNoise = !m_bDeNoise;
	if(m_bDeNoise)
		GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
	else
	{
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
		m_operation = FALSE;	//����
	}
	UpdateData(FALSE);
}
//�̹����� ��θ� �ҷ������Լ�
void CMFCApplication2Dlg::OnFileopen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	char szFilter[] = "Image (*.bmp, *.gif, *.jpg, *.PNG) | *.BMP;*.GIF;*.JPG;*.PNG | All Files(*.*)|*.*||";
	CStringW filter(szFilter);
	CFileDialog open(TRUE,NULL,NULL,OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT,filter);

	if(IDOK == open.DoModal())
	{	
		CString filename = open.GetPathName();
		loadimage(filename);	
		Invalidate(FALSE);
	}
	
}
//�̹����� ����
void CMFCApplication2Dlg::loadimage(CString filename)
{
	if(m_pImage != NULL) cvReleaseImage(&m_pImage);
	KillTimer(_DEF_WEBCAM);

	char * str;

	int sLen = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);

	// ��ȯ�� ���ڿ��� ����� ������ �����Ҵ���
	str=new char[sLen+1];

	// ���ڿ� ��ȯ��
	WideCharToMultiByte(CP_ACP , 0, filename, -1, str, sLen, NULL, NULL);
	
	m_pImage = cvLoadImage(str,1);	

	
	// �����Ҵ�� �޸� ����
	delete []str;

	Detect();
	
}
//�޴� �������
void CMFCApplication2Dlg::OnOncam()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_pCapture = cvCreateCameraCapture(3);
	SetTimer( _DEF_WEBCAM, 100, NULL );		// Ÿ�̸� ����
	
}
//�޴� ��������
void CMFCApplication2Dlg::OnOffcam()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(_DEF_WEBCAM);
}
//�̹����� �ҷ��� �����ϴ� �Լ� (����� ����..)
void CMFCApplication2Dlg::Detect()
{
	m_Preprocess = TRUE;	//��ó��
	m_Labeling = TRUE;		//���̺� 
	m_bDeNoise = TRUE;		//��Ī
	m_operation = TRUE;		//����

	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);

	Upload load = Upload();
	IplImage* binaryImage = cvCreateImage(cvGetSize(m_pImage), 8, 1 );
	
	//��ó�� �Լ�
	Binary(m_pImage, binaryImage); 

	IplImage* labelingImage = cvCreateImage( cvGetSize(binaryImage), IPL_DEPTH_8U, 3);	
	IplImage* outcome = cvCreateImage(cvGetSize(binaryImage),IPL_DEPTH_8U,1);
	outcome = (IplImage*)cvClone(binaryImage);
	//ħ��, ��â�Լ� (��ư)
	ED(binaryImage,m_bErode,m_bDilate); 

	//������������ �簢�� ����
	
	//���̺�
	labelingImage = load.Labeling(outcome, labelingImage, 8, m_bDeNoise);
	load.Loadimage_Preprocessing(m_pImage, m_pImage->width/4, m_pImage->height/2, m_pImage->width, m_pImage->height);
	//load.Operation_Run(m_pImage, m_pImage->width/4, m_pImage->height/2, m_pImage->width, m_pImage->height);

	DrawCDC(labelingImage);

	//�޸� ����
	cvReleaseImage(&labelingImage);
	cvReleaseImage(&outcome);
	cvReleaseImage(&binaryImage);

	load.~Upload();

	Invalidate(FALSE);
}

//��ó��
void CMFCApplication2Dlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_Preprocess = !m_Preprocess;	//��ó��
	if(m_Preprocess)
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	else
	{
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);

		m_Labeling = FALSE;		//���̺� 
		m_bDeNoise = FALSE;		//��Ī
		m_operation = FALSE;	//����
	}
}

//�󺧸�
void CMFCApplication2Dlg::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_Labeling = !m_Labeling;		//���̺� 

	if(m_Labeling)
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	else
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
		m_bDeNoise = FALSE;		//��Ī
		m_operation = FALSE;	//����
	}
}

//����
void CMFCApplication2Dlg::OnBnClickedButton7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_operation = !m_operation;		//����
}
