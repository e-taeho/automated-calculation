// MFCApplication2Dlg.h : ��� ����
//
#pragma once
#include "afxwin.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "CvvIamge.h"
#include "Upload.h"
#include "Mmsystem.h"
// CMFCApplication2Dlg ��ȭ ����
class CMFCApplication2Dlg : public CDialogEx
{
	// �����Դϴ�.
public:
	CMFCApplication2Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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

	BOOL m_Preprocess;		//��ó��
	BOOL m_Labeling;		//���̺� 
	BOOL m_bDeNoise;		//��Ī
	BOOL m_operation;		//����
	BOOL m_bDilate;			//��â
	BOOL m_bErode;			//ħ��

	int Erodecount;			//ħ�� ī��Ʈ
	int Dilatecount;		//��â ī��Ʈ
	double noise;

	int cx;					//����ȭ ������ center x��
	int cy;					//����ȭ ������ center y��
	int widthvalue;			//����ȭ ������ ���� ���� ��
	int heightvalue;		//����ȭ ������ ���� ���� ��

public:
	virtual BOOL DestroyWindow();

	//��ư
	//afx_msg void OnBnClickedOk();				//����ȭ
	//afx_msg void OnBnClickedChkLabel();		//�󺧸�
	afx_msg void OnBnClickedChkErode();			//ħ��
	afx_msg void OnBnClickedChkDilate();		//��â

	//��� �Լ�
	afx_msg void OnTimer(UINT_PTR nIDEvent);							//ȣ��
	afx_msg void DrawCDC(IplImage* Image);								//�׸���
	afx_msg void Binary(IplImage*	m_pImage,IplImage* binaryImage);	//����ȭ
	afx_msg void ED(IplImage* Image, BOOL Erode, BOOL Dilate);			//ħ��, ��â

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
