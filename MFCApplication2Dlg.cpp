// MFCApplication2Dlg.cpp : 구현 파일
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

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
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

// CMFCApplication2Dlg 대화 상자

CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication2Dlg::IDD, pParent)
	, m_Preprocess(FALSE)	//전처리
	, m_Labeling(FALSE)		//레이블링 
	, m_bDeNoise(FALSE)		//매칭
	, m_operation(FALSE)	//연산
	, m_pImage(0)
	, m_pCapture(0)
	//사각형크기 초기설정
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
// CMFCApplication2Dlg 메시지 처리기

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
 

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	
	//침식 팽창 카운트 초기화
	Erodecount = 0;
	Dilatecount = 0;

	//침식,팽창 처리된 영상 이미지


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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
// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if( m_pImage )
		{
			// 메인 이미지
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
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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

		//전처리 함수
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

		//침식, 팽창함수 (버튼)
		ED(binaryImage,m_bErode,m_bDilate); 

		//메인프레임의 사각형 설정
		img = (IplImage*)cvClone(binaryImage);
		cvSetImageROI(img,cvRect(binaryImage->roi->xOffset,
			binaryImage->roi->yOffset, binaryImage->roi->width, binaryImage->roi->height) );
		cvResize(img, outcome, CV_INTER_NN);			
		cvResetImageROI(img);

		//레이블링
		if(m_Labeling)
		labelingImage = load.Labeling(outcome, labelingImage, 8, m_bDeNoise);
		if(m_operation)
		load.Operation_Run(m_pImage, rx, ry, rw, rh);


		if(m_Labeling)
		DrawCDC(labelingImage);
		else
		DrawCDC(binaryImage);

		//메모리 해제
		cvReleaseImage(&labelingImage);
		cvReleaseImage(&outcome);
		cvReleaseImage(&img);
		cvReleaseImage(&binaryImage);

		load.~Upload();

		Invalidate(FALSE);

		//가로 세로 +, - 버튼 가로, 세로 값에 따라 활성화
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
//닫기
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
//침식 버튼
void CMFCApplication2Dlg::OnBnClickedChkErode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bErode = TRUE;
	UpdateData(FALSE);
	Erodecount++;
}
//팽창 버튼
void CMFCApplication2Dlg::OnBnClickedChkDilate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bDilate = TRUE;
	UpdateData(FALSE);
	Dilatecount++;
}
//DC에 그리기
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
//전처리
void CMFCApplication2Dlg ::Binary(IplImage* m_pImage,IplImage* binaryImage)
{
	cvCvtColor( m_pImage, binaryImage, CV_BGR2GRAY);
	cvThreshold( binaryImage, binaryImage, 70, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvErode(binaryImage, binaryImage, NULL, 2);
}
//침식 팽창
void CMFCApplication2Dlg ::ED(IplImage* Image, BOOL Erode, BOOL Dilate)
{
	if(Erode=TRUE)//침식 
	{
		cvDilate(Image,Image,NULL,Erodecount);
	}
	if(Dilate=TRUE)//팽창
	{
		cvErode(Image,Image,NULL,Dilatecount);
	}
}
//세로 + 버튼
void CMFCApplication2Dlg::OnClickedButtonHeightPlus()
{
	if(heightvalue > 200)
	{
		AfxMessageBox(_T("최대 크기입니다"));
		GetDlgItem(IDC_BUTTON_height_P)->EnableWindow(FALSE);
	}
	else
		heightvalue+=25;
}
//세로 - 버튼
void CMFCApplication2Dlg::OnClickedButtonHeightMinus()
{
	if(heightvalue < 30)
	{
		AfxMessageBox(_T("최소 크기입니다"));
		GetDlgItem(IDC_BUTTON_height_M)->EnableWindow(FALSE);
	}
	else
		heightvalue-=25;
}
//가로 + 버튼
void CMFCApplication2Dlg::OnClickedButtonWidthPlus()
{
	if(widthvalue > 275)
	{
		AfxMessageBox(_T("최대 크기입니다"));
		GetDlgItem(IDC_BUTTON_width_P)->EnableWindow(FALSE);
	}
	else
		widthvalue+=25;
}
//가로 - 버튼
void CMFCApplication2Dlg::OnClickedButtonWidthMinus()
{
	if(widthvalue < 30)
	{
		AfxMessageBox(_T("최소 크기입니다"));
		GetDlgItem(IDC_BUTTON_width_M)->EnableWindow(FALSE);
	}
	else
		widthvalue-=25;
}

//매칭
void CMFCApplication2Dlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_bDeNoise = !m_bDeNoise;
	if(m_bDeNoise)
		GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
	else
	{
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
		m_operation = FALSE;	//연산
	}
	UpdateData(FALSE);
}
//이미지의 경로를 불러오는함수
void CMFCApplication2Dlg::OnFileopen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
//이미지의 띄운다
void CMFCApplication2Dlg::loadimage(CString filename)
{
	if(m_pImage != NULL) cvReleaseImage(&m_pImage);
	KillTimer(_DEF_WEBCAM);

	char * str;

	int sLen = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);

	// 변환된 문자열이 저장될 공간을 동적할당함
	str=new char[sLen+1];

	// 문자열 변환함
	WideCharToMultiByte(CP_ACP , 0, filename, -1, str, sLen, NULL, NULL);
	
	m_pImage = cvLoadImage(str,1);	

	
	// 동적할당된 메모리 제거
	delete []str;

	Detect();
	
}
//메뉴 영상시작
void CMFCApplication2Dlg::OnOncam()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_pCapture = cvCreateCameraCapture(3);
	SetTimer( _DEF_WEBCAM, 100, NULL );		// 타이머 설정
	
}
//메뉴 영상정지
void CMFCApplication2Dlg::OnOffcam()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	KillTimer(_DEF_WEBCAM);
}
//이미지를 불러와 연산하는 함수 (결과값 수정..)
void CMFCApplication2Dlg::Detect()
{
	m_Preprocess = TRUE;	//전처리
	m_Labeling = TRUE;		//레이블링 
	m_bDeNoise = TRUE;		//매칭
	m_operation = TRUE;		//연산

	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);

	Upload load = Upload();
	IplImage* binaryImage = cvCreateImage(cvGetSize(m_pImage), 8, 1 );
	
	//전처리 함수
	Binary(m_pImage, binaryImage); 

	IplImage* labelingImage = cvCreateImage( cvGetSize(binaryImage), IPL_DEPTH_8U, 3);	
	IplImage* outcome = cvCreateImage(cvGetSize(binaryImage),IPL_DEPTH_8U,1);
	outcome = (IplImage*)cvClone(binaryImage);
	//침식, 팽창함수 (버튼)
	ED(binaryImage,m_bErode,m_bDilate); 

	//메인프레임의 사각형 설정
	
	//레이블링
	labelingImage = load.Labeling(outcome, labelingImage, 8, m_bDeNoise);
	load.Loadimage_Preprocessing(m_pImage, m_pImage->width/4, m_pImage->height/2, m_pImage->width, m_pImage->height);
	//load.Operation_Run(m_pImage, m_pImage->width/4, m_pImage->height/2, m_pImage->width, m_pImage->height);

	DrawCDC(labelingImage);

	//메모리 해제
	cvReleaseImage(&labelingImage);
	cvReleaseImage(&outcome);
	cvReleaseImage(&binaryImage);

	load.~Upload();

	Invalidate(FALSE);
}

//전처리
void CMFCApplication2Dlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Preprocess = !m_Preprocess;	//전처리
	if(m_Preprocess)
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	else
	{
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);

		m_Labeling = FALSE;		//레이블링 
		m_bDeNoise = FALSE;		//매칭
		m_operation = FALSE;	//연산
	}
}

//라벨링
void CMFCApplication2Dlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Labeling = !m_Labeling;		//레이블링 

	if(m_Labeling)
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	else
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
		m_bDeNoise = FALSE;		//매칭
		m_operation = FALSE;	//연산
	}
}

//연산
void CMFCApplication2Dlg::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_operation = !m_operation;		//연산
}
