#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "stdafx.h"
#include "BlobLabeling.h"
#include <vector>
#include <string.h>
using namespace std;
#pragma warning(disable:4819)

//��Ī�� �����͸� ���� ����ü�� ����!
typedef struct tagLabel_Data Label_Data;
struct tagLabel_Data
{
	string data;         //�ǿ�����, ������
	CvPoint left_top;      //������
	CvPoint right_bottom;   //����
	int size;            //�̹��� ����ũ��
};

static vector<Label_Data> data_list;//���̺� �� �� �ν� �� ������� �� vector
static vector<Label_Data> data_list_print;//���̺� �� �� �ν� �� ������� �� vector
static vector<Label_Data> Final_Modification_Print;//���� ���� 

class Upload
{
private:
	CBlobLabeling blob;
	vector<string>L_data;
	string Result_Value;         //�����
	bool Square_Check;			//��������
	string Formula_Eq;				//������ ��º���
	string Answer_Eq;				//���� ����� ��º���
	bool SimpleEquation;
	bool LinearEquation;
	bool GeneralCalculation;
public:
	//������ & �Ҹ���
	Upload(){
		blob = CBlobLabeling();
		Result_Value = "";
		Square_Check = false;
		Formula_Eq = "";
		Answer_Eq = "";
		SimpleEquation = false;
		LinearEquation = false;
		GeneralCalculation = false;
	}
	~Upload(){}

	//------------------------------------- �ν� �κ�--------------------------------------------- //
	//���̺� �Լ�
	IplImage* Labeling(IplImage* image, IplImage* LabelingIamge ,DOUBLE mask, BOOL noise);
	//������ ���ø���Ī �Լ�
	double Template_Matching(IplImage* template_image, IplImage* labeling_image);
	//���ø� �Լ�
	CString Number_Matching(IplImage* image);
	//�ʱ�ü ����ȭ �Լ�
	double Thinning_Matching(IplImage* template_image, IplImage* labeling_image);


	//------------------------------------- ��� �κ�--------------------------------------------- //
	//�ﰢ�Լ�
	BOOL Trigonometrical_Function();
	//��ó�� �Լ���
	void SquareFunc();		// ����
	void SDFEFunc();		// -
	void PointFunc();		// �Ҽ���
	void BracketFunc();		// ��ȣ
	//��Ģ���� ���� ���� �˰���
	double Calculation(double a, char op, double b);
	//�߰� ����
	string Middle_Final_Operation(int FristIdx, int SecondIdx);

	//�����ڿ� �ǿ����� ���� ����
	void Final_Set();
	//����Լ�
	void Result_Print(IplImage* original, int x, int y, int width, int height);
	//�ε� ó���Լ�
	void Upload::Loadimage_Preprocessing(IplImage* original, int x, int y, int width, int height);
	//������� ���Լ�
	void Operation_Run(IplImage* original, int x, int y, int width, int height);
	void Error_Printing(IplImage* original);
	void Save_DB(BOOL& T_SAVE);



	//�μ� ������


	//1, 2�� ������ �ڵ� By ���μ�
	vector<string> M_F_O_V;
	vector<string> lineup_OM;

	bool Eq_SimpleEquation;
	bool Eq_LinearEquation;
	bool Eq_RecluseNation_LTH;

	int find;





	void Eq_SDFEFunc();
	void Eq_PointFunc();
	void Eq_SquareFunc();
	void Eq_Final_Set();
	void Eq_BracketFunc();
	double Eq_Calculation(double a, char op, double b);
	void Eq_Middle_Final_Operation(int FristIdx, int SecondIdx);
	bool Eq_Bracket_Middle_Operation(int OPidx, char OP);
	bool Eq_Bracket_Middle_Operation_Label_Data(int OPidx, char OP, vector<Label_Data> &K);
	void Eq_lineup_String(vector<string>& K);
	void Eq_lineup_Label_Data(vector<Label_Data>& K);
	bool Eq_Lvalue_Rvalue_Groupage();
	void Eq_LinearEquation_Final_ArithmeticOperation();
	void Eq_SimpleEquation_Final_ArithmeticOperation();
	void Eq_Quad_Eqn(double a, double b, double c);
	void Real_Final_Deduct(IplImage* original, int x, int y, int width, int height);



	void PrintBitmap(LPCTSTR filename);
};