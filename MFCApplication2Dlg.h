// MFCApplication2Dlg.h : 헤더 파일
//
#pragma once
#include "afxwin.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "CvvIamge.h"
#include "Upload.h"
#include "Mmsystem.h"
// CMFCApplication2Dlg 대화 상자
class CMFCApplication2Dlg : public CDialogEx
{
	// 생성입니다.
public:
	CMFCApplication2Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	CStatic m_stDisplay;
	CStatic m_stDisplay2;

	IplImage*	m_pImage;
	CvvImage	m_cImage;
	//CvvImage	limage;

	CvCapture*	m_pCapture;

	BOOL m_Preprocess;		//전처리
	BOOL m_Labeling;		//레이블링 
	BOOL m_bDeNoise;		//매칭
	BOOL m_operation;		//연산
	BOOL m_bDilate;			//팽창
	BOOL m_bErode;			//침식

	int Erodecount;			//침식 카운트
	int Dilatecount;		//팽창 카운트
	double noise;

	int cx;					//이진화 영역의 center x값
	int cy;					//이진화 영역의 center y값
	int widthvalue;			//이진화 영역의 가로 가변 값
	int heightvalue;		//이진화 영역의 세로 가변 값

public:
	virtual BOOL DestroyWindow();

	//버튼
	//afx_msg void OnBnClickedOk();				//이진화
	//afx_msg void OnBnClickedChkLabel();		//라벨링
	afx_msg void OnBnClickedChkErode();			//침식
	afx_msg void OnBnClickedChkDilate();		//팽창

	//기능 함수
	afx_msg void OnTimer(UINT_PTR nIDEvent);							//호출
	afx_msg void DrawCDC(IplImage* Image);								//그리기
	afx_msg void Binary(IplImage*	m_pImage,IplImage* binaryImage);	//이진화
	afx_msg void ED(IplImage* Image, BOOL Erode, BOOL Dilate);			//침식, 팽창

	afx_msg void OnClickedButtonHeightMinus();
	afx_msg void OnClickedButtonHeightPlus();
	afx_msg void OnClickedButtonWidthPlus();
	afx_msg void OnClickedButtonWidthMinus();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnFileopen();
	void loadimage(CString filename);
	afx_msg void OnOncam();
	afx_msg void OnOffcam();
	void Detect();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton7();
};
