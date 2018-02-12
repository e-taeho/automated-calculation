#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "stdafx.h"
#include "BlobLabeling.h"
#include <vector>
#include <string.h>
using namespace std;
#pragma warning(disable:4819)

//매칭후 데이터를 담을 구조체를 선언!
typedef struct tagLabel_Data Label_Data;
struct tagLabel_Data
{
	string data;         //피연산자, 연산자
	CvPoint left_top;      //시작점
	CvPoint right_bottom;   //끝점
	int size;            //이미지 실제크기
};

static vector<Label_Data> data_list;//레이블링 한 후 인식 한 결과들이 들어갈 vector
static vector<Label_Data> data_list_print;//레이블링 한 후 인식 한 결과들이 들어갈 vector
static vector<Label_Data> Final_Modification_Print;//최종 수식 

class Upload
{
private:
	CBlobLabeling blob;
	vector<string>L_data;
	string Result_Value;         //결과값
	bool Square_Check;			//제곱변수
	string Formula_Eq;				//수식을 담는변수
	string Answer_Eq;				//수식 결과를 담는변수
	bool SimpleEquation;
	bool LinearEquation;
	bool GeneralCalculation;
public:
	//생성자 & 소멸자
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

	//------------------------------------- 인식 부분--------------------------------------------- //
	//레이블링 함수
	IplImage* Labeling(IplImage* image, IplImage* LabelingIamge ,DOUBLE mask, BOOL noise);
	//디지털 템플릿매칭 함수
	double Template_Matching(IplImage* template_image, IplImage* labeling_image);
	//템플릿 함수
	CString Number_Matching(IplImage* image);
	//필기체 세선화 함수
	double Thinning_Matching(IplImage* template_image, IplImage* labeling_image);


	//------------------------------------- 계산 부분--------------------------------------------- //
	//삼각함수
	BOOL Trigonometrical_Function();
	//전처리 함수들
	void SquareFunc();		// 제곱
	void SDFEFunc();		// -
	void PointFunc();		// 소수점
	void BracketFunc();		// 괄호
	//사칙연산 최종 연산 알고리즘
	double Calculation(double a, char op, double b);
	//중간 연산
	string Middle_Final_Operation(int FristIdx, int SecondIdx);

	//연산자와 피연산자 최종 정리
	void Final_Set();
	//출력함수
	void Result_Print(IplImage* original, int x, int y, int width, int height);
	//로드 처리함수
	void Upload::Loadimage_Preprocessing(IplImage* original, int x, int y, int width, int height);
	//연산과정 런함수
	void Operation_Run(IplImage* original, int x, int y, int width, int height);
	void Error_Printing(IplImage* original);
	void Save_DB(BOOL& T_SAVE);



	//민석 방정식


	//1, 2차 방정식 코드 By 오민석
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