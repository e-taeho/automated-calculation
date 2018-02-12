#include "stdafx.h"
#include "Upload.h"
#include "BlobLabeling.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <functional>
#pragma warning(disable:4996)
#pragma warning(disable:4244)
//------------------------------------- 인식 부분-------------------------------------------------- //
//레이블링
IplImage* Upload::Labeling(IplImage* image,IplImage* LabelingIamge, DOUBLE mask, BOOL noise)
{

   data_list.clear();
   data_list_print.clear();
   // 레이블링시 흰색(255)을 개체로 잡으므로 이진화된 영상을 반전시킨다.
   cvThreshold( image, image, 1, 255, CV_THRESH_BINARY_INV );   
   cvCvtColor( image, LabelingIamge, CV_GRAY2BGR);

   // 레이블링
   blob.SetParam( image, 100 );   // 레이블링 할 이미지와 최대 픽셀수 등을 설정
   blob.DoLabeling();

   //전체화면에서 mask의 크기의 레이블 제거
   int nMaxWidth   = (int)(image->width  * mask / 10);
   int nMaxHeight   = (int)(image->height * mask / 10);

   //blob.BlobSmallSizeConstraint( 30, 30 );   //작은것 제거
   blob.BlobBigSizeConstraint( nMaxWidth, nMaxHeight );

   //레이블링 영역에 사각형을 그린다.
   for( int i=0; i < blob.m_nBlobs; i++ )
   {
      //레이블링 영역을 하나의 크기로 담을 이미지변수 선언
      IplImage* outcome = cvCreateImage(cvSize(16,16),IPL_DEPTH_8U,1);
      IplImage* img = (IplImage*)cvClone(image);

      //사각형의 시작점의 위치 pt1, 끝점의 위치 pt2
      CvPoint   pt1 = cvPoint(   blob.m_recBlobs[i].x-5,blob.m_recBlobs[i].y-5);
      CvPoint pt2 = cvPoint(   pt1.x + blob.m_recBlobs[i].width+10, pt1.y + blob.m_recBlobs[i].height+10);

      //이미지 관심영역 설정 후 크기 재설정
      cvSetImageROI(img,cvRect(pt1.x, pt1.y, blob.m_recBlobs[i].width+10, blob.m_recBlobs[i].height+10));
      cvResize(img, outcome, CV_INTER_NN);

      CString num = NULL;
      if(noise)
      {
         //디지털 인식, 필기체인식
         num = Number_Matching(outcome);
      }
      else
      {
         CvScalar red   = cvScalar( 0, 0, 255 );
         cvDrawRect( LabelingIamge, pt1, pt2, red, 2);   
      }
      //매칭값을 찾았을때
      if(num != "")
      {
         //실제 사각형과 템플릿 매칭후 결과값을 그린다.
         CvScalar red   = cvScalar( 0, 0, 255 );
         cvDrawRect( LabelingIamge, pt1, pt2, red, 2);         

         //매칭 결과값 출력함수
         char* ss = LPSTR(LPCTSTR(num));
         CvFont font;
         int hscale = 1;
         int vscale = 1;
         int linewidth = 2;
         cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
         cvPutText(LabelingIamge, ss, cvPoint(blob.m_recBlobs[i].x + blob.m_recBlobs[i].width/2, pt1.y+3), &font, cvScalar(0,0,255));

         //결과값, 해당위치, 크기를 넣는다.
         Label_Data data;
         data.data = *LPSTR(LPCTSTR(num));
         data.left_top = pt1;
         data.right_bottom = pt2;
         data.size = blob.m_recBlobs[i].width * blob.m_recBlobs[i].height;
         data_list.push_back(data);
      }
      else
      {
         CvScalar red   = cvScalar( 255, 0, 0 );
         cvDrawRect( LabelingIamge, pt1, pt2, red, 2);
      }

      cvReleaseImage(&outcome);
      cvReleaseImage(&img);
   }
   blob.~CBlobLabeling();
   return LabelingIamge;
}
//템플릿 함수
CString Upload::Number_Matching(IplImage* image)
{
	int count = 0;
	char value;
	char number[20] = {0,}; 
	BOOL nocount = FALSE;
	double similarity[20] = {0,};
	IplImage* num;
	double result1;
	int threshold = 0;

	for(int j = 0; j < 20; j++)
	{
		if(j == 0)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\0.jpg", 1);
			value = '0';
			nocount = TRUE;
		}
		else if(j == 1)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\1.jpg", 1);
			value = '1';
			nocount = TRUE;
		}
		else if(j == 2)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\2.jpg", 1);
			value = '2';
			nocount = TRUE;
		}
		else if(j == 3)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\3.jpg", 1);
			value = '3';
			nocount = TRUE;
		}
		else if(j == 4)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\4.jpg", 1);
			value = '4';
			nocount = TRUE;
		}
		else if(j == 5)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\5.jpg", 1);
			value = '5';
			nocount = TRUE;
		}
		else if(j == 6)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\6.jpg", 1);
			value = '6';
			nocount = TRUE;
		}
		else if(j == 7)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\7.jpg", 1);
			value = '7';
			nocount = TRUE;
		}
		else if(j == 8)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\8.jpg", 1);
			value = '8';
			nocount = TRUE;
		}
		else if(j == 9)
		{
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\9.jpg", 1);
			value = '9';
			nocount = TRUE;
		}
		else if(j == 10)
		{
			//+
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\+.jpg", 1);
			value = '+';
			nocount = TRUE;
		}
		else if(j == 11)
		{
			//-
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\-.jpg", 1);
			value = '-';
			nocount = TRUE;
		}
		else if(j == 12)
		{
			//*
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\m.jpg", 1);
			value = '*';
			nocount = TRUE;
		}
		else if(j == 13)
		{
			//(
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\(.jpg", 1);
			value = '(';
			nocount = TRUE;
		}
		else if(j == 14)
		{
			//)
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\).jpg", 1);
			value = ')';
			nocount = TRUE;
		}
		else if(j == 15)
		{
			//.
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\..jpg", 1);
			value = '.';
			nocount = TRUE;
		}
		else if(j == 16)
		{
			//s
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\s.jpg", 1);
			value = 's';
			nocount = TRUE;
		}
		else if(j == 17)
		{	
			//c
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\c.jpg", 1);
			value = 'c';
			nocount = TRUE;

		}
		else if(j == 18)
		{	
			//t
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\t.jpg", 1);
			value = 't';
			nocount = TRUE;

		}
		else if(j == 19)
		{	
			//n
			num = cvLoadImage("C:\\Users\\Becks\\Desktop\\프로젝트\\BIT\\조절숫자\\n.jpg", 1);
			value = 'n';
			nocount = TRUE;

		}

		//경로가 잘못지정된 경우 리턴
		if(num == NULL)
			return NULL;      

		//템플릿 이미지
		IplImage* D_binary = cvCreateImage(cvSize(num->width, num->height), IPL_DEPTH_8U, 1);
		IplImage* result = cvCreateImage(cvSize(num->width - image->width +1, num->height - image->height +1),IPL_DEPTH_32F, 1);
		cvCvtColor(num , D_binary, CV_BGR2GRAY);
		cvThreshold(D_binary,D_binary,70, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

		//템플릿 함수
		result1 = Template_Matching(D_binary,image);
		threshold = 93;

		//유사도가 93퍼센트이상일경우 리턴
		if(result1 > threshold)
		{
			cvReleaseImage(&num);
			cvReleaseImage(&D_binary);
			cvReleaseImage(&result);
			CString result;
			result.Format(_T("%c"), value);
			return result;
		}

		if(result1 > threshold - 10) 
		{
			number[count] = value;   
			similarity[count] = result1; 
			count++;
		}
		cvReleaseImage(&num);
		cvReleaseImage(&D_binary);
		cvReleaseImage(&result);
	}

	//유사도가 가장큰 매칭값을 0번째 인덱스로 가져옴 (버블정렬)
	for(int i = 0; i <count-1; i++)
		for(int j = 0; j < count-1; j++)
		{
			if(similarity[j] < similarity[j+1])
			{
				int temp = number[j];
				number[j] = number[j+1];
				number[j+1] = temp;

				double temp2 = similarity[j];
				similarity[j] = similarity[j+1];
				similarity[j+1] = temp2;
			}
		}

		//형변환
		CString result;
		result.Format(_T("%c"), number[0]);

		if(count!=0)
			return  result;
		else
			return NULL;
}
//세선화 함수 
double Upload::Thinning_Matching(IplImage* template_image, IplImage* labeling_image)
{
	//필기체의 매칭이미지
	IplImage* addimage = cvCreateImage(cvGetSize(labeling_image), 8, 1);
	//레이블이미지 크기설정
	int height = labeling_image->height; 
	int width = labeling_image->width;
	int widstep = labeling_image->widthStep;
	//템플릿이미지 크기설정
	int twidstep = template_image->widthStep;
	int wsize = template_image->width/width;
	int hsize = template_image->height/height;
	//ROI영역설정할 변수
	int sx = 0; int sy = 0; int idx = 0;
	double* similarity = new double[wsize*hsize]; //유사도를 담을 배열
	double result = 0;							//결과값을 담을 배열

	//실제 매칭 알고리즘
	for(int n = 0; n < hsize; n++)
	{
		for(int m = 0; m < wsize; m++)
		{
			similarity[n*m+m] = 0;
			cvSetImageROI(template_image, cvRect(sx, sy, width, height));
			uchar* aptr = (uchar*)addimage->imageData;			
			uchar* tptr = (uchar*)template_image->imageData;	//템플릿이미지 포인터
			uchar* lptr = (uchar*)labeling_image->imageData;	//라벨링이미지 포인터

			for(int i = 0; i < height; i++)
			{
				for(int j = 0; j < width; j++)
				{
					idx = i*widstep+j;
					if(tptr[((sy+i)*twidstep)+(j+sx)] + lptr[idx] >= 255)
					{
						aptr[idx] = 255;
						if(lptr[idx] == aptr[idx])
							similarity[n*m+m]++;
					}
					else
					{
						aptr[idx] = 0;
						if(lptr[idx] == aptr[idx])
							similarity[n*m+m]++;
					}
				}
			}

			//유사도를 백분률로 변환
			result = (similarity[n*m+m]/(height*width))*100;

			cvShowImage("1", addimage);
			if(result > 98)
			{
				delete[] similarity;
				cvResetImageROI(template_image);
				cvReleaseImage(&addimage);
				return result;
			}
			//탐색위치 변경
			if(m == wsize -1)
			{
				sy += height;
				sx = 0;
			}
			else
				sx += width;
			cvResetImageROI(template_image);
		}
	}

	//버블정렬로 최대값을 찾는다.
	for(int i = 0; i <wsize*hsize-1; i++)
		for(int j = 0; j < wsize*hsize-1; j++)
		{
			if(similarity[j] < similarity[j+1])
			{
				double temp = similarity[j];
				similarity[j] = similarity[j+1];
				similarity[j+1] = temp;
			}
		}
		//찾은 최대값을 리턴
		result = (similarity[0]/(height*width))*100;
		delete[] similarity;
		cvReleaseImage(&addimage);
		return result;
}
//실제템플릿매칭함수
double Upload::Template_Matching(IplImage* template_image, IplImage* labeling_image)
{
	//레이블이미지 크기설정
	int height = labeling_image->height; 
	int width = labeling_image->width;
	int widstep = labeling_image->widthStep;
	//템플릿이미지 크기설정
	int twidstep = template_image->widthStep;
	int wsize = template_image->width/width;
	int hsize = template_image->height/height;
	//ROI영역설정할 변수
	int sx = 0; int sy = 0;
	double* similarity = new double[wsize*hsize]; //유사도를 담을 배열
	double result = 0;							//결과값을 담을 배열

	//실제 매칭 알고리즘
	for(int n = 0; n < hsize; n++)
	{
		for(int m = 0; m < wsize; m++)
		{
			similarity[n*m+m] = 0;
			uchar* tptr = (uchar*)template_image->imageData;	//템플릿이미지 포인터
			uchar* lptr = (uchar*)labeling_image->imageData;	//라벨링이미지 포인터

			for(int i = 0; i < height; i++)
			{
				for(int j = 0; j < width; j++)
				{
					//픽셀값 비교
					if(tptr[((sy+i)*twidstep)+(j+sx)] == lptr[i*widstep+j])
						similarity[n*m+m]++;
				}
			}
			//유사도를 백분률로 변환
			result = (similarity[n*m+m]/(height*width))*100;
			if(result > 94)
			{
				delete[] similarity;
				return result;
			}
			//탐색위치 변경
			if(m == wsize -1)
			{
				sy += height;
				sx = 0;
			}
			else
				sx += width;
		}
	}

	//버블정렬로 최대값을 찾는다.
	for(int i = 0; i <wsize*hsize-1; i++)
		for(int j = 0; j < wsize*hsize-1; j++)
		{
			if(similarity[j] < similarity[j+1])
			{
				double temp = similarity[j];
				similarity[j] = similarity[j+1];
				similarity[j+1] = temp;
			}
		}
		//찾은 최대값을 리턴
		result = (similarity[0]/(height*width))*100;
		delete[] similarity;
		return result;
}
//------------------------------------------------------------------------------------------------//

//------------------------------------- 계산 부분-------------------------------------------------- //
//수식 연산함수
BOOL Upload::Trigonometrical_Function()
{
	BOOL isSin = FALSE;
	BOOL isCos = FALSE;
	BOOL isTan = FALSE;
	int idx = -1;		//각도 찾을 인덱스
	string degree = "";	//각도
	double result;		//결과값

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		//sin
		if(data_list.at(i).data == "s")
		{
			for(unsigned int j = i; j < data_list.size(); j++)
			{
				if(data_list.at(j).data == "n")
				{
					isSin = TRUE;
					idx = j;
					break;//추가
				}
			}
		}

		//cos
		if(data_list.at(i).data == "c")
		{
			for(unsigned int j = i; j < data_list.size(); j++)
			{
				if(data_list.at(j).data == "s")
				{
					isCos = TRUE;
					idx = j;
					break;//추가
				}
			}
		}

		//tan
		if(data_list.at(i).data == "t")
		{
			for(unsigned int j = i; j < data_list.size(); j++)
			{
				if(data_list.at(j).data == "n")
				{
					isTan = TRUE;
					idx = j;
					break;//추가
				}
			}
		}
	}

	if(isSin || isCos || isTan)
	{
		for(unsigned int i = idx; i < data_list.size(); i++)
		{
			if(*(data_list.at(i).data.c_str()) >= 48 && *(data_list.at(i).data.c_str()) <= 57)
				degree += data_list.at(i).data;
		}
	}
	else
		return FALSE;

	int r = -1;
	r = atoi(degree.c_str());

	if(r >= -1 && r >= 180)
		return FALSE;

	if(isSin)
	{
		result = sin(r*3.1416/180);
		data_list.at(0).data = "sin " + degree;
		Label_Data temp;
		temp.data = data_list.at(0).data;
		data_list_print.push_back(temp);
	}
	if(isCos)
	{
		result = cos(r*3.1416/180);
		data_list.at(0).data = "cos " + degree;
		Label_Data temp;
		temp.data = data_list.at(0).data;
		data_list_print.push_back(temp);
	}
	if(isTan)
	{
		result = tan(r*3.1416/180);
		data_list.at(0).data = "tan " + degree;
		Label_Data temp;
		temp.data = data_list.at(0).data;
		data_list_print.push_back(temp);

	}

	data_list.erase(data_list.begin()+1, data_list.begin() + data_list.size());

	Result_Value = to_string(result);
	int i = 0;
	return TRUE;
}												
void Upload::PointFunc()
{
	if(data_list.size() == 0)
		return;

	if(data_list.at(0).data == "." || data_list.at(data_list.size()-1).data == ".")
	{
		data_list.clear();
		return;
	}


	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == "+" || data_list.at(i).data == "*" || data_list.at(i).data == "/")
		{
			if(i == data_list.size()-1)
			{
				data_list.clear();
				return;
			}
			if(data_list.at(i+1).data == ".")
			{
				data_list.clear();
				return;
			}
		}
	}

	int idx = 0;
	int FrontIdx = 0;
	int BackIdx = 0;
	string DoubleStr = "";

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == ".")
			idx++;
	}

	if(idx == 0)
		return;
	for (int i = 0; i < idx; i++)
	{
		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(data_list.at(i).data == ".")
			{
				for(unsigned int B = i; B >= 0; B--)
				{
					if(data_list.at(B).data == "+" || data_list.at(B).data == "-" 
						|| data_list.at(B).data == "*" || data_list.at(B).data == "/" || data_list.at(B).data == "(" || B == 0)
					{
						if(data_list.at(B).data == "-")
						{
							if(data_list.at(B-1).data == "+" || data_list.at(B-1).data == "-" || //추가
								data_list.at(B-1).data == "*" || data_list.at(B-1).data == "/")
							{
								FrontIdx = B;
								break;
							}
							else
							{
								FrontIdx = B+1;
								break;
							}
						}

						if(data_list.at(B).data == "(")
						{
							FrontIdx = B+1;//추가 변경
							break;
						}

						if(B == 0)
							B=-1;
						FrontIdx = B+1;
						break;
					}
				}

				for(unsigned int j = FrontIdx; j < data_list.size()-1; j++)
				{
					if((data_list.at(j).size / 2   > data_list.at(j+1).size && 
						data_list.at(j+1).data != "0" && 
						data_list.at(j).data != "+" && data_list.at(j).data != "-" && 
						data_list.at(j).data != "*" && data_list.at(j).data != "/" &&
						data_list.at(j+1).data != "+" && data_list.at(j+1).data != "-" && 
						data_list.at(j+1).data != "*" && data_list.at(j+1).data != "/" && data_list.at(j+1).data != "." &&
						j+1 == data_list.size()) || 
						((data_list.at(j).right_bottom.y - data_list.at(j).left_top.y)/2 + (data_list.at(j).left_top.y)) > data_list.at(j+1).right_bottom.y)
					{
						if(j+1 == data_list.size())
							j = data_list.size();
						else
							BackIdx = j+1;

						for(int k = FrontIdx; k < BackIdx; k++)
						{
							DoubleStr += data_list.at(k).data;
						}
						break;
					}


					if(data_list.at(j).data == "+" || data_list.at(j).data == "-" 
						|| data_list.at(j).data == "*" || data_list.at(j).data == "/" || data_list.at(j).data == ")" || j == data_list.size()-1)
					{
						if(j == FrontIdx)
							continue;

						if(j == data_list.size()-1)
							j++;
						BackIdx = j;
						for(int k = FrontIdx; k < BackIdx; k++)
							DoubleStr += data_list.at(k).data;
						break;
					}
				}
				idx--;
			}

			if(DoubleStr != "")
			{
				data_list.at(FrontIdx).size = data_list.at(BackIdx-1).size;
				data_list.at(FrontIdx).data = DoubleStr;
				data_list.erase(data_list.begin()+FrontIdx+1, data_list.begin()+BackIdx);
				DoubleStr = "";
			}
			if(idx != 0)
				continue;
		}
		idx = 0;
		FrontIdx = 0;
		BackIdx = 0;
		DoubleStr = "";
	}
	return;
}
void Upload::SquareFunc()
{
	if(data_list.size() == 0)
		return;

	int Square_Idx = 0;
	if(data_list.size() <= 1)
		return;


	for(unsigned int i = 0; i <data_list.size(); i++)//제곱근을 찾아 '^'문자를 삽입
	{
		if(atof(data_list.at(i).data.c_str()) < -1 ||
			*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' 
			&&*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/'
			&&*data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != '.')//idx i의 값이 정수형이여야한.
		{
			if(data_list.size() >= 2 &&  i+1 < data_list.size())
			{
				if(*data_list.at(i+1).data.c_str() != '+' && *data_list.at(i+1).data.c_str() != '-' 
					&&*data_list.at(i+1).data.c_str() != '*' && *data_list.at(i+1).data.c_str() != '/'
					&&*data_list.at(i+1).data.c_str() != '(' && *data_list.at(i).data.c_str() != '.')//문자 다음으로 숫자만 와야 함
				{
					if(i+1 == data_list.size())
						continue;

					if( ((data_list.at(i).right_bottom.y - data_list.at(i).left_top.y)/2 + (data_list.at(i).left_top.y)) > data_list.at(i+1).right_bottom.y &&
						data_list.at(i).data != ".")//추가
					{
						Square_Idx++;
						Label_Data temp;
						temp.data = "^";
						if(*data_list.at(i+1).data.c_str() == ')')
						{
							data_list.insert(data_list.begin() + i + 2, temp);
							Square_Check = true;
						}
						else
						{
							data_list.insert(data_list.begin() + i + 1, temp);
							Square_Check = true;
						}
					}
					else
						continue;
				}
			}
		}
	}

	if(!Square_Check)
		return;

	for(int i = 0; i < Square_Idx; i++)
	{
		int y = -1;
		int BackIdx=-1;
		int FrontIdx=-1;
		int idx=-1;   
		string Big_Str = "";
		string Small_Str = "";   
		double Big_Double = 0;
		double Small_Double = 0;

		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(data_list.at(i).data == "^")
			{
				idx = i;
				if(i-1 == 0)
				{
					FrontIdx = 0;
					if(data_list.at(i-1).data == "-")
						Big_Str += data_list.at(i-1).data;
					break;
				}
				for(unsigned int B = i-1; B >= 0; B--)
				{
					if(B == 0 && FrontIdx == -1)
					{
						FrontIdx = 0;
						break;
					}
					if(data_list.at(B).data == "(" && FrontIdx == -1)
					{
						FrontIdx = B+1;
						break;
					}
					if(data_list.at(B).data == "-" && FrontIdx == -1)
					{
						if(B != 0)
						{
							if(data_list.at(B-1).data == "+" || data_list.at(B-1).data == "*" 
								|| data_list.at(B-1).data == "/" || data_list.at(B-1).data == "(" && FrontIdx == -1)
							{
								FrontIdx = B;
								break;      
							}
							else
							{
								FrontIdx = B+1;
								break;      
							}
						}
						else
						{
							FrontIdx = B;
							break;
						}
					}

					if(data_list.at(B).data == "+" || data_list.at(B).data == "*" || data_list.at(B).data == "/" && FrontIdx == -1)
					{
						if(B==0)
						{
							FrontIdx = 0;
							break;
						}
						else
						{
							FrontIdx = B+1;
							break;
						}
					}
				}
			}
			if(FrontIdx != -1)
				break;
		}

		for(unsigned int j = FrontIdx+1; j < data_list.size(); j++)
		{
			if(data_list.at(j).data == "^" && BackIdx == -1)
				continue;

			if(data_list.at(j).data == ")" && BackIdx == -1)
			{
				BackIdx = j;            
				break;
			}

			if(j == data_list.size()-1 && BackIdx == -1)
			{
				BackIdx = data_list.size();            
				break;
			}

			if(data_list.at(j).data == "+" || data_list.at(j).data == "-" 
				|| data_list.at(j).data == "*" || data_list.at(j).data == "/"  || data_list.at(j).data == "^" && BackIdx == -1)
			{
				BackIdx = j;               
				break;
			}
		}


		for(int i = FrontIdx; i < idx; i++)
			Big_Str += data_list.at(i).data.c_str();

		for(int i = idx+1; i < BackIdx; i++)
		{
			if(y == -1)
				y = data_list.at(i).left_top.y;
			Small_Str += data_list.at(i).data.c_str();
		}

		if(strstr((char*)Small_Str.c_str(),"+") != NULL || 
			strstr((char*)Small_Str.c_str(),"-") != NULL ||
			strstr((char*)Small_Str.c_str(),"*") != NULL ||
			strstr((char*)Small_Str.c_str(),"/") != NULL ||
			strstr((char*)Small_Str.c_str(),"(") != NULL ||
			strstr((char*)Small_Str.c_str(),")") != NULL ||
			strstr((char*)Small_Str.c_str(),"^") != NULL ||
			strstr((char*)Small_Str.c_str(),".") != NULL)
		{
			idx=-1;
			Big_Str = "";
			Small_Str = "";
			Big_Double = 0;
			Small_Double = 0;
			BackIdx=0;
			FrontIdx=0;
			data_list.clear();
			return;
		}

		Big_Double = atof(Big_Str.c_str());
		Small_Double = atof(Small_Str.c_str());

		double result = Calculation(Big_Double, '*', Big_Double);

		for(int i = 0; i < Small_Double-2; i++)
			result = Calculation(result, '*', Big_Double);

		int result1 = result;
		if(result - result1 == 0)
		{	
			if(result1 == 0)
			{
				data_list.at(FrontIdx).data = to_string(0);
				data_list.erase(data_list.begin()+FrontIdx+1,data_list.begin()+BackIdx);
			}
			else
			{
				data_list.at(FrontIdx).data = to_string(result1);
				data_list.erase(data_list.begin()+FrontIdx+1,data_list.begin()+BackIdx);
			}
		}
		else
		{
			if(result == 0)
			{
				data_list.at(FrontIdx).data = to_string(0);
				data_list.erase(data_list.begin()+FrontIdx+1,data_list.begin()+BackIdx);
			}
			else
			{
				data_list.at(FrontIdx).data = to_string(result);
				data_list.erase(data_list.begin()+FrontIdx+1,data_list.begin()+BackIdx);
			}
		}

		idx=-1;
		Big_Double = 0;
		Small_Double = 0;
		result = 0;
		BackIdx=0;
		FrontIdx=0;
		string::iterator iter;
		int find=0;

		if(atof(Big_Str.c_str()) != 0)/////
		{
			for(unsigned int i = 0; i < Final_Modification_Print.size(); i++)
			{
				if(atof(Big_Str.c_str()) < 0)
				{
					for(iter = Big_Str.begin(); iter != Big_Str.end(); iter++, find++)
					{
						if(*iter == '-')
						{
							Big_Str.erase(find,1);
							break;
						}
					}
					find = 0;
					for(unsigned int i = 0; i < Final_Modification_Print.size(); i++)				
					{
						if(Final_Modification_Print.at(i).left_top.y == y)
						{
							Label_Data L;
							L.data			= "^";
							Final_Modification_Print.insert(Final_Modification_Print.begin() + i, L);
							break;
						}
					}
					break;
				}
				else
				{
					for(unsigned int i = 0; i < Final_Modification_Print.size(); i++)				
					{
						if(Final_Modification_Print.at(i).left_top.y == y)//추가변경
						{
							Label_Data L;
							L.data			= "^";
							Final_Modification_Print.insert(Final_Modification_Print.begin() + i, L);
							break;
						}
					}
					break;
				}
			}
		}
		Big_Str = "";
		Small_Str = "";
		y = -1;
	}
	return;

}														
void Upload::SDFEFunc()
{
	Final_Modification_Print.clear();

	if(data_list.size() == 0)
		return;

	int num = 0;

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == "-" )
			num++;
	}
	BOOL SubOrFoun_Check = false;
	for(int S = 0; S < num; S++)
	{
		int frontnum=-1;
		int backnum=-1;

		int SubOrFoun_idx=0;

		string denominator_low = "";
		string numerator_high = "";

		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(SubOrFoun_Check)
				SubOrFoun_Check = false;

			// 분수
			if(data_list.at(i).data == "-" )
			{
				for(unsigned int SDFEIdx = 0; SDFEIdx < data_list.size(); SDFEIdx++)
				{
					if(data_list.at(SDFEIdx).data == "+" || data_list.at(SDFEIdx).data == "-" 
						|| data_list.at(SDFEIdx).data == "*" || data_list.at(SDFEIdx).data == "/"|| data_list.at(SDFEIdx).data == ".")
						continue;

					if(data_list.at(SDFEIdx).right_bottom.y > data_list.at(i).right_bottom.y)
					{
						if( data_list.at(i).left_top.x <     (data_list.at(SDFEIdx).left_top.x + data_list.at(SDFEIdx).right_bottom.x)/2 && 
							data_list.at(i).right_bottom.x > (data_list.at(SDFEIdx).left_top.x + data_list.at(SDFEIdx).right_bottom.x)/2)
						{
							SubOrFoun_Check = true;
							break;
						}
					}                  
				}

				if(!SubOrFoun_Check)
					continue;

				SubOrFoun_idx = i;
				for(unsigned int B = i; B >= 0; B--)
				{ 
					if(data_list.at(B).data == "+" || data_list.at(B).data == "-" 
						|| data_list.at(B).data == "*" || data_list.at(B).data == "/" || data_list.at(B).data == "(" || B == 0)
					{
						if(data_list.at(B).data == "(")
						{
							frontnum = B+1;
							break;
						}

						if(B==0)
						{
							frontnum = B;
							break;
						}
						if(B==data_list.size()-1)
						{
							if(*data_list.at(data_list.size()-1).data.c_str() == '-')
							{
								for(unsigned int C = B-1; C >= 0; C--)
								{ 
									if(data_list.at(C).data == "+" || data_list.at(C).data == "-" 
										|| data_list.at(C).data == "*" || data_list.at(C).data == "/" || data_list.at(C).data == "(" || C == 0)
									{
										frontnum = C+1;
										break;
									}
								}
							}
							else
								continue;
						}
						else if(data_list.at(i).left_top.x == data_list.at(B).left_top.x)
							continue;
						else
							frontnum = B+1;
						break;
					}
				}

				for(unsigned int j = frontnum; j < data_list.size(); j++)
				{
					if(data_list.at(j).data == "+" || data_list.at(j).data == "-" 
						|| data_list.at(j).data == "*" || data_list.at(j).data == "/" ||  data_list.at(j).data == ")" ||j == data_list.size()-1)
					{
						if(data_list.at(j).data == ")")
						{
							backnum = j;
							break;
						}

						if(j == data_list.size()-1)
						{
							j++;                  
							backnum = j;
							break;
						}
						if(j == SubOrFoun_idx)
							continue;
						backnum = j;
						break;
					}
				}
				break;
			}
		}

		for(int i = frontnum; i < backnum; i++)
		{
			if(data_list.at(SubOrFoun_idx).left_top.y > data_list.at(i).left_top.y 
				|| data_list.at(SubOrFoun_idx).left_top.y == data_list.at(i).left_top.y)// - 빼기
			{
				if(i == backnum-1)
					return;
				else
					continue;
			}
			else
			{
				SubOrFoun_Check = true;
				break;
			}
			return;
		}
		if(SubOrFoun_Check)
		{
			for(int i = frontnum; i < backnum; i++)
			{
				if(data_list.at(SubOrFoun_idx).left_top.y < data_list.at(i).left_top.y
					&&*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' 
					&&*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/'
					&&*data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != ')')
				{
					if(i == SubOrFoun_idx)
						continue;
					denominator_low += data_list.at(i).data.c_str();
					if(denominator_low=="")
						return;
				}
				else if(*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' 
					&&*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/'
					&&*data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != ')')
				{
					if(i == SubOrFoun_idx)
						continue;
					numerator_high += data_list.at(i).data.c_str();
				}
			}
		}
		if(backnum == -1 || frontnum == -1)
			continue;

		int omsint = backnum - frontnum;
		if(omsint == 3)
		{
			double Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));

			int Calculation_Int;
			Calculation_Int = Calculation_Double;
			if(Calculation_Double - Calculation_Int == 0)
			{
				data_list.at(frontnum).data = to_string(Calculation_Int);
				data_list.erase(data_list.begin()+frontnum+1,data_list.begin()+frontnum+3);
				Calculation_Double = 0;
				Calculation_Int = 0;
			}
			else
			{
				data_list.at(frontnum).data = to_string(Calculation_Double);
				data_list.erase(data_list.begin()+frontnum+1,data_list.begin()+frontnum+3);
				Calculation_Double = 0;
				Calculation_Int = 0;
			}
		}
		else
		{
			omsint -=3;
			if(omsint<0)
				break;
			data_list.erase(data_list.begin()+frontnum,data_list.begin()+frontnum+omsint);
			double Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
			int Calculation_Int;
			Calculation_Int = Calculation_Double;
			if(Calculation_Double - Calculation_Int == 0)
			{
				data_list.at(frontnum).data = to_string(Calculation_Int);
				data_list.erase(data_list.begin()+frontnum+1,data_list.begin()+frontnum+3);
				Calculation_Double = 0;
				Calculation_Int = 0;				
			}
			else
			{
				data_list.at(frontnum).data = to_string(Calculation_Double);
				data_list.erase(data_list.begin()+frontnum+1,data_list.begin()+frontnum+3);
				Calculation_Double = 0;
				Calculation_Int = 0;				
			}
		}		

		frontnum=-1;
		backnum=-1;

		SubOrFoun_idx=0;
		denominator_low = "";
		numerator_high = "";
	}
	for(unsigned int i = 0; i < data_list.size(); i++)//괄호 갯수로 return 여부 결정
		Final_Modification_Print.push_back(data_list.at(i));
}
void Upload::BracketFunc()
{
	if(data_list.size() == 0)
		return;

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == '(')
		{
			if( data_list.at(i+1).data == "+" || data_list.at(i+1).data == "/" || 
				data_list.at(i+1).data == "-" || data_list.at(i+1).data == "*" || 
				data_list.at(i+1).data == "." || data_list.at(i+1).data == "^")
			{
				data_list.clear();
				return;
			}
		}
		else if(*data_list.at(i).data.c_str() == ')')
		{
			if(i != 0)
				if( data_list.at(i-1).data == "+" || data_list.at(i-1).data == "-" || 
					data_list.at(i-1).data == "/" || data_list.at(i-1).data == "*" ||
					data_list.at(i-1).data == "." || data_list.at(i-1).data == "^")
				{
					data_list.clear();
					return;
				}
		}
	}

	int FrontIdx = -1;
	int BackIdx = -1;
	string Return_Operation_Value = "";
	int idx = 0;
	bool Open_Bracket_Check = false;
	bool Close_Bracket_Check = false;

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(i < data_list.size()-1)
			if(*data_list.at(i).data.c_str() == '(' )
			{
				if(*data_list.at(i+1).data.c_str() == ')' )
				{
					data_list.clear();
					return;
				}
			}
	}

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == '(' )
			Open_Bracket_Check = true;
		if(*data_list.at(i).data.c_str() == ')')
			Close_Bracket_Check = true;
		if(Open_Bracket_Check == false && Close_Bracket_Check == true)
		{
			data_list.clear();
			return;
		}
	}

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == '(' || *data_list.at(i).data.c_str() == ')')
			idx++;
	}
	idx/=2;

	if(idx != 0)
	{
		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '(')
			{
				if(*data_list.at(i+2).data.c_str() == ')')
				{
					data_list.erase(data_list.begin()+i);
					data_list.erase(data_list.begin()+i+1);
					idx--;
				}
			}
		}
	}

	if(idx == 0)
		return;

	for (int o = 0; o < idx; o++)
	{
		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '(')
				FrontIdx = i;

			if(*data_list.at(i).data.c_str() == ')' && BackIdx == -1)
				BackIdx = i;

			if(FrontIdx != -1 && BackIdx != -1)
				break;
		}

		if(FrontIdx == -1 || BackIdx == -1)
			return;

		if(BackIdx - FrontIdx == 2)
		{
			data_list.erase(data_list.begin()+FrontIdx);
			data_list.erase(data_list.begin()+FrontIdx+1);
		}
		else
		{
			Return_Operation_Value = Middle_Final_Operation(FrontIdx+1, BackIdx);//인덱스 3 부터 6까지 연산 실행 함
			int Return_Operation_Value_int = -1;
			Return_Operation_Value_int = atof(Return_Operation_Value.c_str());
			if(Return_Operation_Value_int == atof(Return_Operation_Value.c_str()))
				Return_Operation_Value = to_string(Return_Operation_Value_int);
			data_list.at(FrontIdx).data = Return_Operation_Value;
			data_list.erase(data_list.begin()+FrontIdx+1,data_list.begin()+BackIdx+1);
			FrontIdx = -1;
			BackIdx = -1;
		}
	}
}
double Upload::Calculation(double a, char op, double b)
{
	switch(op)
	{
	case '+': return a+b; break;
	case '-': return a-b; break;
	case '*': return a*b; break;
	case '/': return a/b; break;
	}

	return NULL;
}
string Upload::Middle_Final_Operation(int FristIdx, int SecondIdx)
{   
	if(data_list.size() == 0)
		return "";

	string Middle_Value = "";

	vector<string>Middle_Operation_Vector;

	for(int i = FristIdx; i < SecondIdx; i++)
		Middle_Operation_Vector.push_back(data_list.at(i).data);

	for(unsigned int i = 1; i < Middle_Operation_Vector.size(); i+=2)
	{
		if(Middle_Operation_Vector.size() == 1)
		{
			Middle_Value = Middle_Operation_Vector.at(0);
			Middle_Operation_Vector.clear();
			return Middle_Value;
		}

		int Middle_Value_int;
		if(*Middle_Operation_Vector.at(i).c_str() == '*')
		{
			Middle_Value = to_string(Calculation(atof(Middle_Operation_Vector.at(i-1).c_str()), *Middle_Operation_Vector.at(i).c_str(), atof(Middle_Operation_Vector.at(i+1).c_str())));
			Middle_Value_int = atoi(Middle_Value.c_str());
			if(atof(Middle_Value.c_str()) - Middle_Value_int == 0)
			{
				Middle_Operation_Vector.at(i-1) = to_string(Middle_Value_int);
				Middle_Operation_Vector.erase(Middle_Operation_Vector.begin()+i,Middle_Operation_Vector.begin()+i+2);
				Middle_Value = "";
				i=-1;
			}
			else
			{
				Middle_Operation_Vector.at(i-1) = Middle_Value;
				Middle_Operation_Vector.erase(Middle_Operation_Vector.begin()+i,Middle_Operation_Vector.begin()+i+2);
				Middle_Value = "";
				i=-1;
			}

		}
		else if(*Middle_Operation_Vector.at(i).c_str() == '/')
		{
			Middle_Value = to_string(Calculation(atof(Middle_Operation_Vector.at(i-1).c_str()), *Middle_Operation_Vector.at(i).c_str(), atof(Middle_Operation_Vector.at(i+1).c_str())));
			Middle_Value_int = atoi(Middle_Value.c_str());
			if(atof(Middle_Value.c_str()) - Middle_Value_int == 0)
			{
				Middle_Operation_Vector.at(i-1) = to_string(Middle_Value_int);
				Middle_Operation_Vector.erase(Middle_Operation_Vector.begin()+i,Middle_Operation_Vector.begin()+i+2);
				Middle_Value = "";
				i=-1;
			}
			else
			{
				Middle_Operation_Vector.at(i-1) = Middle_Value;
				Middle_Operation_Vector.erase(Middle_Operation_Vector.begin()+i,Middle_Operation_Vector.begin()+i+2);
				Middle_Value = "";
				i=-1;
			}
		}
	}
	
	int Middle_Value_int;
	for(unsigned int i = 1; i < Middle_Operation_Vector.size(); i+=2)
	{
		if(Middle_Operation_Vector.size() == 1)
		{
			Middle_Value = Middle_Operation_Vector.at(0);
			Middle_Operation_Vector.clear();
			return Middle_Value;
		}

		if(*Middle_Operation_Vector.at(i).c_str() == '+')
		{
			Middle_Value = to_string(Calculation(atof(Middle_Operation_Vector.at(i-1).data()), *Middle_Operation_Vector.at(i).c_str(), atof(Middle_Operation_Vector.at(i+1).data())));
			Middle_Value_int = atoi(Middle_Value.c_str());
			if(atof(Middle_Value.c_str()) - Middle_Value_int == 0)
			{
				Middle_Operation_Vector.at(i-1) = to_string(Middle_Value_int);
				Middle_Operation_Vector.erase(Middle_Operation_Vector.begin()+i,Middle_Operation_Vector.begin()+i+2);
				Middle_Value = "";
				i=-1;
			}
			else
			{
				Middle_Operation_Vector.at(i-1) = Middle_Value;
				Middle_Operation_Vector.erase(Middle_Operation_Vector.begin()+i,Middle_Operation_Vector.begin()+i+2);
				Middle_Value = "";
				i=-1;
			}
		}
		else if(*Middle_Operation_Vector.at(i).c_str() == '-')
		{
			Middle_Value = to_string(Calculation(atof(Middle_Operation_Vector.at(i-1).c_str()), *Middle_Operation_Vector.at(i).c_str(), atof(Middle_Operation_Vector.at(i+1).c_str())));
			Middle_Value_int = atoi(Middle_Value.c_str());
			if(atof(Middle_Value.c_str()) - Middle_Value_int == 0)
			{
				Middle_Operation_Vector.at(i-1) = to_string(Middle_Value_int);
				Middle_Operation_Vector.erase(Middle_Operation_Vector.begin()+i,Middle_Operation_Vector.begin()+i+2);
				Middle_Value = "";
				i=-1;
			}
			else
			{
				Middle_Operation_Vector.at(i-1) = Middle_Value;
				Middle_Operation_Vector.erase(Middle_Operation_Vector.begin()+i,Middle_Operation_Vector.begin()+i+2);
				Middle_Value = "";
				i=-1;
			}
		}
	}
	Middle_Value = Middle_Operation_Vector.at(0);
	Middle_Operation_Vector.clear();
	Result_Value = Middle_Value;
	return Middle_Value;
}
void Upload::Final_Set()
{
	if(data_list.size() == 0)
		return;

	if(data_list.size() == 1)
		if((*data_list.at(0).data.c_str() != '+' || *data_list.at(0).data.c_str() != '-' 
			|| *data_list.at(0).data.c_str() != '*' || *data_list.at(0).data.c_str() != '/'
			|| *data_list.at(0).data.c_str() != '(') || *data_list.at(0).data.c_str() != ')')
			return;

	string LTH = "";
	int FrontNum = -1;
	int BackNum = -1;

	if(data_list.size() == 3 && 
		(*data_list.at(0).data.c_str() != '+' || *data_list.at(0).data.c_str() != '-' || *data_list.at(0).data.c_str() != '*' || 
		*data_list.at(0).data.c_str() != '/' || *data_list.at(0).data.c_str() != '(') && 

		(*data_list.at(1).data.c_str() == '+' || *data_list.at(1).data.c_str() == '-' || *data_list.at(1).data.c_str() == '*' || 
		*data_list.at(1).data.c_str() == '/') &&

		(*data_list.at(2).data.c_str() != '+' || *data_list.at(2).data.c_str() != '-' || *data_list.at(2).data.c_str() != '*' || 
		*data_list.at(2).data.c_str() != '/' || *data_list.at(2).data.c_str() != '(' || *data_list.at(2).data.c_str() != ')'))
		return;
	while(1)//십의 자리 이상 숫자 들을 묶는 while문
	{
		for(unsigned int i = 0; i < data_list.size()-1; i++)//십의 자리 이상 숫자 들을 묶는 for문
		{
			if((*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' && *data_list.at(i).data.c_str() != '*' && 
				*data_list.at(i).data.c_str() != '/' && *data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != ')') &&
				atof(data_list.at(i).data.c_str()) >= 0 && atof(data_list.at(i).data.c_str()) < 10)
			{
				if((*data_list.at(i+1).data.c_str() != '+' && *data_list.at(i+1).data.c_str() != '-' && *data_list.at(i+1).data.c_str() != '*' && 
					*data_list.at(i+1).data.c_str() != '/' && *data_list.at(i+1).data.c_str() != '(' && *data_list.at(i+1).data.c_str() != ')') &&
					atof(data_list.at(i+1).data.c_str()) >= 0 && atof(data_list.at(i+1).data.c_str()) < 10)
				{
					if(FrontNum == -1)
						FrontNum = i;
				}         
			}
		}
		for(unsigned int i = FrontNum; i < data_list.size(); i++)//십의 자리 이상 숫자 들을 묶는 for문
		{
			if(*data_list.at(i).data.c_str() == '+' || *data_list.at(i).data.c_str() == '-' || *data_list.at(i).data.c_str() == '*' || 
				*data_list.at(i).data.c_str() == '/' || *data_list.at(i).data.c_str() == ')' || i == data_list.size()-1)
			{
				if(*data_list.at(i).data.c_str() == ')')
				{
					BackNum = i;
					break;
				}
				if(i == data_list.size()-1)
					i++;
				BackNum = i;
				break;
			}
		}
		for (int i = FrontNum; i < BackNum; i++)
			LTH+=data_list.at(i).data;
		if(FrontNum != -1 && BackNum != -1)//십의 자리 이상 숫자 들을 묶는 for문
		{
			data_list.at(FrontNum).data = LTH;
			data_list.erase(data_list.begin()+FrontNum+1, data_list.begin() +BackNum);
			LTH = "";
		}
		if(FrontNum == -1 || BackNum == -1)
		{         
			FrontNum = -1;
			BackNum = -1;
			break;
		}
		FrontNum = -1;
		BackNum = -1;
	}



	if(data_list.size() == 3 && 
		(*data_list.at(0).data.c_str() != '+' || *data_list.at(0).data.c_str() != '-' || *data_list.at(0).data.c_str() != '*' || 
		*data_list.at(0).data.c_str() != '/' || *data_list.at(0).data.c_str() != '(') && 

		(*data_list.at(1).data.c_str() == '+' || *data_list.at(1).data.c_str() == '-' || *data_list.at(1).data.c_str() == '*' || 
		*data_list.at(1).data.c_str() == '/') &&

		(*data_list.at(2).data.c_str() != '+' || *data_list.at(2).data.c_str() != '-' || *data_list.at(2).data.c_str() != '*' || 
		*data_list.at(2).data.c_str() != '/' || *data_list.at(2).data.c_str() != '(' || *data_list.at(2).data.c_str() != ')'))
		return;

	string Operator_And_Operand = "";
	int OperandIdx=-1;

	for(unsigned int i = 0; i < data_list.size(); i++)//음의 정수를 찾는 for문
	{
		if(*data_list.at(i).data.c_str() == '-')
		{
			if(i > 0)
				if( *data_list.at(i-1).data.c_str() != '+' && *data_list.at(i-1).data.c_str() != '-' &&
					*data_list.at(i-1).data.c_str() != '*' && *data_list.at(i-1).data.c_str() != '/' &&
					*data_list.at(i-1).data.c_str() != '(' && *data_list.at(i-1).data.c_str() != ')' &&
					*data_list.at(i+1).data.c_str() == '+' && *data_list.at(i+1).data.c_str() == '-' &&
					*data_list.at(i+1).data.c_str() == '*' && *data_list.at(i+1).data.c_str() == '/' &&
					*data_list.at(i+1).data.c_str() == '(' && *data_list.at(i+1).data.c_str() == ')')
					continue;

			if(i == 1)
			{
				/*OperandIdx = i;
				do
				{
					Operator_And_Operand += data_list.at(i).data;
					i++;
					if(i==data_list.size())break;
				}while(*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' && 
					*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/' && 
					*data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != ')'
					&& (strstr((char*)data_list.at(i-2).data.c_str(),"-") != NULL));
				data_list.at(OperandIdx).data = Operator_And_Operand;
				Operator_And_Operand = "";
				data_list.erase(data_list.begin()+OperandIdx+1,data_list.begin()+i);
				i = OperandIdx;
				continue;*/
			}

			if(i != 0)
			{
				if((*data_list.at(i-1).data.c_str() == '+' || *data_list.at(i-1).data.c_str() == '/' || 
					*data_list.at(i-1).data.c_str() == '-' || *data_list.at(i-1).data.c_str() == '*' || 
					*data_list.at(i-1).data.c_str() == '(') && (strstr((char*)data_list.at(i-1).data.c_str(),"-") == NULL))
				{
					OperandIdx = i;
					do
					{
						Operator_And_Operand += data_list.at(i).data;
						i++;
						if(i==data_list.size())break;
					}while(*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' && 
						*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/' && 
						*data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != ')');
					data_list.at(OperandIdx).data = Operator_And_Operand;
					Operator_And_Operand = "";
					data_list.erase(data_list.begin()+OperandIdx+1,data_list.begin()+i);
					i = OperandIdx;
					continue;
				}
				else
					continue;
			}
			else
			{
				OperandIdx = i;
				do
				{
					Operator_And_Operand += data_list.at(i).data;
					i++;
					if(i==data_list.size())break;
				}while(*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' && 
					*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/' && *data_list.at(i).data.c_str() != ')');
				data_list.at(OperandIdx).data = Operator_And_Operand;
				Operator_And_Operand = "";
				data_list.erase(data_list.begin()+OperandIdx+1,data_list.begin()+i);
				i = OperandIdx;
				continue;
			}
		}
	}
}
void Upload::Result_Print(IplImage* original, int x, int y, int width, int height)
{
	//출력과정에 필요한 변수 선언 및 초기화
	CvFont font;
	int hscale = 1;
	int vscale = 1;
	int linewidth = 2;
	int my = y-5;
	int rx = width/2;
	int ry = y + height + 40;  
	string str = "";
	//출력과정 필요한 변수 선언 및 초기화

	//수식출력
	for(unsigned int i = 0; i < Final_Modification_Print.size(); i++)
	{
		Formula_Eq += Final_Modification_Print.at(i).data;
		str += Final_Modification_Print.at(i).data;
	}	
	Formula_Eq+="=";

	for(unsigned int i = 0; i < data_list.size(); i++)
		Answer_Eq = Result_Value;

	char* rs = (char*)Result_Value.c_str();

	//결과출력
	int size = 20;

	//double형의 범위를 벗어날때
	if(Result_Value.size() >= 30)
	{
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
		cvPutText(original, "Out of range", cvPoint(rx-size, ry), &font, cvScalar(0,0,255));
		data_list.clear();
		return;
	}

	str += "=";
	str += Result_Value;
	char* ss = (char*)str.c_str();
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
	cvPutText(original, ss, cvPoint(x, my), &font, cvScalar(0,0,255));
}
void Upload::Loadimage_Preprocessing(IplImage* original, int x, int y, int width, int height)
{
	vector<Label_Data> formula;
	vector<Label_Data> data;
	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		data.push_back(data_list.at(i));
	}

	int line	= 0;	//연산자의 가운데선
	int ty		= 0;	//위쪽 라인값
	int by		= 0;	//아래쪽 라인값
	int y_opsize= 0;	//피연산자의 크기
	int x_opsize= 0;	//피연산자의 크기
	int count	= 0;	
	//1.피연산자의 크기
	for(unsigned int i = 0; i < data.size(); i++)
	{
		//y-size
		if(*data.at(i).data.c_str() >= 48 && *data.at(i).data.c_str() <= 57)
		{
			y_opsize += data.at(i).right_bottom.y - data.at(i).left_top.y;
			x_opsize += data.at(i).right_bottom.x - data.at(i).left_top.x;
			count++;
		}
	}
	if(y_opsize == 0) return;

	y_opsize = y_opsize / count;	//y라인크기
	x_opsize = x_opsize / count;	//글자크기

	//2. 라인찾기
	for(unsigned int i = 0; i < data.size(); i++)
	{
		if(data.at(i).data != "")
		{
			if(*data.at(i).data.c_str() == '+' || *data.at(i).data.c_str() == '*' || *data.at(i).data.c_str() == '-')
			{
				line = data.at(i).left_top.y + (data.at(i).right_bottom.y - data.at(i).left_top.y)/2;	//연산자의 중앙점.
			}
			//경계선을 만든다
			if(line != 0)
			{
				ty = line - y_opsize;
				by = line + y_opsize;

				if(ty < 0)
					ty = 0;
				if(by > original->height)
					by = original->height;

				//경계선 내부의 모든 레이블이미지를 찾는다
				for(unsigned int j = 0; j < data.size(); j++)
				{
					int half_point = data.at(j).left_top.y + (data.at(j).right_bottom.y - data.at(j).left_top.y)/2;
					if(half_point >= ty && half_point <= by)
					{
						Label_Data L;
						L.data			= data.at(j).data;
						L.left_top		= data.at(j).left_top;
						L.right_bottom	= data.at(j).right_bottom;
						L.size			= data.at(j).size;
						formula.push_back(L);
						data.at(j).data = " ";
					}
				}

				//x값으로 정렬
				for(unsigned int k = 0; k <formula.size()-1; k++)
				{
					for(unsigned int j = 0; j < formula.size()-1; j++)
					{
						if(formula.at(j).left_top.x > formula.at(j+1).left_top.x)
						{
							Label_Data temp2 = formula.at(j);
							formula.at(j) = formula.at(j+1);
							formula.at(j+1) = temp2;
						}
					}
				}

				int idx[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
				count = 0;
				//정렬 후
				for(unsigned int j = 0; j < formula.size(); j++)
				{
					if(formula.size() -1 > j)
						if((formula.at(j+1).left_top.x - formula.at(j).left_top.x) >= x_opsize*1.7)
						{
							idx[count] = j;	
							count++;
						}

				}
				if(count)
				{
					for(int j = 0; j < count; j++)
					{
						Label_Data L;
						L.data			= "&";
						L.size			= 0;
						formula.insert(formula.begin() + idx[j]+1, L);
					}
				}

				//마지막에 넣어준다
				Label_Data L;
				L.data			= "&";
				L.size			= 0;
				formula.push_back(L);
				data_list.clear();

				for(unsigned int j = 0; j < formula.size(); j++)
				{
					if(formula.at(j).data != "&")
					{
						data_list.push_back(formula.at(j));				
					}
					else
					{
						if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
						{
							data_list.clear();
							return;
						}
						Operation_Run(original, data_list.at(0).left_top.x, line + x_opsize, width, height);
						data_list.clear();
					}
				}

				formula.clear();
				line = 0;
			}
		}
	}
	data.clear();
	data_list.clear();
	formula.clear();
}
void Upload::Operation_Run(IplImage* original, int x, int y, int width, int height)//소수점, 제곱, 분수, 괄호
{
	BOOL IsEquation = FALSE;	//방정식
	BOOL isSCT		= FALSE;	//삼각함수
	int Bracket_Idx	= 0;

	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		Error_Printing(original);
		return;
	}
	if(data_list.size() == 2)//data_list vector에 size가 0이면 return함
	{
		if((*data_list.at(0).data.c_str() == '+' || 
			*data_list.at(0).data.c_str() == '*' || *data_list.at(0).data.c_str() == '/' ||
			*data_list.at(0).data.c_str() == '(' || *data_list.at(0).data.c_str() == ')' || 
			*data_list.at(0).data.c_str() == '.') && 
			(*data_list.at(1).data.c_str() == '+' || *data_list.at(1).data.c_str() == '-' ||
			*data_list.at(1).data.c_str() == '*' || *data_list.at(1).data.c_str() == '/' ||
			*data_list.at(1).data.c_str() == '(' || *data_list.at(1).data.c_str() == ')' || 
			*data_list.at(1).data.c_str() == '.'))
		{
			///data_list.clear();
			Error_Printing(original);
			return;
		}
	}
	if(data_list.size() == 1)//data_list vector에 size가 0이면 return함
	{
		if(*data_list.at(0).data.c_str() == '+' || *data_list.at(0).data.c_str() == '-' || 
			*data_list.at(0).data.c_str() == '*' || *data_list.at(0).data.c_str() == '/' ||
			*data_list.at(0).data.c_str() == '(' || *data_list.at(0).data.c_str() == ')' || 
			*data_list.at(0).data.c_str() == '.')
		{
			Error_Printing(original);
			return;
		}
	}

	for(unsigned int i = 0; i <data_list.size()-1; i++)//레이블링 한 요소들을 x값으로 정렬
	{
		for(unsigned int j = 0; j < data_list.size()-1; j++)
		{
			if(data_list.at(j).left_top.x > data_list.at(j+1).left_top.x)
			{
				Label_Data temp2 = data_list.at(j);
				data_list.at(j) = data_list.at(j+1);
				data_list.at(j+1) = temp2;
			}
		}
	}


	int OP=0;
	int CL=0;
	for(unsigned int i = 0; i < data_list.size(); i++)//괄호 갯수로 return 여부 결정
	{
		if(*data_list.at(i).data.c_str() == '(')
			OP++;
		if(*data_list.at(i).data.c_str() == ')')
			CL++;
	}
	if(OP != CL)
	{
		Error_Printing(original);
		 OP=0;
		 CL=0;
		return;
	}
	if((OP+CL)%2 == 1)
	{
		///data_list.clear();
		Error_Printing(original);
		Bracket_Idx=0;
		return;
	}

	if(Bracket_Idx > 0)
	{
		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '(')
			{
				if(	*data_list.at(i+1).data.c_str() == '.' || *data_list.at(i+1).data.c_str() == '^')
				{
					Error_Printing(original);
					return;
				}
			}
			else if(*data_list.at(i).data.c_str() == ')')
			{
				if(i != 0)
					if(*data_list.at(i-1).data.c_str() == '+' || *data_list.at(i-1).data.c_str() == '-' || 
						*data_list.at(i-1).data.c_str() == '/' || 
						*data_list.at(i-1).data.c_str() == '.' || *data_list.at(i-1).data.c_str() == '^')
					{
						Error_Printing(original);
						return;
					}
					else if(i != data_list.size()-1)
					{
						if(*data_list.at(i+1).data.c_str() == '.' || *data_list.at(i+1).data.c_str() == '^')
						{
							Error_Printing(original);
							return;
						}
					}
			}
		}
	}

	for(unsigned int j = 0; j < data_list.size(); j++)//나누기 연산자 맞지 않으면 끝남.
	{
		if(data_list.size() > (unsigned)j + 2)//
		{
			if(data_list.at(j).data == "-" && data_list.at(j+1).data == "." && data_list.at(j+2).data != ".")//나누기
			{
				//data_list.clear();
				Error_Printing(original);
				return;
			}         
		}
	}

	for(unsigned int i = 0; i < data_list.size(); i++)//-로 나누기 연산자 전처리, = 전처리
	{
		if(data_list.size() > i + 2)
		{
			if(data_list.at(i).data == "-" && data_list.at(i+1).data == "." && data_list.at(i+2).data == ".")//나누기
			{ 
				data_list.at(i).data = "/";
				data_list.erase(data_list.begin()+i+1,data_list.begin()+i+3);
				continue;
			}         
		}

		if((data_list.at(data_list.size()-1).data == "+") || (data_list.at(data_list.size()-1).data == "-") || 
			(data_list.at(data_list.size()-1).data == "."))
		{
			if(data_list.at(data_list.size()-1).data == "-" && data_list.at(data_list.size()-2).data == "-")
			{
				//원래 없는 부분임
			}
			else
			{
				Error_Printing(original);
				return;
			}
		}

		for(unsigned int j = 0; j < data_list.size(); j++)//나누기 연산자 맞지 않으면 끝남.
		{
			if(data_list.size() >= (unsigned)j + 2)
			{
				if(data_list.at(j).data == "-" && data_list.at(j+1).data == "." && data_list.at(j+2).data != ".")//나누기
				{
					///data_list.clear();
					Error_Printing(original);
					return;
				}         
			}
		}

		if(i < data_list.size()-1)// = 찾기
		{
			if(data_list.at(i).data == "-" && data_list.at(i+1).data == "-")
			{
				int middle = (data_list.at(i).left_top.x + data_list.at(i).right_bottom.x)/2;
				if(middle > data_list.at(i+1).left_top.x && data_list.at(i+1).right_bottom.x > middle)
				{
					data_list.at(i).data = "=";
					data_list.erase(data_list.begin()+i+1);
				}
			}
		}
	}

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == "-" && data_list.at(i+1).data == "-")
		{
			data_list.at(i).data = "+";
			data_list.erase(data_list.begin()+i+1);

		}
	}

	//연산자가 연속 3번 이상 나올 시 return(*제외);
	if(data_list.size() >= 3)
	{
		for(unsigned int i = 0; i < data_list.size()-2; i++)
		{
			if(*data_list.at(i).data.c_str() == '+' || *data_list.at(i).data.c_str() == '-' || *data_list.at(i).data.c_str() == '/')
			{
				if(*data_list.at(i+1).data.c_str() == '+' || *data_list.at(i+1).data.c_str() == '-' 
					|| *data_list.at(i+1).data.c_str() == '/' || *data_list.at(i+1).data.c_str() == '*')
				{
					if(*data_list.at(i+2).data.c_str() == '+' || *data_list.at(i+2).data.c_str() == '-' || *data_list.at(i+2).data.c_str() == '/'
						|| *data_list.at(i+2).data.c_str() == '.' || *data_list.at(i+2).data.c_str() == '*')
					{
						//data_list.clear();
						Error_Printing(original);
						Bracket_Idx=0;
						return;
					}
				}
			}
		}
	}


	//x를 넣고 방정식인지를 검사
	int E_INT = -1;
	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(i == 0)
		{
			for(unsigned int j = 0; j < data_list.size(); j++)
			{
				if(data_list.at(j).data == "=")
				{
					IsEquation = TRUE;
					if(j == 0 || j == data_list.size()-1)
					{
						IsEquation = FALSE;
						E_INT = j;
						break;
					}
				}
			}	
			if(!IsEquation && E_INT == -1)
				break;		
		}
		if(!IsEquation)
			break;	

		//방정식 검출
		bool BracketCheck = true; 
		if(data_list.size()-1 > i)
			if(data_list.at(i).data == "*")
			{
				for(unsigned int j = i; data_list.at(j).data != ")"; j--)
				{
					if(j==0)
					{
						BracketCheck = false;
						break;
					}

					if(data_list.at(j).data == "+" || data_list.at(j).data == "-" || data_list.at(j).data == "(" || 
						data_list.at(j).data == "/" || data_list.at(j).data == "=")
					{
						BracketCheck = false;
						break;
					}
				}

				if(BracketCheck)
					data_list.at(i).data = "x";

				if(i == 0)
				{
					if(data_list.at(i+1).data == "2")
					{
						data_list.at(i).data = "T";
						data_list.erase(data_list.begin()+i+1);
						IsEquation = TRUE;
					}
					else
					{
						data_list.at(i).data = "x";
						IsEquation = TRUE;
					}
				}
				else if(data_list.at(i+1).data == "+" || data_list.at(i+1).data == "-" || data_list.at(i+1).data == "*" ||
					data_list.at(i+1).data == "/"|| data_list.at(i+1).data == "+" || data_list.at(i+1).data == "(" ||
					data_list.at(i+1).data == ")"|| data_list.at(i+1).data == "=")
				{
					if(data_list.at(i+1).data == "-")
					{
						if(i+1 != data_list.size()-1)
						{
							if(data_list.at(i+2).data == "+" || data_list.at(i+2).data == "-" || 
								data_list.at(i+2).data == "*" || data_list.at(i+2).data == "/" || 
								data_list.at(i+2).data == "^" || data_list.at(i+2).data == ".")
							{
								Error_Printing(original);
								Bracket_Idx=0;
								return;
							}		
							else
							{
								data_list.at(i).data = "x";
								IsEquation = TRUE;
							}
						}
					}
					else
					{
						data_list.at(i).data = "x";
						IsEquation = TRUE;
					}
				}

				if(data_list.at(i+1).data != "+" && data_list.at(i+1).data != "-" && data_list.at(i+1).data != "*"	
					&&data_list.at(i+1).data != "/"	&& data_list.at(i+1).data != "+" && data_list.at(i+1).data != "(" 
					&&data_list.at(i+1).data != ")" && data_list.at(i+1).data != "=")
				{
					int m_height = (data_list.at(i).left_top.y + data_list.at(i).right_bottom.y)/2;
					if(m_height > data_list.at(i+1).left_top.y && m_height > data_list.at(i+1).right_bottom.y)
					{
						if(i != 0)
						{
							if(data_list.at(i-1).data == "-")
							{
								data_list.at(i).data = "-T";
								data_list.erase(data_list.begin()+i+1);
								data_list.erase(data_list.begin()+i-1);
								IsEquation = TRUE;
							}
							else 
							{
								data_list.at(i).data = "T";
								data_list.erase(data_list.begin()+i+1);
								IsEquation = TRUE;
							}
						}
						else
						{
							data_list.at(i).data = "T";
							data_list.erase(data_list.begin()+i+1);
							data_list.erase(data_list.begin()+i-1);
							IsEquation = TRUE;
						}
					}
				}
			}

			if(data_list.size()-1 == i)
			{
				if(data_list.at(i).data == "*")
				{
					data_list.at(i).data = "x";
					IsEquation = TRUE;
				}
			}

			//삼각함수 검출코드
	}

	for(unsigned int i = 0; i < data_list.size(); i++)//추가
	{
		if(*data_list.at(i).data.c_str() == 's' || *data_list.at(i).data.c_str() == 'c' 
			|| *data_list.at(i).data.c_str() == 't' || *data_list.at(i).data.c_str() =='n')
		{
			isSCT = TRUE;
		}
	}


	//x나 T 다음에 연산자가 오면 return
	for(unsigned int i = 0; i < data_list.size()-1; i++)
	{
		if(*data_list.at(i).data.c_str() == 'x' || *data_list.at(i).data.c_str() == 'T')
		{
			if(*data_list.at(i+1).data.c_str() != '+' && *data_list.at(i+1).data.c_str() != '-' && 
				*data_list.at(i+1).data.c_str() != '*' && *data_list.at(i+1).data.c_str() != '/' && 
				*data_list.at(i+1).data.c_str() != '=' && *data_list.at(i+1).data.c_str() != ')')
			{
				//data_list.clear();
				Error_Printing(original);
				return;
			}
		}
	}

	//연산자 전에 -가 오면 return
	for(unsigned int i = 1; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == "+" || data_list.at(i).data == "/" || data_list.at(i).data == "*"
			|| data_list.at(i).data == ")" || data_list.at(i).data == "." || data_list.at(i).data == "=")
		{
			if(data_list.at(i-1).data == "-")
			{
				//data_list.clear();
				Error_Printing(original);
				return;
			}
		}
	}

	//IsEquation요게 x나 T를 넣을때 넣으면 방정식 bool형 true
	if(IsEquation)
	{
		//방정식 함수 호출 부분
		bool IsEquationCheck = false;

		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == 'c' || *data_list.at(i).data.c_str() == 's' || 
				*data_list.at(i).data.c_str() == 't' || *data_list.at(i).data.c_str() == 'n' )
			{
				Error_Printing(original);
				return;
			}
		}

		if(*data_list.at(0).data.c_str() == '=' || *data_list.at(data_list.size()-1).data.c_str() == '=' )
		{
			Error_Printing(original);
			return;
		}

		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '=')
			{
				IsEquationCheck = true;
				break;
			}
		}

		if(!IsEquationCheck)
		{
			Error_Printing(original);
			return;
		}

		//방정식 함수 호출 부분
		int RqualIdx = -1;

		Eq_SDFEFunc();
		Eq_PointFunc();
		Eq_SquareFunc();
		Eq_Final_Set();
		Eq_BracketFunc();

		if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
		{
			//data_list.clear();
			Error_Printing(original);
			return;
		}


		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '=')
			{
				if(i == 0 || i == data_list.size()-1)
				{
					Error_Printing(original);
					return;
				}
				else if(*data_list.at(i-1).data.c_str() == '+' || data_list.at(i-1).data == "-" ||
					*data_list.at(i-1).data.c_str() == '*' || *data_list.at(i-1).data.c_str() == '/' 
					|| data_list.at(i-1).data == "0" || 
					data_list.at(i+1).data == "+" || data_list.at(i+1).data == "-" || 
					data_list.at(i+1).data == "*" || data_list.at(i+1).data == "/" )
				{
					Error_Printing(original);
					return;
				}
				else
					break;
			}
		}

		if(*data_list.at(0).data.c_str() == '=' || *data_list.at(data_list.size()-1).data.c_str() == '=')
		{
			Error_Printing(original);
			return;
		}

		//연산 전 오류 검출 - 끝

		//방전식 연산 시작
		for (unsigned int i = 0; i < data_list.size(); i++)
			data_list_print.push_back(data_list.at(i));

		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '=')
			{
				RqualIdx = i;
				Eq_Middle_Final_Operation(0, RqualIdx);
				break;
			}
		}
		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '=')
			{
				RqualIdx = i;
				Eq_Middle_Final_Operation(RqualIdx+1, data_list.size());
				break;
			}
		}

		if(Eq_Lvalue_Rvalue_Groupage())
			Eq_LinearEquation_Final_ArithmeticOperation();//2차 방정식 완료 함수
		else
		{
			for(unsigned int i = 0; i < data_list.size(); i+=2)
			{
				if((strstr((char*)data_list.at(i).data.c_str(),"x") != NULL))
					break;
			}
			Eq_SimpleEquation_Final_ArithmeticOperation();//1차 방정식 완료 함수   
		}

		if(data_list.size() == 0)
			return;
		Real_Final_Deduct(original, x, y, width, height);//결과 출력
		return;
	}

	if(isSCT)
	{
		//삼각함수 호출 부분
		if(!Trigonometrical_Function())
		{
			Error_Printing(original);
			return;
		}
		else
		{
			Result_Print(original, x, y, width, height);
			return;
		}
	}

	//연산자가 연속 3번 이상 나올 시 return;
	if(data_list.size() >= 3)
	{
		for(unsigned int i = 0; i < data_list.size()-2; i++)
		{
			if(*data_list.at(i).data.c_str() == '+' || *data_list.at(i).data.c_str() == '-' || 
				*data_list.at(i).data.c_str() == '*' || *data_list.at(i).data.c_str() == '/')
			{
				if(*data_list.at(i+1).data.c_str() == '+' || *data_list.at(i+1).data.c_str() == '-' || 
					*data_list.at(i+1).data.c_str() == '*' || *data_list.at(i+1).data.c_str() == '/')
				{
					if(*data_list.at(i+2).data.c_str() == '+' || *data_list.at(i+2).data.c_str() == '-' || 
						*data_list.at(i+2).data.c_str() == '*' || *data_list.at(i+2).data.c_str() == '/'
						|| *data_list.at(i+2).data.c_str() == '.')
					{
						Error_Printing(original);
						Bracket_Idx=0;
						return;
					}
				}
			}
		}
	}


	for(unsigned int i = 0; i < data_list.size()-1; i++)//소수점 앞 뒤로 문자가 올 경우 return;
	{
		if(*data_list.at(i).data.c_str() == '.')
		{
			if(i==0 || i == data_list.size()-1)
			{
				Error_Printing(original);
				return;
			}
			if(i > 0 && i < data_list.size())
				if(*data_list.at(i-1).data.c_str() == '+' || *data_list.at(i-1).data.c_str() == '-' || 
					*data_list.at(i-1).data.c_str() == '*' || *data_list.at(i-1).data.c_str() == '/' || 
					*data_list.at(i-1).data.c_str() == '(' || *data_list.at(i+1).data.c_str() == '+' ||
					*data_list.at(i+1).data.c_str() == '*' || *data_list.at(i+1).data.c_str() == '/' ||
					*data_list.at(i+1).data.c_str() == ')')
				{
					Error_Printing(original);
					Bracket_Idx=0;
					return;
				}
				else
					continue;
		}
	}

	if(data_list.at(0).data == "+" || data_list.at(0).data == "*" || 
		data_list.at(0).data == "/" || data_list.at(0).data == "^" || data_list.at(0).data == ".")
	{
		Error_Printing(original);
		return;
	}

	//==================전처리 함수(1.소수점, 2.제곱, 3.분수, 4.괄호) 호출 부==================↙
	for (unsigned int i = 0; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == "=" && i == data_list.size()-1)
			data_list.erase(data_list.begin() + i);
	}

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == 'c' || *data_list.at(i).data.c_str() == 's' || 
			*data_list.at(i).data.c_str() == 't' || *data_list.at(i).data.c_str() == 'n' )
		{
			Error_Printing(original);
			return;
		}
	}


	SDFEFunc();//-
	PointFunc();//소수점
	SquareFunc();//제곱
	Final_Set();//연산 전 최종 정리

	if(data_list.size() == 1 &&  !Square_Check &&
		(*data_list.at(0).data.c_str() != '+' || *data_list.at(0).data.c_str() != '-' ||
		*data_list.at(0).data.c_str() != '*' || 
		*data_list.at(0).data.c_str() != '/' || *data_list.at(0).data.c_str() != '('))
	{
		Error_Printing(original);
		return;
	}

	for (unsigned int i = 0; i < data_list.size(); i++)
		data_list_print.push_back(data_list.at(i));

	BracketFunc();//괄호

	if(data_list.size()>= 3)
		Middle_Final_Operation(0, data_list.size());

	if(data_list.size() == 1 && 
		(*data_list.at(0).data.c_str() != '+' || *data_list.at(0).data.c_str() != '-' || *data_list.at(0).data.c_str() != '*' || 
		*data_list.at(0).data.c_str() != '/' || *data_list.at(0).data.c_str() != '('))
		Result_Value = data_list.at(0).data;

	if(Result_Value == "")
	{
		Error_Printing(original);
		Bracket_Idx=0;
		return;
	}
	//==================전처리 함수(소수점, 제곱, 분수, 괄호) 호출 부==================↖
	Result_Print(original, x, y, width, height);
	return;
}
void Upload::Error_Printing(IplImage* original)
{
	if(data_list.size() == 0)
	{
		data_list.clear();
		return;
	}
	string str = "not a valid formula :";
	CvFont font;
	int width = original->width;
	int height = original->height;
	int hscale = 1;
	int vscale = 1;
	int linewidth = 2;
	for(unsigned int i = 0; i < data_list.size(); i++)
		str += data_list.at(i).data;
	char* ss = (char*)str.c_str();
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
	cvPutText(original, ss, cvPoint(5, (DWORD)(height*0.9)), &font, cvScalar(0,0,255));
	str = "";
	data_list.clear();
}
//------------------------------------------------------------------------------------------------//
void Upload::Eq_SDFEFunc()
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	string::iterator iter;
	int find=0;

	if(data_list.size() == 0)
		return;

	int num = 0;

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == "-" )
			num++;
	}
	BOOL SubOrFoun_Check = false;
	for(int i = 0; i < num; i++)
	{
		int frontnum=-1;
		int backnum=-1;

		int num=0;
		int SubOrFoun_idx=0;

		string denominator_low = "";
		string numerator_high = "";

		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(SubOrFoun_Check)
				SubOrFoun_Check = false;

			// 분수
			if(data_list.at(i).data == "-" )
			{
				for(unsigned int SDFEIdx = 0; SDFEIdx < data_list.size(); SDFEIdx++)
				{
					if(data_list.at(SDFEIdx).data == "+" || data_list.at(SDFEIdx).data == "-" 
						|| data_list.at(SDFEIdx).data == "*" || data_list.at(SDFEIdx).data == "/"|| data_list.at(SDFEIdx).data == ".")
						continue;

					if(data_list.at(SDFEIdx).right_bottom.y > data_list.at(i).right_bottom.y)
					{
						if( data_list.at(i).left_top.x <     (data_list.at(SDFEIdx).left_top.x + data_list.at(SDFEIdx).right_bottom.x)/2 && 
							data_list.at(i).right_bottom.x > (data_list.at(SDFEIdx).left_top.x + data_list.at(SDFEIdx).right_bottom.x)/2)
						{
							SubOrFoun_Check = true;
							break;
						}
					}
				}

				if(!SubOrFoun_Check)
					continue;

				SubOrFoun_idx = i;
				for(unsigned int B = i; B >= 0; B--)
				{ 
					if(data_list.at(B).data == "+" || data_list.at(B).data == "-" 
						|| data_list.at(B).data == "*" || data_list.at(B).data == "/" || data_list.at(B).data == "(" || B == 0)
					{
						if(data_list.at(B).data == "(")
						{
							frontnum = B+1;
							break;
						}
						if(B==0)
						{
							frontnum = B;
							break;
						}
						if(B==data_list.size()-1)
						{
							if(*data_list.at(data_list.size()-1).data.c_str() == '-')
							{
								for(unsigned int C = B-1; C >= 0; C--)
								{ 
									if(data_list.at(C).data == "+" || data_list.at(C).data == "-" 
										|| data_list.at(C).data == "*" || data_list.at(C).data == "/" || data_list.at(C).data == "(" || C == 0)
									{
										frontnum = C+1;
										break;
									}
								}
							}
							else
								continue;
						}
						else if(data_list.at(i).left_top.x == data_list.at(B).left_top.x)
							continue;
						else
							frontnum = B+1;
						break;
					}
				}
				for(unsigned int j = frontnum; j < data_list.size(); j++)
				{
					if(data_list.at(j).data == "+" || data_list.at(j).data == "-" 
						|| data_list.at(j).data == "*" || data_list.at(j).data == "/" || data_list.at(j).data == "=" ||  data_list.at(j).data == ")" ||j == data_list.size()-1)
					{
						if(data_list.at(j).data == ")")
						{
							backnum = j;
							break;
						}

						if(j == data_list.size()-1)
						{
							j++;                  
							backnum = j;
							break;
						}
						if(j == SubOrFoun_idx)
							continue;
						backnum = j;
						break;
					}
				}
				break;
			}
		}

		if(frontnum == -1 || backnum == -1)
		{
			Final_Modification_Print.clear();
			for(unsigned int i = 0; i < data_list.size(); i++)//괄호 갯수로 return 여부 결정
				Final_Modification_Print.push_back(data_list.at(i));
			return;
		}

		for(int i = frontnum; i < backnum; i++)
		{
			if(data_list.at(SubOrFoun_idx).left_top.y > data_list.at(i).left_top.y 
				|| data_list.at(SubOrFoun_idx).left_top.y == data_list.at(i).left_top.y)// - 빼기
			{
				if(i == backnum-1)
				{
					Final_Modification_Print.clear();
					for(unsigned int i = 0; i < data_list.size(); i++)//괄호 갯수로 return 여부 결정
						Final_Modification_Print.push_back(data_list.at(i));
					return;
				}
				else
					continue;
			}
			else
			{
				SubOrFoun_Check = true;
				break;
			}
			return;
		}
		if(SubOrFoun_Check)
		{
			for(int i = frontnum; i < backnum; i++)
			{
				if(data_list.at(SubOrFoun_idx).left_top.y < data_list.at(i).left_top.y
					&&*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' 
					&&*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/'
					&&*data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != ')'
					&& *data_list.at(i).data.c_str() != '=')
				{
					if(i == SubOrFoun_idx)
						continue;
					denominator_low += data_list.at(i).data.c_str();
				}
				else if(*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' 
					&&*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/'
					&&*data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != ')'
					&& *data_list.at(i).data.c_str() != '=')
				{
					if(i == SubOrFoun_idx)
						continue;
					numerator_high += data_list.at(i).data.c_str();
				}
			}
		}

		if(numerator_high=="" || denominator_low=="")
		{
			data_list.clear();
			numerator_high = "";
			denominator_low ="";
			return;
		}

		int omsint = backnum - frontnum;
		double Calculation_Double;
		if(omsint == 3)
		{
			if(strstr((char*)numerator_high.c_str(),"x") != NULL && strstr((char*)denominator_low.c_str(),"x") == NULL)
			{
				if(numerator_high == "x")
					numerator_high = "1";
				else if(numerator_high == "-x")
					numerator_high = "-1";
				else
				{
					for(iter = numerator_high.begin(); iter != numerator_high.end(); iter++, find++)
					{if(*iter == 'x'){numerator_high.erase(find,1);break;}}
					find = 0;
				}
				Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
				data_list.at(frontnum).data = to_string(Calculation_Double);
				data_list.at(frontnum).data += "x";
			}

			else if(strstr((char*)numerator_high.c_str(),"x") == NULL && strstr((char*)denominator_low.c_str(),"x") != NULL)
			{
				if(denominator_low == "x")
					denominator_low = "1";
				else if(numerator_high == "-x")
					numerator_high = "-1";
				else
				{
					for(iter = denominator_low.begin(); iter != denominator_low.end(); iter++, find++)
					{if(*iter == 'x'){denominator_low.erase(find,1);break;}}
					find = 0;
				}
				Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
				data_list.at(frontnum).data = to_string(Calculation_Double);
				data_list.at(frontnum).data += "x";
			}

			else if(strstr((char*)numerator_high.c_str(),"x") != NULL && strstr((char*)denominator_low.c_str(),"x") != NULL)
			{
				if(numerator_high == "x")
					numerator_high == "1";
				else if(numerator_high == "-x")
					numerator_high = "-1";
				else
				{
					for(iter = numerator_high.begin(); iter != numerator_high.end(); iter++, find++)
					{if(*iter == 'x'){numerator_high.erase(find,1);break;}}
					find = 0;
				}

				if(denominator_low == "x")
					denominator_low == "1";
				else if(numerator_high == "-x")
					numerator_high = "-1";
				else
				{
					for(iter = denominator_low.begin(); iter != denominator_low.end(); iter++, find++)
					{if(*iter == 'x'){denominator_low.erase(find,1);break;}}
					find = 0;
				}
				Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
				data_list.at(frontnum).data = to_string(Calculation_Double);
			}
			else
			{
				Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
				data_list.at(frontnum).data = to_string(Calculation_Double);
			}
			data_list.erase(data_list.begin()+frontnum+1,data_list.begin()+frontnum+3);
			Calculation_Double = 0;
		}
		else
		{
			omsint -=3;
			if(omsint<0)
				return;
			data_list.erase(data_list.begin()+frontnum,data_list.begin()+frontnum+omsint);
			if(strstr((char*)numerator_high.c_str(),"x") != NULL && strstr((char*)denominator_low.c_str(),"x") == NULL)
			{
				if(numerator_high == "x")
					numerator_high == "1";
				else if(numerator_high == "-x")
					numerator_high = "-1";
				else
				{
					for(iter = numerator_high.begin(); iter != numerator_high.end(); iter++, find++)
					{if(*iter == 'x'){numerator_high.erase(find,1);break;}}
					find = 0;
				}
				Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
				data_list.at(frontnum).data = to_string(Calculation_Double);
				data_list.at(frontnum).data += "x";
			}

			else if(strstr((char*)numerator_high.c_str(),"x") == NULL && strstr((char*)denominator_low.c_str(),"x") != NULL)
			{
				if(denominator_low == "x")
					denominator_low == "1";
				else if(numerator_high == "-x")
					numerator_high = "-1";
				else
				{
					for(iter = denominator_low.begin(); iter != denominator_low.end(); iter++, find++)
					{if(*iter == 'x'){denominator_low.erase(find,1);break;}}
					find = 0;
				}
				Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
				data_list.at(frontnum).data = to_string(Calculation_Double);
				data_list.at(frontnum).data += "x";
			}

			else if(strstr((char*)numerator_high.c_str(),"x") != NULL && strstr((char*)denominator_low.c_str(),"x") != NULL)
			{
				if(numerator_high == "x")
					numerator_high == "1";
				else if(numerator_high == "-x")
					numerator_high = "-1";
				else
				{
					for(iter = numerator_high.begin(); iter != numerator_high.end(); iter++, find++)
					{if(*iter == 'x'){numerator_high.erase(find,1);break;}}
					find = 0;
				}

				if(denominator_low == "x")
					denominator_low == "1";
				else if(numerator_high == "-x")
					numerator_high = "-1";
				else
				{
					for(iter = denominator_low.begin(); iter != denominator_low.end(); iter++, find++)
					{if(*iter == 'x'){denominator_low.erase(find,1);break;}}
					find = 0;
				}
				Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
				data_list.at(frontnum).data = to_string(Calculation_Double);
			}
			else
			{
				Calculation_Double = Calculation(atof(numerator_high.c_str()), '/', atof(denominator_low.c_str()));
				data_list.at(frontnum).data = to_string(Calculation_Double);
			}

			data_list.erase(data_list.begin()+frontnum+1,data_list.begin()+frontnum+3);
			Calculation_Double = 0;
		}
		frontnum=0;
		backnum=0;
		num=0;
		SubOrFoun_idx=0;
		denominator_low = "";
		numerator_high = "";
	}
	Final_Modification_Print.clear();
	for(unsigned int i = 0; i < data_list.size(); i++)//괄호 갯수로 return 여부 결정
		Final_Modification_Print.push_back(data_list.at(i));
}
void Upload::Eq_PointFunc()//소수점
{   
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	if(data_list.at(0).data == "." || data_list.at(data_list.size()-1).data == ".")
	{
		data_list.clear();
		return;
	}

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == "+" || data_list.at(i).data == "*" || data_list.at(i).data == "/")
		{
			if(i == data_list.size()-1)
			{
				data_list.clear();
				return;
			}
			if(data_list.at(i+1).data == ".")
			{
				data_list.clear();
				return;
			}
		}
	}

	int idx = 0;
	int FrontIdx = 0;
	int BackIdx = 0;
	string DoubleStr = "";

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(data_list.at(i).data == ".")
			idx++;
	}

	if(idx == 0)
		return;

	for (int i = 0; i < idx; i++)
	{
		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(data_list.at(i).data == ".")
			{
				for(unsigned int B = i; B >= 0; B--)
				{
					if(data_list.at(B).data == "+" || data_list.at(B).data == "-" 
						|| data_list.at(B).data == "*" || data_list.at(B).data == "/" 
						|| data_list.at(B).data == "=" || data_list.at(B).data == "(" || B == 0)
					{
						if(data_list.at(B).data == "-")
						{
							FrontIdx = B;
							break;
						}

						if(data_list.at(B).data == "(")
						{
							if(B==0)
								FrontIdx = 1;
							else
								FrontIdx = B+1;
							break;
						}

						if(B == 0)
							B=-1;
						FrontIdx = B+1;
						break;
					}
				}

				for(unsigned int j = FrontIdx; j < data_list.size(); j++)
				{
					if( data_list.at(j).data == "+" || data_list.at(j).data == "-" || 
						data_list.at(j).data == "*" || data_list.at(j).data == "/" ||
						j == data_list.size()-1 || data_list.at(j).data == ")")
					{
						if(j+1 == data_list.size())
							BackIdx = data_list.size();
						else if(data_list.at(j).data == ")")
							BackIdx = j;
						else
							BackIdx = j+1;

						for(int k = FrontIdx; k < BackIdx; k++)
						{
							if(data_list.at(FrontIdx).data == ".")
							{
								data_list.clear();
								return;
							}

							DoubleStr += data_list.at(k).data;

							if(k < BackIdx-1)
							{
								if((data_list.at(k).right_bottom.y - data_list.at(k).left_top.y)+(data_list.at(k).left_top.y/2) > data_list.at(k+1).right_bottom.y ||
									data_list.at(k+1).data == "+" || data_list.at(k+1).data == "-" || data_list.at(k+1).data == "^" ||
									data_list.at(k+1).data == "*" || data_list.at(k+1).data == "/" || data_list.at(k+1).data == "=")
								{
									BackIdx = k+1;
									break;
								}
							}
						}
						break;
					}
				}
				idx--;
			}
			if(DoubleStr != "")
			{
				data_list.at(FrontIdx).data = DoubleStr;
				data_list.erase(data_list.begin()+FrontIdx+1, data_list.begin()+BackIdx);
				DoubleStr = "";
			}
			if(idx != 0)
				continue;
		}

		idx = 0;
		FrontIdx = 0;
		BackIdx = 0;
		DoubleStr = "";
	}
	return;
}
void Upload::Eq_SquareFunc()//제곱
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	if(data_list.size() == 0)
		return;

	int Square_Idx = 0;
	bool Square_Check = false;

	if(data_list.size() == 1)
		return;


	for(unsigned int i = 0; i <data_list.size(); i++)//제곱근을 찾아 '^'문자를 삽입
	{
		if(atof(data_list.at(i).data.c_str()) < -1 ||
			*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' 
			&&*data_list.at(i).data.c_str() != '*' && *data_list.at(i).data.c_str() != '/'
			&&*data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != '.')//idx i의 값이 정수형이여야한.
		{
			if(data_list.size() >= 2 &&  i+1 < data_list.size())
			{
				if(*data_list.at(i+1).data.c_str() != '+' && *data_list.at(i+1).data.c_str() != '-' 
					&&*data_list.at(i+1).data.c_str() != '*' && *data_list.at(i+1).data.c_str() != '/'
					&&*data_list.at(i+1).data.c_str() != '(' && *data_list.at(i).data.c_str() != '.' &&
					*data_list.at(i+1).data.c_str() != '=')//문자 다음으로 숫자만 와야 함
				{
					if(i+1 == data_list.size())
						continue;

					if(data_list.at(i).size / 2 > data_list.at(i+1).size && 
						*data_list.at(i+1).data.c_str() != '.' && 
						*data_list.at(i+1).data.c_str() != '=.' && 						
						atof(data_list.at(i+1).data.c_str()) == 2 && 
						atof(data_list.at(i+1).data.c_str()) >= 0 && 
						atof(data_list.at(i+1).data.c_str()) < 10 && 
						*data_list.at(i+1).data.c_str() != ')')
					{
						Square_Idx++;
						Label_Data temp;
						temp.data = "^";
						if(*data_list.at(i+1).data.c_str() == ')')
						{
							data_list.insert(data_list.begin() + i + 2, temp);
							Square_Check = true;
						}
						else
						{
							data_list.insert(data_list.begin() + i + 1, temp);
							Square_Check = true;
						}
					}
					else
						continue;
				}
			}
		}
	}


	for(int i = 0; i < Square_Idx; i++)
	{

		int BackIdx=-1;
		int FrontIdx=-1;
		int idx=-1;   
		string Big_Str = "";
		string Small_Str = "";   
		double Big_Double = 0;
		double Small_Double = 0;

		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(data_list.at(i).data == "^")
			{
				idx = i;

				if(i-1 == 0)
				{
					FrontIdx = 0;
					if(data_list.at(i-1).data == "-")
						Big_Str += data_list.at(i-1).data;
					break;
				}
				for(unsigned int B = i-1; B >= 0; B--)
				{
					if(B == 0 && FrontIdx == -1)
					{
						if(data_list.at(B).data == "(" && FrontIdx == -1)
						{
							FrontIdx = B+1;
							break;
						}
						else
						{
							FrontIdx = 0;
							break;
						}
					}
					if(data_list.at(B).data == "=" && FrontIdx == -1)
					{
						FrontIdx = B+1;
						break;
					}

					if(data_list.at(B).data == "(" && FrontIdx == -1)
					{
						FrontIdx = B+1;
						break;
					}


					if(data_list.at(B).data == "-" && FrontIdx == -1)
					{
						if(B != 0)
						{
							if(data_list.at(B-1).data == "+" || data_list.at(B-1).data == "*" 
								|| data_list.at(B-1).data == "/" || data_list.at(B-1).data == "(" && FrontIdx == -1)
							{
								FrontIdx = B;
								break;      
							}
							else
							{
								FrontIdx = B+1;
								break;      
							}
						}
						else
						{
							FrontIdx = B;
							break;
						}
					}

					if(data_list.at(B).data == "+" || data_list.at(B).data == "*" || data_list.at(B).data == "/" && FrontIdx == -1)
					{
						if(B==0)
						{
							FrontIdx = 0;
							break;
						}
						else
						{
							FrontIdx = B+1;
							break;
						}
					}
				}
			}
			if(FrontIdx != -1)
				break;
		}



		for(unsigned int j = FrontIdx+1; j < data_list.size(); j++)
		{
			if(data_list.at(j).data == "^" && BackIdx == -1)
				continue;

			if(data_list.at(j).data == ")" && BackIdx == -1)
			{
				BackIdx = j;            
				break;
			}

			if(j == data_list.size()-1 && BackIdx == -1)
			{
				BackIdx = data_list.size();            
				break;
			}

			if(data_list.at(j).data == "+" || data_list.at(j).data == "-" 
				|| data_list.at(j).data == "*" || data_list.at(j).data == "/"  
				|| data_list.at(j).data == "^" || data_list.at(j).data == "x" || data_list.at(j).data == "T" 
				&& BackIdx == -1)
			{
				BackIdx = j;               
				break;
			}
		}


		for(int i = FrontIdx; i < idx; i++)
			Big_Str += data_list.at(i).data.c_str();

		for(int i = idx+1; i < BackIdx; i++)
			Small_Str += data_list.at(i).data.c_str();

		if(strstr((char*)Small_Str.c_str(),"+") != NULL || 
			strstr((char*)Small_Str.c_str(),"-") != NULL ||
			strstr((char*)Small_Str.c_str(),"*") != NULL ||
			strstr((char*)Small_Str.c_str(),"/") != NULL ||
			strstr((char*)Small_Str.c_str(),"(") != NULL ||
			strstr((char*)Small_Str.c_str(),")") != NULL ||
			strstr((char*)Small_Str.c_str(),"^") != NULL ||
			strstr((char*)Small_Str.c_str(),".") != NULL)
		{
			idx=-1;
			Big_Str = "";
			Small_Str = "";
			Big_Double = 0;
			Small_Double = 0;
			BackIdx=0;
			FrontIdx=0;
			data_list.clear();
			return;
		}


		Big_Double = atof(Big_Str.c_str());
		Small_Double = atof(Small_Str.c_str());

		double result = Eq_Calculation(Big_Double, '*', Big_Double);

		for(int i = 0; i < Small_Double-2; i++)
			result = Eq_Calculation(result, '*', Big_Double);

		int result_int = (int)result;

		if(result==result_int)
		{
			data_list.at(FrontIdx).data = to_string(result_int);
			data_list.erase(data_list.begin()+FrontIdx+1,data_list.begin()+BackIdx);
		}
		else
		{
			data_list.at(FrontIdx).data = to_string(result);
			data_list.erase(data_list.begin()+FrontIdx+1,data_list.begin()+BackIdx);
		}



		idx=-1;
		Big_Double = 0;
		Small_Double = 0;
		result = 0;
		BackIdx=0;
		FrontIdx=0;
		string::iterator iter;
		int find=0;

		if(atof(Big_Str.c_str()) != 0)
		{
			for(unsigned int i = 0; i < Final_Modification_Print.size(); i++)
			{

				if(atof(Big_Str.c_str()) < 0)
				{
					for(iter = Big_Str.begin(); iter != Big_Str.end(); iter++, find++)
					{
						if(*iter == '-')
						{
							Big_Str.erase(find,1);
							break;
						}
					}
					find = 0;

					for(unsigned int i = 0; i < Final_Modification_Print.size(); i++)				
					{
						if(Final_Modification_Print.at(i).data == Big_Str)
						{
							Label_Data L;
							L.data			= "^";
							Final_Modification_Print.insert(Final_Modification_Print.begin() + i+1, L);
							break;
						}
					}
					break;
				}
				else
				{
					for(unsigned int i = 0; i < Final_Modification_Print.size(); i++)				
					{
						if(Final_Modification_Print.at(i).data == Big_Str)
						{
							Label_Data L;
							L.data			= "^";
							Final_Modification_Print.insert(Final_Modification_Print.begin() + i+1, L);
							break;
						}
					}
					break;
				}
			}
		}
		Big_Str = "";
		Small_Str = "";
	}
	return;
}
void Upload::Eq_Final_Set()//최종 정리
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	if(data_list.size() == 1 && 
		(*data_list.at(0).data.c_str() != '+' || *data_list.at(0).data.c_str() != '-' 
		|| *data_list.at(0).data.c_str() != '*' || *data_list.at(0).data.c_str() != '/'
		|| *data_list.at(0).data.c_str() != '(' || *data_list.at(0).data.c_str() != ')'))
		return;

	string LTH = "";
	int FrontNum = -1;
	int BackNum = -1;

	if(data_list.size() == 0)
		return;


	if(data_list.size() == 3 && 
		(*data_list.at(0).data.c_str() != '+' || *data_list.at(0).data.c_str() != '-' || *data_list.at(0).data.c_str() != '*' || 
		*data_list.at(0).data.c_str() != '/' || *data_list.at(0).data.c_str() != '(') && 

		(*data_list.at(1).data.c_str() == '+' || *data_list.at(1).data.c_str() == '-' || *data_list.at(1).data.c_str() == '*' || 
		*data_list.at(1).data.c_str() == '/') &&

		(*data_list.at(2).data.c_str() != '+' || *data_list.at(2).data.c_str() != '-' || *data_list.at(2).data.c_str() != '*' || 
		*data_list.at(2).data.c_str() != '/' || *data_list.at(2).data.c_str() != '(' || *data_list.at(2).data.c_str() != ')'))
		return;

	while(1)//십의 자리 이상 숫자 들을 묶는 while문
	{
		for(unsigned int i = 0; i < data_list.size()-1; i++)//십의 자리 이상 숫자 들을 묶는 for문
		{
			if((*data_list.at(i).data.c_str() != '+' && *data_list.at(i).data.c_str() != '-' && *data_list.at(i).data.c_str() != '*' && 
				*data_list.at(i).data.c_str() != '/' && *data_list.at(i).data.c_str() != '(' && *data_list.at(i).data.c_str() != ')' &&
				*data_list.at(i).data.c_str() != '=') &&
				atof(data_list.at(i).data.c_str()) >= 0 && atof(data_list.at(i).data.c_str()) < 10)
			{
				if((*data_list.at(i+1).data.c_str() != '+' && *data_list.at(i+1).data.c_str() != '-' && *data_list.at(i+1).data.c_str() != '*' && 
					*data_list.at(i+1).data.c_str() != '/' && *data_list.at(i+1).data.c_str() != '(' && *data_list.at(i+1).data.c_str() != ')' &&
					*data_list.at(i+1).data.c_str() != '=') &&
					atof(data_list.at(i+1).data.c_str()) >= 0 && atof(data_list.at(i+1).data.c_str()) < 10)
				{
					if(FrontNum == -1)
						FrontNum = i;
					break;
				}         
			}
		}
		for(unsigned int i = FrontNum; i < data_list.size(); i++)//십의 자리 이상 숫자 들을 묶는 for문
		{
			if(*data_list.at(i).data.c_str() == '+' || *data_list.at(i).data.c_str() == '-' || *data_list.at(i).data.c_str() == '*' || 
				*data_list.at(i).data.c_str() == '/' || *data_list.at(i).data.c_str() == '(' || *data_list.at(i).data.c_str() == ')' || 
				*data_list.at(i).data.c_str() == '=' || i == data_list.size()-1)
			{
				if(*data_list.at(i).data.c_str() == ')')
				{
					BackNum = i;
					break;
				}
				if(i == data_list.size()-1)
					i++;
				BackNum = i;
				break;
			}
		}
		for (int i = FrontNum; i < BackNum; i++)
			LTH+=data_list.at(i).data;

		if((strstr((char*)LTH.c_str(),".x") != NULL && strstr((char*)LTH.c_str(),"x.") != NULL))
		{
			data_list.clear();
			return;
		}

		if(FrontNum != -1 && BackNum != -1)//십의 자리 이상 숫자 들을 묶는 for문
		{
			data_list.at(FrontNum).data = LTH;
			data_list.erase(data_list.begin()+FrontNum+1, data_list.begin() +BackNum);
			LTH = "";
		}
		if(FrontNum == -1 || BackNum == -1)
		{         
			FrontNum = -1;
			BackNum = -1;
			break;
		}
		FrontNum = -1;
		BackNum = -1;
	}

	string Operator_And_Operand = "";
	int OperandIdx=-1;

	for(unsigned int i = 0; i < data_list.size(); i++)//음의 정수를 찾는 for문
	{
		if(*data_list.at(i).data.c_str() == '-')
		{
			if(i > 0)
				if( data_list.at(i-1).data != "+" && data_list.at(i-1).data != "-" && 
					data_list.at(i-1).data != "*" && data_list.at(i-1).data != "/" && 
					data_list.at(i-1).data != "=" && 						   
					data_list.at(i-1).data != "(" && data_list.at(i-1).data != ")" && 
					data_list.at(i+1).data != "+" && data_list.at(i+1).data != "-" && 
					data_list.at(i+1).data != "*" && data_list.at(i+1).data != "/" && 
					data_list.at(i+1).data != "(" && data_list.at(i+1).data != ")")
					continue;

			if(i == 1)
			{
				OperandIdx = i;
				do
				{
					Operator_And_Operand += data_list.at(i).data;
					i++;
					if(i==data_list.size())break;
				}while(data_list.at(i).data != "+" && data_list.at(i).data != "-" && 
					data_list.at(i).data != "*" && data_list.at(i).data != "/" && 
					data_list.at(i).data != "(" && data_list.at(i).data != ")" &&
					data_list.at(i).data != "=");
				data_list.at(OperandIdx).data = Operator_And_Operand;
				Operator_And_Operand = "";
				data_list.erase(data_list.begin()+OperandIdx+1,data_list.begin()+i);
				i = OperandIdx;
				continue;
			}

			if(i != 0)
			{
				if( data_list.at(i-1).data == "+" || data_list.at(i-1).data == "-" || 
					data_list.at(i-1).data == "*" || data_list.at(i-1).data == "/" || 
					data_list.at(i-1).data == "(" || data_list.at(i-1).data == "=")
				{
					OperandIdx = i;
					do
					{
						Operator_And_Operand += data_list.at(i).data;
						i++;
						if(i==data_list.size())break;
					}while(data_list.at(i).data != "+" && data_list.at(i).data != "-" && 
						data_list.at(i).data != "*" && data_list.at(i).data != "/" && 
						data_list.at(i).data != "(" && data_list.at(i).data != ")" && data_list.at(i).data != "=");
					data_list.at(OperandIdx).data = Operator_And_Operand;
					Operator_And_Operand = "";
					data_list.erase(data_list.begin()+OperandIdx+1,data_list.begin()+i);
					i = OperandIdx;
					continue;
				}
				else
					continue;


			}
			else
			{
				OperandIdx = i;
				do
				{
					Operator_And_Operand += data_list.at(i).data;
					i++;
					if(i==data_list.size())break;
				}while(data_list.at(i).data != "+" && data_list.at(i).data != "-" && 
					data_list.at(i).data != "*" && data_list.at(i).data != "/" && data_list.at(i).data != ")"
					&& data_list.at(i).data != "=");
				data_list.at(OperandIdx).data = Operator_And_Operand;
				Operator_And_Operand = "";
				data_list.erase(data_list.begin()+OperandIdx+1,data_list.begin()+i);
				i = OperandIdx;
				continue;
			}
		}
	}

}
void Upload::Eq_BracketFunc()//1. 괄호
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	bool Open_Bracket_Check = false;
	bool Close_Bracket_Check = false;
	int idx = 0;
	int FrontIdx = -1;
	int BackIdx = -1;
	string Return_Operation_Value = "";

	if(data_list.size() == 0)
		return;

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(i < data_list.size()-1)
			if(*data_list.at(i).data.c_str() == '(' )
			{
				if(*data_list.at(i+1).data.c_str() == ')' )
				{
					data_list.clear();
					return;
				}
			}
	}

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == '(' )
			Open_Bracket_Check = true;
		if(*data_list.at(i).data.c_str() == ')')
			Close_Bracket_Check = true;
		if(Open_Bracket_Check == false && Close_Bracket_Check == true)
		{
			data_list.clear();
			return;
		}
	}

	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == '(' || *data_list.at(i).data.c_str() == ')')
			idx++;
	}

	if(idx == 0)
		return;

	idx/=2;

	if(idx != 0)
	{
		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '(')
			{
				if(*data_list.at(i+2).data.c_str() == ')')
				{
					data_list.erase(data_list.begin()+i);
					data_list.erase(data_list.begin()+i+1);
					idx--;
				}
			}
		}
	}

	for (int o = 0; o < idx; o++)
	{
		for(unsigned int i = 0; i < data_list.size(); i++)
		{
			if(*data_list.at(i).data.c_str() == '(')
				FrontIdx = i;

			if(*data_list.at(i).data.c_str() == ')' && BackIdx == -1)
				BackIdx = i;

			if(FrontIdx != -1 && BackIdx != -1)
				break;
		}

		if(FrontIdx == -1 || BackIdx == -1)
			return;

		if(BackIdx - FrontIdx == 2)
		{
			data_list.erase(data_list.begin()+FrontIdx);
			data_list.erase(data_list.begin()+FrontIdx+1);
		}
		else
		{
			Eq_Middle_Final_Operation(FrontIdx+1, BackIdx);
			FrontIdx = -1;
			BackIdx = -1;
		}
	}
}
double Upload::Eq_Calculation(double a, char op, double b)
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return -1;
	}

	switch(op)
	{
	case '+': return a+b; break;
	case '-': return a-b; break;
	case '*': return a*b; break;
	case '/': return a/b; break;
	default:
		{
			data_list.clear();
			return 0;
		}
	}

	return NULL;
}
void Upload::Eq_Middle_Final_Operation(int FristIdx, int SecondIdx)
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	string::iterator iter;
	int find=0;
	bool Right = false;
	bool Left = false;


	for(int i = FristIdx; i < SecondIdx; i++)
		M_F_O_V.push_back(data_list.at(i).data);
	if(M_F_O_V.size() == 1)
	{
		M_F_O_V.clear();
		return;
	}

	for(unsigned int i = 1; i < M_F_O_V.size(); i+=2)
	{
		if(*M_F_O_V.at(i).c_str() == '*')
		{
			Eq_Bracket_Middle_Operation(i, '*');
			i = -1;
			continue;
		}

		if(*M_F_O_V.at(i).c_str() == '/')
		{
			Eq_Bracket_Middle_Operation(i, '/');
			i = -1;
			continue;
		}
	}
	for(unsigned int i = 1; i < M_F_O_V.size(); i+=2)
	{
		if(*M_F_O_V.at(i).c_str() == '+')
		{
			if((strstr((char*)M_F_O_V.at(i).c_str(),"x") != NULL && strstr((char*)M_F_O_V.at(i).c_str(),"x") != NULL) ||
				(strstr((char*)M_F_O_V.at(i).c_str(),"x") == NULL && strstr((char*)M_F_O_V.at(i).c_str(),"x") == NULL) ||
				(strstr((char*)M_F_O_V.at(i).c_str(),"T") != NULL && strstr((char*)M_F_O_V.at(i).c_str(),"T") != NULL) ||
				(strstr((char*)M_F_O_V.at(i).c_str(),"T") == NULL && strstr((char*)M_F_O_V.at(i).c_str(),"T") == NULL)) 
			{
				bool check = Eq_Bracket_Middle_Operation(i, '+');
				if(check)
				{
					i = -1;
					continue;
				}
				else
					continue;
			}
		}
		if(*M_F_O_V.at(i).c_str() == '-')
		{
			if((strstr((char*)M_F_O_V.at(i).c_str(),"x") != NULL && strstr((char*)M_F_O_V.at(i).c_str(),"x") != NULL) ||
				(strstr((char*)M_F_O_V.at(i).c_str(),"x") == NULL && strstr((char*)M_F_O_V.at(i).c_str(),"x") == NULL) ||
				(strstr((char*)M_F_O_V.at(i).c_str(),"T") != NULL && strstr((char*)M_F_O_V.at(i).c_str(),"T") != NULL) ||
				(strstr((char*)M_F_O_V.at(i).c_str(),"T") == NULL && strstr((char*)M_F_O_V.at(i).c_str(),"T") == NULL)) 
			{
				bool check = Eq_Bracket_Middle_Operation(i, '-');
				if(check)
				{
					i = -1;
					continue;
				}
				else
					continue;
			}
		}
	}
	if(M_F_O_V.size() > 3)
	{
		Eq_lineup_String(M_F_O_V);      
		M_F_O_V.clear();
		for(unsigned int i = 0; i < lineup_OM.size(); i++)
			M_F_O_V.push_back(lineup_OM.at(i));
		lineup_OM.clear();
	}
	int idx = 0;
	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == '(' || *data_list.at(i).data.c_str() == ')')
			idx++;
	}
	Label_Data temp;
	if(idx == 0)
	{
		data_list.erase(data_list.begin()+FristIdx,data_list.begin()+SecondIdx);
		if(M_F_O_V.size() == 0)
		{
			temp.data = "0";
			data_list.insert(data_list.begin() + FristIdx, temp);
			return;
		}
		if(M_F_O_V.size() == 1)
		{
			temp.data = M_F_O_V.at(0);      
			data_list.insert(data_list.begin() + FristIdx, temp);
		}
		else
		{
			for(unsigned int b = M_F_O_V.size()-1; b >= 0; b--)
			{
				temp.data = M_F_O_V.at(b);      
				data_list.insert(data_list.begin() + FristIdx, temp);
				if(b == 0)
					break;
			}
		}
		M_F_O_V.clear();
		return;
	}
	if(FristIdx !=0)
	{
		if(*data_list.at(FristIdx-1).data.c_str() == '(' && *data_list.at(SecondIdx).data.c_str() == ')')//괄호 양 옆으로 피연산자가 있는 경우
		{
			if(FristIdx-1 != 0)
			{
				if(*data_list.at(FristIdx-2).data.c_str() != '+' && *data_list.at(FristIdx-2).data.c_str() != '-' && 
					*data_list.at(FristIdx-2).data.c_str() != '*' && *data_list.at(FristIdx-2).data.c_str() != '/' &&
					*data_list.at(FristIdx-2).data.c_str() != '(' && *data_list.at(FristIdx-2).data.c_str() != '.' && 
					*data_list.at(FristIdx-2).data.c_str() != '=')
				{
					if((strstr((char*)data_list.at(FristIdx-2).data.c_str(),"T") != NULL))//괄호 왼쪽이 2차일때
					{
						if(data_list.at(FristIdx-2).data == "T")
							data_list.at(FristIdx-2).data = "1";
						else if(data_list.at(FristIdx-2).data == "-T")
							data_list.at(FristIdx-2).data = "-1";
						else
						{
							for(iter = data_list.at(FristIdx-2).data.begin(); iter != data_list.at(FristIdx-2).data.end(); iter++, find++)
							{if(*iter == 'T'){data_list.at(FristIdx-2).data.erase(find,1);break;}}
							find = 0;
						}

						for(unsigned int i = 0; i < M_F_O_V.size(); i+=2)
						{
							if((strstr((char*)M_F_O_V.at(i).c_str(),"T") == NULL || (strstr((char*)M_F_O_V.at(i).c_str(),"x") == NULL)))//정수형일때만 계산
							{
								M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(FristIdx-2).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
								M_F_O_V.at(i) += "T";
								Left = true;
							}
							else
							{
								data_list.clear();
								M_F_O_V.clear();
								return;
							}
						}
					}
					else if((strstr((char*)data_list.at(FristIdx-2).data.c_str(),"x") != NULL))//괄호 왼쪽이 1차일때
					{
						if(data_list.at(FristIdx-2).data == "x")
							data_list.at(FristIdx-2).data = "1";
						else if(data_list.at(FristIdx-2).data == "-x")
							data_list.at(FristIdx-2).data = "-1";
						else
						{
							for(iter = data_list.at(FristIdx-2).data.begin(); iter != data_list.at(FristIdx-2).data.end(); iter++, find++)
							{if(*iter == 'x'){data_list.at(FristIdx-2).data.erase(find,1);break;}}
							find = 0;
						}
						for(unsigned int i = 0; i < M_F_O_V.size(); i+=2)
						{
							if((strstr((char*)M_F_O_V.at(i).c_str(),"T")))
							{
								data_list.clear();
								M_F_O_V.clear();
								return;
							}
							else if((strstr((char*)M_F_O_V.at(i).c_str(),"x")))
							{
								for(iter = M_F_O_V.at(i).begin(); iter != M_F_O_V.at(i).end(); iter++, find++)
								{
									if(*iter == 'x'){M_F_O_V.at(i).erase(find,1);break;}
								}
								find = 0;
								M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(FristIdx-2).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
								M_F_O_V.at(i) += "T";
								Left = true;
							}
							else if((strstr((char*)M_F_O_V.at(i).c_str(),"T") == NULL || (strstr((char*)M_F_O_V.at(i).c_str(),"x") == NULL)))//정수형일때만 계산
							{
								M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(FristIdx-2).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
								M_F_O_V.at(i) += "x";
								Left = true;
							}
						}
					}
					//괄호 왼쪽이 정수형일때
					else if((strstr((char*)data_list.at(FristIdx-2).data.c_str(),"x") == NULL) && (strstr((char*)data_list.at(FristIdx-2).data.c_str(),"T") == NULL))
					{               
						for(unsigned int i = 0; i < M_F_O_V.size(); i+=2)
						{
							if((strstr((char*)M_F_O_V.at(i).c_str(),"T")))
							{
								for(iter = M_F_O_V.at(i).begin(); iter != M_F_O_V.at(i).end(); iter++, find++)
								{if(*iter == 'T'){M_F_O_V.at(i).erase(find,1);break;}}
								find = 0;

								M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(FristIdx-2).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
								M_F_O_V.at(i) += "T";
							}
							else if((strstr((char*)M_F_O_V.at(i).c_str(),"x")))
							{
								for(iter = M_F_O_V.at(i).begin(); iter != M_F_O_V.at(i).end(); iter++, find++)
								{if(*iter == 'x'){M_F_O_V.at(i).erase(find,1);break;}}
								find = 0;

								M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(FristIdx-2).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
								M_F_O_V.at(i) += "x";
								Left = true;
							}
							else
								M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(FristIdx-2).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
							Left = true;
						}
					}
					SecondIdx--;
					FristIdx--;
				}
			}         
		}



		if(SecondIdx != data_list.size()-1)
		{
			if(*data_list.at(SecondIdx+1).data.c_str() != '+' && *data_list.at(SecondIdx+1).data.c_str() != '-' && 
				*data_list.at(SecondIdx+1).data.c_str() != '*' && *data_list.at(SecondIdx+1).data.c_str() != '/' &&
				*data_list.at(SecondIdx+1).data.c_str() != ')' && *data_list.at(SecondIdx+1).data.c_str() != '.' && 
				*data_list.at(SecondIdx+1).data.c_str() != '=')
			{
				if((strstr((char*)data_list.at(SecondIdx+1).data.c_str(),"T") != NULL))//괄호 왼쪽이 2차일때
				{
					if(data_list.at(SecondIdx+1).data == "T")
						data_list.at(SecondIdx+1).data = "1";
					else if(data_list.at(SecondIdx+1).data == "-T")
						data_list.at(SecondIdx+1).data = "-1";
					else
					{
						for(iter = data_list.at(SecondIdx+1).data.begin(); iter != data_list.at(SecondIdx+1).data.end(); iter++, find++)
						{if(*iter == 'T'){data_list.at(SecondIdx+1).data.erase(find,1);break;}}
						find = 0;
					}//<==T를 지우고

					for(unsigned int i = 0; i < M_F_O_V.size(); i+=2)
					{
						if((strstr((char*)M_F_O_V.at(i).c_str(),"T") == NULL || (strstr((char*)M_F_O_V.at(i).c_str(),"x") == NULL)))//정수형일때만 계산
						{
							M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(SecondIdx+1).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
							M_F_O_V.at(i) += "T";
							Right = true;
						}
						else
						{
							data_list.clear();
							M_F_O_V.clear();
							return;
						}
					}
				}

				else if((strstr((char*)data_list.at(SecondIdx+1).data.c_str(),"x") != NULL))//괄호 왼쪽이 1차일때
				{
					if(data_list.at(SecondIdx+1).data == "x")
						data_list.at(SecondIdx+1).data = "1";
					else if(data_list.at(SecondIdx+1).data == "-x")
						data_list.at(SecondIdx+1).data = "-1";
					else
					{
						for(iter = data_list.at(SecondIdx+1).data.begin(); iter != data_list.at(SecondIdx+1).data.end(); iter++, find++)
						{if(*iter == 'x'){data_list.at(SecondIdx+1).data.erase(find,1);break;}}
						find = 0;
					}//<==x지우고

					for(unsigned int i = 0; i < M_F_O_V.size(); i+=2)
					{
						if((strstr((char*)M_F_O_V.at(i).c_str(),"T")))
						{
							data_list.clear();
							M_F_O_V.clear();
							return;
						}
						else if((strstr((char*)M_F_O_V.at(i).c_str(),"x")))
						{
							if(M_F_O_V.at(i) == "x")
								M_F_O_V.at(i) = "1";
							else
							{
								for(iter = M_F_O_V.at(i).begin(); iter != M_F_O_V.at(i).end(); iter++, find++)
								{
									{if(*iter == 'x'){M_F_O_V.at(i).erase(find,1);break;}}
									find = 0;
								}
							}
							M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(SecondIdx+1).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
							M_F_O_V.at(i) += "T";
							Right = true;
						}
						else if((strstr((char*)M_F_O_V.at(i).c_str(),"T") == NULL || (strstr((char*)M_F_O_V.at(i).c_str(),"x") == NULL)))//정수형일때만 계산
						{
							M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(SecondIdx+1).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
							M_F_O_V.at(i) += "x";
							Right = true;
						}
					}
				}
				//괄호 왼쪽이 정수형일때
				else if((strstr((char*)data_list.at(SecondIdx+1).data.c_str(),"x") == NULL) && (strstr((char*)data_list.at(SecondIdx+1).data.c_str(),"T") == NULL))
				{               
					for(unsigned int i = 0; i < M_F_O_V.size(); i+=2)
					{
						if((strstr((char*)M_F_O_V.at(i).c_str(),"T")))
						{
							for(iter = M_F_O_V.at(i).begin(); iter != M_F_O_V.at(i).end(); iter++, find++)
							{if(*iter == 'T'){M_F_O_V.at(i).erase(find,1);break;}}
							find = 0;

							M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(SecondIdx+1).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
							M_F_O_V.at(i) += "T";
							Right = true;
						}
						else if((strstr((char*)M_F_O_V.at(i).c_str(),"x")))
						{
							for(iter = M_F_O_V.at(i).begin(); iter != M_F_O_V.at(i).end(); iter++, find++)
							{if(*iter == 'x'){M_F_O_V.at(i).erase(find,1);break;}}
							find = 0;

							M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(SecondIdx+1).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
							M_F_O_V.at(i) += "x";
							Right = true;
						}
						else
							M_F_O_V.at(i) = to_string(Eq_Calculation( atof(data_list.at(SecondIdx+1).data.c_str()), '*', atof(M_F_O_V.at(i).c_str())));
						Right = true;
					}
				}
				SecondIdx--;
				FristIdx--;
			}         
		}
		if(Left)
		{
			data_list.erase(data_list.begin()+FristIdx-1);
			data_list.erase(data_list.begin()+FristIdx-1,data_list.begin()+SecondIdx+1);
		}
		if(Right)
		{
			data_list.erase(data_list.begin()+SecondIdx+2);
			data_list.erase(data_list.begin()+FristIdx,data_list.begin()+SecondIdx+2);
		}

		if(Left == false && Right == false)
		{
			data_list.erase(data_list.begin()+FristIdx-1,data_list.begin()+SecondIdx+1);
			Label_Data temp;
			if(M_F_O_V.size() == 1)
			{
				if(data_list.size() == FristIdx-1)
				{					
					temp.data = M_F_O_V.at(0);      
					data_list.push_back(temp);	
				}
				else
				{
					temp.data = M_F_O_V.at(0);      
					data_list.insert(data_list.begin() + FristIdx-1, temp);
				}
			}
			else
			{
				if(data_list.size() == FristIdx-1)
				{					
					for(unsigned int b = 0; b < M_F_O_V.size(); b++)
					{
						temp.data = M_F_O_V.at(b);      
						data_list.push_back(temp);
					}
				}
				else
				{
					for(unsigned int b = M_F_O_V.size()-1; b >= 0; b--)
					{
						temp.data = M_F_O_V.at(b);      
						data_list.insert(data_list.begin() + FristIdx-1, temp);
						if(b == 0)
							break;
					}
				}
			}
			M_F_O_V.clear();
			return;
		}


		Label_Data temp;
		if(data_list.size()+1 == FristIdx)//괄호가 마지막이였다면
		{
			for(unsigned int i = 0; i < M_F_O_V.size(); i++)
			{
				temp.data = M_F_O_V.at(i);
				data_list.push_back(temp);
			}
			M_F_O_V.clear();
		}
		else
		{
			M_F_O_V.insert(M_F_O_V.begin(), "");
			for(unsigned int i = M_F_O_V.size()-1; i >= 0; i--)
			{
				if(i == 0)
				{
					M_F_O_V.clear();
					return;
				}

				temp.data = M_F_O_V.at(i);
				if(FristIdx == 0)
					data_list.insert(data_list.begin(), temp);
				else if(*data_list.at(data_list.size()-1).data.c_str() == '=')
				{
					for(unsigned int i = 1; i < M_F_O_V.size(); i++)
					{
						temp.data = M_F_O_V.at(i);
						data_list.push_back(temp);
					}
					break;
				}
				else
					data_list.insert(data_list.begin() + FristIdx, temp);
			}
		}
	}   
	M_F_O_V.clear();
	return;
}
bool Upload::Eq_Bracket_Middle_Operation(int OPidx, char OP)
{   
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return false;
	}

	string::iterator iter;
	int find=0;
	string Middle_Operation = "";
	bool Linear_Equation_Check = false;
	bool Mul = false;
	bool Div = false;

	if((strstr((char*)M_F_O_V.at(OPidx-1).c_str(),"x") == NULL && strstr((char*)M_F_O_V.at(OPidx+1).c_str(),"x") == NULL)&&
		(strstr((char*)M_F_O_V.at(OPidx-1).c_str(),"T") == NULL && strstr((char*)M_F_O_V.at(OPidx+1).c_str(),"T") == NULL)) //둘다 정수형일때
	{
		if(OPidx-2 > 0)
		{
			if(*M_F_O_V.at(OPidx-2).c_str() == '-')
			{
				M_F_O_V.at(OPidx-2) = "+";
				M_F_O_V.at(OPidx-1) = "-" + M_F_O_V.at(OPidx-1);
			}
		}

		Middle_Operation = to_string(Eq_Calculation( atof(M_F_O_V.at(OPidx-1).c_str()),OP ,atof(M_F_O_V.at(OPidx+1).c_str())));

		if(0 == atof(Middle_Operation.c_str()))
		{
			if(M_F_O_V.size() == 3)
			{
				M_F_O_V.clear();
				return false;
			}
			if(*M_F_O_V.at(OPidx+2).c_str() == '-')
			{
				M_F_O_V.at(OPidx+3) = "-" + M_F_O_V.at(OPidx+3);
				return true;
			}
			else
			{
				M_F_O_V.erase(M_F_O_V.begin()+OPidx-1,M_F_O_V.begin()+OPidx+3);
				return true;
			}
		}
		else
		{
			M_F_O_V.insert(M_F_O_V.begin() + OPidx - 1, Middle_Operation);
			M_F_O_V.erase(M_F_O_V.begin()+OPidx,M_F_O_V.begin()+OPidx+3);
			Middle_Operation = "";   
			find=0;
			return true;
		}
	}


	else if(strstr((char*)M_F_O_V.at(OPidx-1).c_str(),"T") != NULL && strstr((char*)M_F_O_V.at(OPidx+1).c_str(),"T") != NULL) //둘다 T가 있을때
	{
		if(OP == '*')
			Mul = true;
		else if(OP == '/')
			Div = true;

		if(Mul)
		{
			M_F_O_V.clear();
			data_list.clear();
			return false;
		}

		if(M_F_O_V.at(OPidx-1) == "T")
			M_F_O_V.at(OPidx-1) = "1";
		else
		{
			for(iter = M_F_O_V.at(OPidx-1).begin(); iter != M_F_O_V.at(OPidx-1).end(); iter++, find++)
			{if(*iter == 'T'){M_F_O_V.at(OPidx-1).erase(find,1);break;}}
			find = 0;
		}

		if(M_F_O_V.at(OPidx+1) == "T")
			M_F_O_V.at(OPidx+1) = "1";
		else
		{
			for(iter = M_F_O_V.at(OPidx+1).begin(); iter != M_F_O_V.at(OPidx+1).end(); iter++, find++)
			{if(*iter == 'T'){M_F_O_V.at(OPidx+1).erase(find,1);break;}}
			find = 0;
		}


		if(OPidx-2 > 0)
		{
			if(*M_F_O_V.at(OPidx-2).c_str() == '-')
			{
				M_F_O_V.at(OPidx-2) = "+";
				M_F_O_V.at(OPidx-1) = "-" + M_F_O_V.at(OPidx-1);
			}
		}

		Middle_Operation = to_string(Eq_Calculation( atof(M_F_O_V.at(OPidx-1).c_str()),OP ,atof(M_F_O_V.at(OPidx+1).c_str())));

		if(0 == atof(Middle_Operation.c_str()))
		{
			if(M_F_O_V.size() == 3)
			{
				M_F_O_V.clear();
				return false;
			}
			if(*M_F_O_V.at(OPidx+2).c_str() == '-')
				M_F_O_V.at(OPidx+3) = "-" + M_F_O_V.at(OPidx+3);
			else
				M_F_O_V.erase(M_F_O_V.begin()+OPidx-1,M_F_O_V.begin()+OPidx+3);
		}
		else
		{
			Middle_Operation+="T";
			M_F_O_V.insert(M_F_O_V.begin() + OPidx - 1, Middle_Operation);
			M_F_O_V.erase(M_F_O_V.begin()+OPidx,M_F_O_V.begin()+OPidx+3);
			Middle_Operation = "";   
			find=0;   
			return true;
		}
	}

	else if(strstr((char*)M_F_O_V.at(OPidx-1).c_str(),"x") != NULL && strstr((char*)M_F_O_V.at(OPidx+1).c_str(),"x") != NULL) //둘다 x가 있을때
	{
		if(OP == '*')
			Mul = true;
		else if(OP == '/')
			Div = true;

		if(M_F_O_V.at(OPidx-1) == "x")
			M_F_O_V.at(OPidx-1) = "1";
		else if(M_F_O_V.at(OPidx-1) == "-x")
			M_F_O_V.at(OPidx-1) = "-1";
		else
		{
			for(iter = M_F_O_V.at(OPidx-1).begin(); iter != M_F_O_V.at(OPidx-1).end(); iter++, find++)
			{if(*iter == 'x'){M_F_O_V.at(OPidx-1).erase(find,1);break;}}
			find = 0;
		}

		if(M_F_O_V.at(OPidx+1) == "x")
			M_F_O_V.at(OPidx+1) = "1";
		else if(M_F_O_V.at(OPidx+1) == "-x")
		{
			M_F_O_V.at(OPidx+1) = "-1";
		}
		else
		{
			for(iter = M_F_O_V.at(OPidx+1).begin(); iter != M_F_O_V.at(OPidx+1).end(); iter++, find++)
			{if(*iter == 'x'){M_F_O_V.at(OPidx+1).erase(find,1);break;}}
			find = 0;
		}

		if(OPidx-2 > 0)
		{
			if(*M_F_O_V.at(OPidx-2).c_str() == '-')
			{
				M_F_O_V.at(OPidx-2) = "+";
				M_F_O_V.at(OPidx-1) = "-" + M_F_O_V.at(OPidx-1);
			}

		}
		Middle_Operation = to_string(Eq_Calculation( atof(M_F_O_V.at(OPidx-1).c_str()),OP ,atof(M_F_O_V.at(OPidx+1).c_str())));

		if(0 == atof(Middle_Operation.c_str()))
		{
			if(M_F_O_V.size() == 3)
			{
				M_F_O_V.clear();
				return false;
			}
			if(*M_F_O_V.at(OPidx+2).c_str() == '-')
			{
				M_F_O_V.at(OPidx+3) = "-" + M_F_O_V.at(OPidx+3);
				return true;
			}
			else
			{
				M_F_O_V.erase(M_F_O_V.begin()+OPidx-1,M_F_O_V.begin()+OPidx+3);
				return true;
			}
		}
		else
		{
			if(Mul)
				Middle_Operation+="T";
			else if(Div)
			{}
			else
				Middle_Operation+="x";

			M_F_O_V.insert(M_F_O_V.begin() + OPidx - 1, Middle_Operation);
			M_F_O_V.erase(M_F_O_V.begin()+OPidx,M_F_O_V.begin()+OPidx+3);
			Middle_Operation = "";   
			find=0;   
			return true;
		}
	}

	else if(strstr((char*)M_F_O_V.at(OPidx-1).c_str(),"T") != NULL || strstr((char*)M_F_O_V.at(OPidx+1).c_str(),"T") != NULL) //둘다or하나만 T가 있을때
	{
		if(OP == '+' || OP == '-')
			return false;

		if(strstr((char*)M_F_O_V.at(OPidx-1).c_str(),"T") != NULL)
		{
			if(M_F_O_V.at(OPidx-1) == "T")
				M_F_O_V.at(OPidx-1) = "1";
			else
			{
				for(iter = M_F_O_V.at(OPidx-1).begin(); iter != M_F_O_V.at(OPidx-1).end(); iter++, find++)
				{if(*iter == 'T'){M_F_O_V.at(OPidx-1).erase(find,1);break;}}
			}
		}
		if(strstr((char*)M_F_O_V.at(OPidx+1).c_str(),"T") != NULL)
		{
			if(M_F_O_V.at(OPidx+1) == "T")
				M_F_O_V.at(OPidx+1) = "1";
			else
			{
				for(iter = M_F_O_V.at(OPidx+1).begin(); iter != M_F_O_V.at(OPidx+1).end(); iter++, find++)
				{if(*iter == 'T'){M_F_O_V.at(OPidx+1).erase(find,1);break;}}
			}
		}

		if(OPidx-2 > 0)
		{
			if(*M_F_O_V.at(OPidx-2).c_str() == '-')
			{
				M_F_O_V.at(OPidx-2) = "+";
				M_F_O_V.at(OPidx-1) = "-" + M_F_O_V.at(OPidx-1);
			}
		}

		Middle_Operation = to_string(Eq_Calculation( atof(M_F_O_V.at(OPidx-1).c_str()),OP ,atof(M_F_O_V.at(OPidx+1).c_str())));
		Middle_Operation+="T";
		M_F_O_V.insert(M_F_O_V.begin() + OPidx - 1, Middle_Operation);
		M_F_O_V.erase(M_F_O_V.begin()+OPidx,M_F_O_V.begin()+OPidx+3);
		Middle_Operation = "";   
		find=0;      
		return true;
	}

	else if(strstr((char*)M_F_O_V.at(OPidx-1).c_str(),"x") != NULL || strstr((char*)M_F_O_V.at(OPidx+1).c_str(),"x") != NULL) //둘다or하나만 x가 있을때
	{
		if(OP == '+' || OP == '-')
			return false;

		if(strstr((char*)M_F_O_V.at(OPidx-1).c_str(),"x") != NULL)
		{
			if(M_F_O_V.at(OPidx-1) == "x")
				M_F_O_V.at(OPidx-1) = "1";
			else if(M_F_O_V.at(OPidx-1) == "-x")
				M_F_O_V.at(OPidx-1) = "-1";
			else
			{
				for(iter = M_F_O_V.at(OPidx-1).begin(); iter != M_F_O_V.at(OPidx-1).end(); iter++, find++)
				{if(*iter == 'x'){M_F_O_V.at(OPidx-1).erase(find,1);break;}}
			}
		}
		if(strstr((char*)M_F_O_V.at(OPidx+1).c_str(),"x") != NULL)
		{
			if(M_F_O_V.at(OPidx+1) == "x")
				M_F_O_V.at(OPidx+1) = "1";
			else if(M_F_O_V.at(OPidx+1) == "-x")
				M_F_O_V.at(OPidx+1) = "-1";
			else
			{
				for(iter = M_F_O_V.at(OPidx+1).begin(); iter != M_F_O_V.at(OPidx+1).end(); iter++, find++)
				{if(*iter == 'x'){M_F_O_V.at(OPidx+1).erase(find,1);break;}}
			}
		}
		if(OPidx-2 > 0)
		{
			if(*M_F_O_V.at(OPidx-2).c_str() == '-')
			{
				M_F_O_V.at(OPidx-2) = "+";
				M_F_O_V.at(OPidx-1) = "-" + M_F_O_V.at(OPidx-1);
			}
		}
		Middle_Operation = to_string(Eq_Calculation( atof(M_F_O_V.at(OPidx-1).c_str()),OP ,atof(M_F_O_V.at(OPidx+1).c_str())));
		Middle_Operation+="x";
		M_F_O_V.insert(M_F_O_V.begin() + OPidx - 1, Middle_Operation);
		M_F_O_V.erase(M_F_O_V.begin()+OPidx,M_F_O_V.begin()+OPidx+3);
		Middle_Operation = "";   
		find=0;      
		return true;
	}
	return false;
}
bool Upload::Eq_Bracket_Middle_Operation_Label_Data(int OPidx, char OP, vector<Label_Data> &K)
{   
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return false;
	}

	string::iterator iter;
	int find=0;
	Label_Data temp;
	string Middle_Operation = "";
	bool Linear_Equation_Check = false;
	bool Mul = false;
	bool Div = false;

	if((strstr((char*)K.at(OPidx-1).data.c_str(),"x") == NULL && strstr((char*)K.at(OPidx+1).data.c_str(),"x") == NULL)&&
		(strstr((char*)K.at(OPidx-1).data.c_str(),"T") == NULL && strstr((char*)K.at(OPidx+1).data.c_str(),"T") == NULL)) //둘다 정수형일때
	{
		if(data_list.at(OPidx).data == "-" && data_list.size() == 3 && data_list.at(OPidx).data == "-")
		{
			data_list.at(OPidx).data = "+";

			for(iter = K.at(OPidx+1).data.begin(); iter != K.at(OPidx+1).data.end(); iter++, find++)
			{if(*iter == '-'){K.at(OPidx+1).data.erase(find,1);break;}}
			find = 0;
		}
		else if(data_list.at(OPidx-2).data == "-")
			K.at(OPidx-1).data = "-" + K.at(OPidx-1).data;

		Middle_Operation = to_string(Eq_Calculation( atof(K.at(OPidx-1).data.c_str()),OP ,atof(K.at(OPidx+1).data.c_str())));

		if(atof(Middle_Operation.c_str()) == 0)
		{
			if(K.size() == 5)
			{
				if(OPidx == 3)
				{
					K.erase(K.begin()+OPidx-2,K.begin()+OPidx+2);
					return true;
				}
				else if(OPidx == 1)
				{
					K.erase(K.begin()+OPidx-1,K.begin()+OPidx+2);
					return true;
				}
			}
			else if(K.size() == 7)
			{
				if(OPidx == 5)
				{
					K.erase(K.begin()+OPidx-2,K.begin()+OPidx+2);
					return true;
				}
			}
			else if(K.size() == 3)
			{
				K.clear();
				return false;
			}
			else
			{
				K.erase(K.begin()+OPidx-1,K.begin()+OPidx+3);
				Middle_Operation = "";   
				find=0;   
				return true;
			}
		}

		if(atof(Middle_Operation.c_str()) < 0)
		{
			for(iter = Middle_Operation.begin(); iter != Middle_Operation.end(); iter++, find++)
			{if(*iter == '-'){Middle_Operation.erase(find,1);break;}}
			find = 0;
		}
		temp.data = Middle_Operation;
		K.insert(K.begin() + OPidx - 1, temp);
		K.erase(K.begin()+OPidx,K.begin()+OPidx+3);
		Middle_Operation = "";   
		find=0;
		return true;
	}

	if(strstr((char*)K.at(OPidx-1).data.c_str(),"T") != NULL && strstr((char*)K.at(OPidx+1).data.c_str(),"T") != NULL) //둘다 T가 있을때
	{
		if(OP == '*')
			Mul = true;
		else if(OP == '/')
			Div = true;

		if(Mul)
		{
			K.clear();
			data_list.clear();
			return false;
		}

		if(K.at(OPidx-1).data == "T")
			K.at(OPidx-1).data = "1";
		else
		{
			for(iter = K.at(OPidx-1).data.begin(); iter != K.at(OPidx-1).data.end(); iter++, find++)
			{if(*iter == 'T'){K.at(OPidx-1).data.erase(find,1);break;}}
			find = 0;
		}

		if(K.at(OPidx+1).data == "T")
			K.at(OPidx+1).data = "1";
		else
		{
			for(iter = K.at(OPidx+1).data.begin(); iter != K.at(OPidx+1).data.end(); iter++, find++)
			{if(*iter == 'T'){K.at(OPidx+1).data.erase(find,1);break;}}
			find = 0;
		}
		Label_Data temp;
		Middle_Operation = to_string(Eq_Calculation( atof(K.at(OPidx-1).data.c_str()),OP ,atof(K.at(OPidx+1).data.c_str())));
		if(atof(Middle_Operation.c_str()) == 0)
		{
			data_list.clear();
			return false;
		}
		Middle_Operation+="T";
		temp.data = Middle_Operation;
		K.insert(K.begin() + OPidx - 1, temp);
		K.erase(K.begin()+OPidx,K.begin()+OPidx+3);
		Middle_Operation = "";   
		find=0;   
		return true;
	}


	if(strstr((char*)K.at(OPidx-1).data.c_str(),"x") != NULL && strstr((char*)K.at(OPidx+1).data.c_str(),"x") != NULL) //둘다 x가 있을때
	{
		if(OP == '*')
			Mul = true;
		else if(OP == '/')
			Div = true;

		if(K.at(OPidx-1).data == "x")
			K.at(OPidx-1).data = "1";
		else if(K.at(OPidx-1).data == "-x")
			K.at(OPidx-1).data = "-1";
		else
		{
			for(iter = K.at(OPidx-1).data.begin(); iter != K.at(OPidx-1).data.end(); iter++, find++)
			{if(*iter == 'x'){K.at(OPidx-1).data.erase(find,1);break;}}
			find = 0;
		}

		if(K.at(OPidx+1).data == "x")
			K.at(OPidx+1).data = "1";
		else if(K.at(OPidx+1).data == "-x")
			K.at(OPidx+1).data = "-1";
		else
		{
			for(iter = K.at(OPidx+1).data.begin(); iter != K.at(OPidx+1).data.end(); iter++, find++)
			{if(*iter == 'x'){K.at(OPidx+1).data.erase(find,1);break;}}
			find = 0;
		}

		if(OPidx-1 > 0)
		{
			if(*K.at(OPidx-2).data.c_str() == '-')
			{
				if(atof(K.at(OPidx-1).data.c_str()) > 0)
				{
					K.at(OPidx-2).data = "+";
					K.at(OPidx-1).data = "-" + K.at(OPidx-1).data;
				}
				else
				{
					for(iter = K.at(OPidx-1).data.begin(); iter != K.at(OPidx-1).data.end(); iter++, find++)
					{if(*iter == '-'){K.at(OPidx-1).data.erase(find,1);break;}}
					find = 0;
					K.at(OPidx-2).data = "+";
				}
			}
		}

		Middle_Operation = to_string(Eq_Calculation( atof(K.at(OPidx-1).data.c_str()),OP ,atof(K.at(OPidx+1).data.c_str())));

		if(atof(Middle_Operation.c_str()) == 0)
		{
			data_list.clear();
			return false;
		}

		if(Mul)
			Middle_Operation+="T";
		else if(Div)
		{}
		else
			Middle_Operation+="x";
		temp.data = Middle_Operation;
		K.insert(K.begin() + OPidx - 1, temp);
		K.erase(K.begin()+OPidx,K.begin()+OPidx+3);
		Middle_Operation = "";   
		find=0;   
		return true;
	}

	if(strstr((char*)K.at(OPidx-1).data.c_str(),"T") != NULL || strstr((char*)K.at(OPidx+1).data.c_str(),"T") != NULL) //둘다or하나만 T가 있을때
	{
		if(OP == '+' || OP == '-')
			return false;

		if(strstr((char*)K.at(OPidx-1).data.c_str(),"T") != NULL)
		{
			if(K.at(OPidx-1).data == "T")
				K.at(OPidx-1).data = "1";
			else
			{
				for(iter = K.at(OPidx-1).data.begin(); iter != K.at(OPidx-1).data.end(); iter++, find++)
				{if(*iter == 'T'){K.at(OPidx-1).data.erase(find,1);break;}}
			}
		}
		if(strstr((char*)K.at(OPidx+1).data.c_str(),"T") != NULL)
		{
			if(K.at(OPidx+1).data == "T")
				K.at(OPidx+1).data = "1";
			else
			{
				for(iter = K.at(OPidx+1).data.begin(); iter != K.at(OPidx+1).data.end(); iter++, find++)
				{if(*iter == 'T'){K.at(OPidx+1).data.erase(find,1);break;}}
			}
		}
		Middle_Operation = to_string(Eq_Calculation( atof(K.at(OPidx-1).data.c_str()),OP ,atof(K.at(OPidx+1).data.c_str())));
		Middle_Operation+="T";
		Label_Data temp;
		temp.data = Middle_Operation;
		K.insert(K.begin() + OPidx - 1, temp);
		K.erase(K.begin()+OPidx,K.begin()+OPidx+3);
		Middle_Operation = "";   
		find=0;      
		return true;
	}

	if(strstr((char*)K.at(OPidx-1).data.c_str(),"x") != NULL || strstr((char*)K.at(OPidx+1).data.c_str(),"x") != NULL) //둘다or하나만 x가 있을때
	{
		if(OP == '+' || OP == '-')
			return false;

		if(strstr((char*)K.at(OPidx-1).data.c_str(),"x") != NULL)
		{
			if(K.at(OPidx-1).data == "x")
				K.at(OPidx-1).data = "1";
			else if(K.at(OPidx-1).data == "-x")
				K.at(OPidx-1).data = "-1";
			else
			{
				for(iter = K.at(OPidx-1).data.begin(); iter != K.at(OPidx-1).data.end(); iter++, find++)
				{if(*iter == 'x'){K.at(OPidx-1).data.erase(find,1);break;}}
			}
		}
		if(strstr((char*)K.at(OPidx+1).data.c_str(),"x") != NULL)
		{
			if(K.at(OPidx+1).data == "x")
				K.at(OPidx+1).data = "1";
			else if(K.at(OPidx+1).data == "-x")
				K.at(OPidx+1).data = "-1";
			else
			{
				for(iter = K.at(OPidx+1).data.begin(); iter != K.at(OPidx+1).data.end(); iter++, find++)
				{if(*iter == 'x'){K.at(OPidx+1).data.erase(find,1);break;}}
			}
		}

		Middle_Operation = to_string(Eq_Calculation( atof(K.at(OPidx-1).data.c_str()),OP ,atof(K.at(OPidx+1).data.c_str())));
		Middle_Operation+="x";
		Label_Data temp;
		temp.data = Middle_Operation;
		K.insert(K.begin() + OPidx - 1, temp);
		K.erase(K.begin()+OPidx,K.begin()+OPidx+3);
		Middle_Operation = "";   
		find=0;      
		return true;
	}
	return false;
}
void Upload::Eq_lineup_String(vector<string>& K)
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	string LineUpStr = "-";
	for(unsigned int i = 0; i < K.size(); i+=2)
	{
		if(K.size() == lineup_OM.size())
			break;

		if((strstr((char*)K.at(i).c_str(),"T") != NULL))
		{
			if(i == 0)
			{
				lineup_OM.push_back(K.at(i).c_str());            
				continue;
			}
			else
			{
				if(*K.at(i-1).c_str() == '+')
				{
					if(lineup_OM.size() == 0)
						lineup_OM.push_back(K.at(i));
					else
					{
						lineup_OM.push_back("+");
						lineup_OM.push_back(K.at(i));
					}
					continue;
				}

				else if(*K.at(i-1).c_str() == '-')
				{
					if(lineup_OM.size() == 0)
					{
						LineUpStr += K.at(i);
						lineup_OM.push_back(LineUpStr);
						LineUpStr = "-";
					}
					else
					{
						lineup_OM.push_back("-");
						lineup_OM.push_back(K.at(i));
					}
					continue;
				}
			}
		}
	}

	for(unsigned int i = 0; i < K.size(); i+=2)
	{
		if(K.size() == lineup_OM.size())
			break;

		if((strstr((char*)K.at(i).c_str(),"x") != NULL))
		{
			if(i == 0)
			{
				if(lineup_OM.size() != 0)
				{
					lineup_OM.push_back("+");
					lineup_OM.push_back(K.at(i).c_str());
				}
				else
					lineup_OM.push_back(K.at(i).c_str());
				continue;
			}
			else
			{
				if(*K.at(i-1).c_str() == '+')
				{
					if(lineup_OM.size() == 0)
						lineup_OM.push_back(K.at(i));
					else
					{
						lineup_OM.push_back("+");
						lineup_OM.push_back(K.at(i));
					}
					continue;
				}

				else if(*K.at(i-1).c_str() == '-')
				{
					lineup_OM.push_back("-");
					lineup_OM.push_back(K.at(i));
					continue;
				}
			}
		}
	}

	for(unsigned int i = 0; i < K.size(); i+=2)
	{
		if(K.size() == lineup_OM.size())
			break;

		if((strstr((char*)K.at(i).c_str(),"x") == NULL) && (strstr((char*)K.at(i).c_str(),"T") == NULL))
		{
			if(i == 0)
			{
				if(lineup_OM.size() !=0)
				{
					lineup_OM.push_back("+");
					lineup_OM.push_back(K.at(i));
				}
				else
					lineup_OM.push_back(K.at(i).c_str());
				continue;
			}
			else
			{
				if(*K.at(i-1).c_str() == '+')
				{
					lineup_OM.push_back("+");
					lineup_OM.push_back(K.at(i));
					continue;
				}

				else if(*K.at(i-1).c_str() == '-')
				{
					lineup_OM.push_back("-");
					lineup_OM.push_back(K.at(i));
					continue;
				}
			}
		}
	}
}
void Upload::Eq_lineup_Label_Data(vector<Label_Data>& K)
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	string LineUpStr = "-";
	for(unsigned int i = 0; i < K.size(); i+=2)
	{
		if(K.size() == lineup_OM.size())
			break;

		if((strstr((char*)K.at(i).data.c_str(),"T") != NULL))
		{
			if(i == 0)
			{
				lineup_OM.push_back(K.at(i).data.c_str());            
				continue;
			}
			else
			{
				if(*K.at(i-1).data.c_str() == '+')
				{
					if(lineup_OM.size() == 0)
						lineup_OM.push_back(K.at(i).data);
					else
					{
						lineup_OM.push_back("+");
						lineup_OM.push_back(K.at(i).data);
					}
					continue;
				}

				else if(*K.at(i-1).data.c_str() == '-')
				{
					if(lineup_OM.size() == 0)
					{
						LineUpStr += K.at(i).data;
						lineup_OM.push_back(LineUpStr);
						LineUpStr = "-";
					}
					else
					{
						lineup_OM.push_back("-");
						lineup_OM.push_back(K.at(i).data);
					}
					continue;
				}
			}
		}
	}

	for(unsigned int i = 0; i < K.size(); i+=2)
	{
		if(K.size() == lineup_OM.size())
			break;

		if((strstr((char*)K.at(i).data.c_str(),"x") != NULL))
		{
			if(i == 0)
			{
				if(lineup_OM.size() != 0)
				{
					lineup_OM.push_back("+");
					lineup_OM.push_back(K.at(i).data.c_str());
				}
				else
					lineup_OM.push_back(K.at(i).data.c_str());
				continue;
			}
			else
			{
				if(*K.at(i-1).data.c_str() == '+')
				{
					if(lineup_OM.size() == 0)
						lineup_OM.push_back(K.at(i).data);
					else
					{
						lineup_OM.push_back("+");
						lineup_OM.push_back(K.at(i).data);
					}
					continue;
				}

				else if(*K.at(i-1).data.c_str() == '-')
				{
					if(lineup_OM.size() == 0)
						lineup_OM.push_back("-" + K.at(i).data);					
					else
					{
						lineup_OM.push_back("-");
						lineup_OM.push_back(K.at(i).data);
					}
					continue;
				}
			}
		}
	}

	for(unsigned int i = 0; i < K.size(); i+=2)
	{
		if(K.size() == lineup_OM.size())
			break;

		if((strstr((char*)K.at(i).data.c_str(),"x") == NULL) && (strstr((char*)K.at(i).data.c_str(),"T") == NULL))
		{
			if(i == 0)
			{
				if(lineup_OM.size() !=0)
				{
					lineup_OM.push_back("+");
					lineup_OM.push_back(K.at(i).data);
				}
				else
					lineup_OM.push_back(K.at(i).data.c_str());
				continue;
			}
			else
			{
				if(*K.at(i-1).data.c_str() == '+')
				{
					lineup_OM.push_back("+");
					lineup_OM.push_back(K.at(i).data);
					continue;
				}

				else if(*K.at(i-1).data.c_str() == '-')
				{
					lineup_OM.push_back("-");
					lineup_OM.push_back(K.at(i).data);
					continue;
				}
			}
		}
	}
}
bool Upload::Eq_Lvalue_Rvalue_Groupage()
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return false;
	}

	for(unsigned int i = 0; i < data_list.size(); i+=2)
	{
		if((strstr((char*)data_list.at(i).data.c_str(),"T") != NULL))
			return true;//2차 방정식
	}

	return false;//1차 방정식
}
void Upload::Eq_LinearEquation_Final_ArithmeticOperation()
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	string::iterator iter;
	int find=0;
	bool SimpleEquation = false;
	bool LinearEquation = false;
	int RqualIdx=-1;
	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == '=')
		{
			RqualIdx = i;
			break;
		}   
	}

	find = 0;
	if((strstr((char*)data_list.at(RqualIdx+1).data.c_str(),"T") != NULL))
	{
		if(*data_list.at(RqualIdx+1).data.c_str() == 'T')
			data_list.at(RqualIdx+1).data = "1";
		else
		{
			for(iter = data_list.at(RqualIdx+1).data.begin(); iter != data_list.at(RqualIdx+1).data.end(); iter++, find++)
			{if(*iter == 'T'){data_list.at(RqualIdx+1).data.erase(find,1);break;}}
			find = 0;
		}
		LinearEquation = true;
	}
	else if((strstr((char*)data_list.at(RqualIdx+1).data.c_str(),"x") != NULL))
	{
		if(*data_list.at(RqualIdx+1).data.c_str() == 'x')
			data_list.at(RqualIdx+1).data = "1";
		else
		{
			for(iter = data_list.at(RqualIdx+1).data.begin(); iter != data_list.at(RqualIdx+1).data.end(); iter++, find++)
			{if(*iter == 'x'){data_list.at(RqualIdx+1).data.erase(find,1);break;}}
			find = 0;
		}
		SimpleEquation = true;
	}

	if(atof(data_list.at(RqualIdx+1).data.c_str()) >= 0)//양수라면
	{
		if(atof(data_list.at(RqualIdx+1).data.c_str()) == 0)
		{
			if(RqualIdx == 1)
			{
				data_list.clear();
				Label_Data temp;
				temp.data = "x";
				data_list.push_back(temp);
				temp.data = "=";
				data_list.push_back(temp);
				temp.data = "0";
				data_list.push_back(temp);
				return;
			}
			data_list.erase(data_list.begin()+RqualIdx, data_list.begin() +RqualIdx+2);
		}
		else
		{
			data_list.at(RqualIdx).data = "-";
			if(LinearEquation)
				data_list.at(RqualIdx+1).data += "T";
			else if(SimpleEquation)
				data_list.at(RqualIdx+1).data += "x";
		}
	}
	else//음수라면
	{
		data_list.at(RqualIdx).data = "+";
		if(LinearEquation)
			data_list.at(RqualIdx+1).data += "T";
		else if(SimpleEquation)
			data_list.at(RqualIdx+1).data += "x";
	}



	for(unsigned int i = RqualIdx+2; i < data_list.size(); i+=2)//연산자 변경
	{
		if(*data_list.at(i).data.c_str() == '+')
			data_list.at(i).data = "-";
		else if(*data_list.at(i).data.c_str() == '-')
			data_list.at(i).data = "+";
	}

	for(unsigned int i = 1; i < data_list.size(); i+=2)
	{
		if(*data_list.at(i).data.c_str() == '*')
		{
			Eq_Bracket_Middle_Operation_Label_Data(i, '*', data_list);
			i = -1;
			continue;
		}

		if(*data_list.at(i).data.c_str() == '/')
		{
			Eq_Bracket_Middle_Operation_Label_Data(i, '/', data_list);
			i = -1;
			continue;
		}
	}

	Eq_lineup_Label_Data(data_list);
	data_list.clear();
	Label_Data temp;
	for(unsigned int i = 0; i < lineup_OM.size(); i++)
	{
		temp.data = lineup_OM.at(i);
		data_list.push_back(temp);
	}
	lineup_OM.clear();

	for(unsigned int i = 1; i < data_list.size(); i+=2)
	{
		if(*data_list.at(i).data.c_str() == '*')
		{
			Eq_Bracket_Middle_Operation_Label_Data(i, '*', data_list);
			i = -1;
			continue;
		}

		if(*data_list.at(i).data.c_str() == '/')
		{
			Eq_Bracket_Middle_Operation_Label_Data(i, '/', data_list);
			i = -1;
			continue;
		}
	}

	int Tvalue = 0;
	for(unsigned int i = 1; i < data_list.size(); i+=2)
	{
		for(unsigned int j = 0; j < data_list.size(); j++)
		{
			if((strstr((char*)data_list.at(j).data.c_str(),"T") != NULL))
				Tvalue++;
		}

		if(Tvalue == 0)
		{
			Tvalue = 0;
			data_list.clear();
			return;
		}
		Tvalue = 0;

		if(*data_list.at(i).data.c_str() == '+')
		{
			if((strstr((char*)data_list.at(i-1).data.c_str(),"x") != NULL && strstr((char*)data_list.at(i+1).data.c_str(),"x") != NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"x") == NULL && strstr((char*)data_list.at(i+1).data.c_str(),"x") == NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"T") != NULL && strstr((char*)data_list.at(i+1).data.c_str(),"T") != NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"T") == NULL && strstr((char*)data_list.at(i+1).data.c_str(),"T") == NULL)) 
			{
				bool check = Eq_Bracket_Middle_Operation_Label_Data(i, '+', data_list);
				if(check)
				{
					i = -1;
					continue;
				}
				else
					continue;
			}
		}
		if(*data_list.at(i).data.c_str() == '-')
		{
			if((strstr((char*)data_list.at(i-1).data.c_str(),"x") != NULL && strstr((char*)data_list.at(i+1).data.c_str(),"x") != NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"x") == NULL && strstr((char*)data_list.at(i+1).data.c_str(),"x") == NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"T") != NULL && strstr((char*)data_list.at(i+1).data.c_str(),"T") != NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"T") == NULL && strstr((char*)data_list.at(i+1).data.c_str(),"T") == NULL)) 
			{
				bool check = Eq_Bracket_Middle_Operation_Label_Data(i, '-', data_list);
				if(check)
				{
					i = -1;
					continue;
				}
				else
					continue;
			}
		}
	}


	if(data_list.size() == 5)
	{
		if(strstr((char*)data_list.at(0).data.c_str(),"T") != NULL)
		{
			if(*data_list.at(0).data.c_str() == 'T')
				data_list.at(0).data = "1";
			else
			{
				for(iter = data_list.at(0).data.begin(); iter != data_list.at(0).data.end(); iter++, find++)
				{if(*iter == 'T'){data_list.at(0).data.erase(find,1);break;}}
				find = 0;
			}
		}
		if(strstr((char*)data_list.at(2).data.c_str(),"x") != NULL)
		{
			if(*data_list.at(2).data.c_str() == 'x')
				data_list.at(2).data = "1";
			else
			{
				for(iter = data_list.at(2).data.begin(); iter != data_list.at(2).data.end(); iter++, find++)
				{if(*iter == 'x'){data_list.at(2).data.erase(find,1);break;}}
				find = 0;
			}
		}
	}


	for(unsigned int i = 1; i < data_list.size(); i+=2)
	{
		if(*data_list.at(i).data.c_str() == '-')
			data_list.at(i+1).data = "-" + data_list.at(i+1).data;
	}

	if(data_list.size() == 5)
	{
		Eq_Quad_Eqn(atof(data_list.at(0).data.c_str()), atof(data_list.at(2).data.c_str()), atof(data_list.at(4).data.c_str()));
		return;
	}
	else if(data_list.size() == 3)
	{
		if(*data_list.at(0).data.c_str() == 'T')
			data_list.at(0).data = "1";
		else
		{
			for(iter = data_list.at(0).data.begin(); iter != data_list.at(0).data.end(); iter++, find++)
			{if(*iter == 'T'){data_list.at(0).data.erase(find,1);break;}}
			find = 0;
		}
		if(*data_list.at(2).data.c_str() == 'x')
		{
			data_list.at(2).data = "1";
			Eq_Quad_Eqn(atof(data_list.at(0).data.c_str()), atof(data_list.at(2).data.c_str()), 0);
			return;
		}
		else if(strstr((char*)data_list.at(2).data.c_str(),"x") != NULL)
		{
			if(*data_list.at(2).data.c_str() == 'x')
				data_list.at(2).data = "1";
			else
			{
				for(iter = data_list.at(2).data.begin(); iter != data_list.at(2).data.end(); iter++, find++)
				{if(*iter == 'x'){data_list.at(2).data.erase(find,1);break;}}
				find = 0;
			}
			Eq_Quad_Eqn(atof(data_list.at(0).data.c_str()), atof(data_list.at(2).data.c_str()), 0);
			return;
		}
		Eq_Quad_Eqn(atof(data_list.at(0).data.c_str()), 0, atof(data_list.at(2).data.c_str()));
		return;
	}
	else if(data_list.size() == 1)
	{
		Eq_Quad_Eqn(atof(data_list.at(0).data.c_str()), 0, 0);
		return;
	}
}
void Upload::Eq_SimpleEquation_Final_ArithmeticOperation()
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	string::iterator iter;
	int find=0;
	int RqualIdx=-1;
	for(unsigned int i = 0; i < data_list.size(); i++)
	{
		if(*data_list.at(i).data.c_str() == '=')
		{
			RqualIdx = i;
			break;
		}   
	}

	if(RqualIdx == -1)
	{
		data_list.clear();
		return;
	}

	if(data_list.size() == 3)
	{
		if(RqualIdx == 1 && (strstr((char*)data_list.at(RqualIdx-1).data.c_str(),"x") != NULL) && 
			(strstr((char*)data_list.at(RqualIdx+1).data.c_str(),"x") == NULL) && data_list.at(RqualIdx+1).data == "0")
		{
			data_list.at(RqualIdx-1).data = "x";
			data_list.at(RqualIdx+1).data = "0";
			return;
		}

		if(RqualIdx == 1 && (strstr((char*)data_list.at(RqualIdx-1).data.c_str(),"x") != NULL) && 
			(strstr((char*)data_list.at(RqualIdx+1).data.c_str(),"x") == NULL))
		{
			if(*data_list.at(RqualIdx-1).data.c_str() == 'x')
				data_list.at(RqualIdx-1).data = "1";
			else
			{
				for(iter = data_list.at(RqualIdx-1).data.begin(); iter != data_list.at(RqualIdx-1).data.end(); iter++, find++)
				{if(*iter == 'x'){data_list.at(RqualIdx-1).data.erase(find,1);break;}}
				find = 0;
			}
			data_list.at(2).data = to_string(Eq_Calculation(atof(data_list.at(2).data.c_str()), '/', atof(data_list.at(0).data.c_str())));
			data_list.at(0).data = "x";
			return;
		}
	}

	if(data_list.at(RqualIdx+1).data == "-")
	{
		data_list.at(RqualIdx+2).data = "-" + data_list.at(RqualIdx+2).data;
		data_list.erase(data_list.begin()+RqualIdx+1);
	}

	find = 0;
	if((strstr((char*)data_list.at(RqualIdx+1).data.c_str(),"x") != NULL))
	{
		for(iter = data_list.at(RqualIdx+1).data.begin(); iter != data_list.at(RqualIdx+1).data.end(); iter++, find++)
		{if(*iter == 'x'){data_list.at(RqualIdx+1).data.erase(find,1);break;}}
		find = 0;

		if(atof(data_list.at(RqualIdx+1).data.c_str()) > 0)//양수라면
		{
			data_list.at(RqualIdx).data = "-";
			data_list.at(RqualIdx+1).data = data_list.at(RqualIdx+1).data + "x";
		}
		else//음수라면
		{
			for(iter = data_list.at(RqualIdx+1).data.begin(); iter != data_list.at(RqualIdx+1).data.end(); iter++, find++)
			{if(*iter == '-'){data_list.at(RqualIdx+1).data.erase(find,1);break;}}
			find = 0;
			data_list.at(RqualIdx).data = "+";
			data_list.at(RqualIdx+1).data = data_list.at(RqualIdx+1).data + "x";
		}
	}
	else if(atof(data_list.at(RqualIdx+1).data.c_str()) > 0)//양수라면
		data_list.at(RqualIdx).data = "-";
	else//음수라면
	{
		for(iter = data_list.at(RqualIdx+1).data.begin(); iter != data_list.at(RqualIdx+1).data.end(); iter++, find++)
		{if(*iter == '-'){data_list.at(RqualIdx+1).data.erase(find,1);break;}}
		find = 0;
		data_list.at(RqualIdx).data = "+";
	}



	for(unsigned int i = RqualIdx+2; i < data_list.size(); i+=2)//연산자 변경
	{
		if(*data_list.at(i).data.c_str() == '+')
			data_list.at(i).data = "-";
		else if(*data_list.at(i).data.c_str() == '-')
			data_list.at(i).data = "+";
	}

	for(unsigned int i = 1; i < data_list.size(); i+=2)
	{
		if(*data_list.at(i).data.c_str() == '*')
		{
			Eq_Bracket_Middle_Operation_Label_Data(i, '*', data_list);
			i = -1;
			continue;
		}

		if(*data_list.at(i).data.c_str() == '/')
		{
			Eq_Bracket_Middle_Operation_Label_Data(i, '/', data_list);
			i = -1;
			continue;
		}
	}

	Eq_lineup_Label_Data(data_list);
	data_list.clear();
	Label_Data temp;
	for(unsigned int i = 0; i < lineup_OM.size(); i++)
	{
		temp.data = lineup_OM.at(i);
		data_list.push_back(temp);
	}
	lineup_OM.clear();

	for(unsigned int i = 1; i < data_list.size(); i+=2)
	{
		if(*data_list.at(i).data.c_str() == '*')
		{
			Eq_Bracket_Middle_Operation_Label_Data(i, '*', data_list);
			i = -1;
			continue;
		}

		if(*data_list.at(i).data.c_str() == '/')
		{
			Eq_Bracket_Middle_Operation_Label_Data(i, '/', data_list);
			i = -1;
			continue;
		}
	}

	int xvalue = 0;
	for(unsigned int j = 0; j < data_list.size(); j++)
	{
		if((strstr((char*)data_list.at(j).data.c_str(),"x") != NULL))
			xvalue++;
	}

	for(unsigned int i = 1; i < data_list.size(); i+=2)
	{
		if(xvalue == 0)
		{
			xvalue = 0;
			data_list.clear();
			return;
		}

		if(*data_list.at(i).data.c_str() == '+')
		{
			if((strstr((char*)data_list.at(i-1).data.c_str(),"x") != NULL && strstr((char*)data_list.at(i+1).data.c_str(),"x") != NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"x") == NULL && strstr((char*)data_list.at(i+1).data.c_str(),"x") == NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"T") != NULL && strstr((char*)data_list.at(i+1).data.c_str(),"T") != NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"T") == NULL && strstr((char*)data_list.at(i+1).data.c_str(),"T") == NULL)) 
			{
				bool check = Eq_Bracket_Middle_Operation_Label_Data(i, '+', data_list);
				if(check)
				{
					i = -1;
					continue;
				}
				else
					continue;
			}
		}
		if(*data_list.at(i).data.c_str() == '-')
		{
			if((strstr((char*)data_list.at(i-1).data.c_str(),"x") != NULL && strstr((char*)data_list.at(i+1).data.c_str(),"x") != NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"x") == NULL && strstr((char*)data_list.at(i+1).data.c_str(),"x") == NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"T") != NULL && strstr((char*)data_list.at(i+1).data.c_str(),"T") != NULL) ||
				(strstr((char*)data_list.at(i-1).data.c_str(),"T") == NULL && strstr((char*)data_list.at(i+1).data.c_str(),"T") == NULL)) 
			{
				bool check = Eq_Bracket_Middle_Operation_Label_Data(i, '-', data_list);
				if(check)
				{
					i = -1;
					continue;
				}
				else
					continue;
			}
		}
	}

	if(data_list.size() == 0)
		return;

	if(data_list.size() == 1)
	{
		if((strstr((char*)data_list.at(0).data.c_str(),"x") != NULL))
		{
			data_list.clear();
			temp.data = "x";
			data_list.push_back(temp);
			temp.data = "=";
			data_list.push_back(temp);
			temp.data = "0";
			data_list.push_back(temp);
		}
		else
		{
			data_list.clear();
			return;
		}
		return;
	}

	if(data_list.size() == 3)
	{
		if(*data_list.at(1).data.c_str() == '+')
		{
			if(atof(data_list.at(2).data.c_str()) < 0)//음수라면
			{
				for(iter = data_list.at(2).data.begin(); iter != data_list.at(2).data.end(); iter++, find++)
				{if(*iter == '-'){data_list.at(2).data.erase(find,1);break;}}
				find = 0;
			}   
			else if(atof(data_list.at(2).data.c_str()) > 0)//양수라면
				data_list.at(2).data = "-" + data_list.at(2).data;
		}
		else if(*data_list.at(1).data.c_str() == '-')
			data_list.at(1).data == "=";
	}
	data_list.at(1).data = "=";

	if(data_list.at(0).data == "x")
		data_list.at(0).data = "1";
	else
	{
		for(iter = data_list.at(0).data.begin(); iter != data_list.at(0).data.end(); iter++, find++)
		{if(*iter == 'x'){data_list.at(0).data.erase(find,1);break;}}
		find = 0;
	}
	data_list.at(2).data = to_string(Eq_Calculation(atof(data_list.at(2).data.c_str()), '/', atof(data_list.at(0).data.c_str())));
	data_list.at(0).data = "x";
}
void Upload::Eq_Quad_Eqn(double a, double b, double c)//근의 공식 코드
{
	if(data_list.size() <= 0)//data_list vector에 size가 0이면 return함
	{
		data_list.clear();
		return;
	}

	data_list.clear();
	Label_Data temp;
	double d, x, y;
	d=b*b-4*a*c;
	if(d>0)
	{
		x=(-b-sqrt(b*b-4*a*c))/(2*a);
		y=(-b+sqrt(b*b-4*a*c))/(2*a);		

		temp.data = "x ";
		data_list.push_back(temp);
		temp.data = "= ";
		data_list.push_back(temp);

		if(-1 < x && x < 1)
		{
			if(x == 0)
			{
				temp.data = to_string(0);
				data_list.push_back(temp);
			}
			else
			{
				temp.data = to_string(x);
				data_list.push_back(temp);
			}
		}
		else
		{
			int x_int = -1;
			x_int = x;
			if(x - x_int == 0)
			{
				temp.data = to_string(x_int);
				data_list.push_back(temp);
			}
			else
			{
				temp.data = to_string(x);
				data_list.push_back(temp);
			}
		}		
		temp.data = ", ";
		data_list.push_back(temp);
		temp.data = "x ";
		data_list.push_back(temp);
		temp.data = "= ";
		data_list.push_back(temp);


		if(-1 < y && y < 1)
		{
			if(y == 0)
			{
				temp.data = to_string(atoi(to_string(y).c_str()));
				data_list.push_back(temp);
			}
			else
			{
				temp.data = to_string(y);
				data_list.push_back(temp);
			}
		}
		else
		{
			int y_int = -1;
			y_int = y;
			if(y - y_int == 0)
			{
				temp.data = to_string(y_int);
				data_list.push_back(temp);
			}
			else
			{
				temp.data = to_string(y);
				data_list.push_back(temp);
			}
		}

	}
	else if(d==0)
	{ 
		x=b/(-2*a);
		temp.data = "x ";
		data_list.push_back(temp);
		temp.data = "= ";
		data_list.push_back(temp);

		if(-1 < x && x < 1)
		{
			if(x == 0)
			{
				temp.data = to_string(0);
				data_list.push_back(temp);
			}
			else
			{
				temp.data = to_string(x);
				data_list.push_back(temp);
			}
		}
		else
		{
			int x_int = -1;
			x_int = x;
			if(x - x_int == 0)
			{
				temp.data = to_string(x_int);
				data_list.push_back(temp);
			}
			else
			{
				temp.data = to_string(x);
				data_list.push_back(temp);
			}
		}
	}
	else
	{
		data_list.clear();
		temp.data = "No Answer";
		data_list.push_back(temp);
	}
}
void Upload::Real_Final_Deduct(IplImage* original, int x, int y, int width, int height)
{
	//출력과정에 필요한 변수 선언 및 초기화
	CvFont font;
	int hscale = 1;
	int vscale = 1;
	int linewidth = 2;
	int mx = data_list.at(0).left_top.x + x;
	int my = y - 15;
	int rx = width/2;
	int ry = y + height + 40;   
	string num = "";
	//출력과정 필요한 변수 선언 및 초기화

	if(data_list.size() == 7)
		LinearEquation = true;
	else
		SimpleEquation = true;

	int data_list_print_int = 0;
	int data_list_int = 0;

	for(unsigned int i = 0; i < Final_Modification_Print.size(); i++)
		Formula_Eq += Final_Modification_Print.at(i).data;

	for(unsigned int i = 0; i < data_list.size(); i++)
		Answer_Eq += data_list.at(i).data;
	char* ss = (char*)Answer_Eq.c_str();
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
	cvPutText(original, ss, cvPoint(x, my), &font, cvScalar(0,0,255));

	//결과출력
	mx +=20;
	int count = -1;
	int decimal, sign;
	int size = 20;
	char* rs;
	rs = _ecvt(atof(Formula_Eq.c_str()), size, &decimal, &sign);

	for(int i = size; i >= decimal; i--)
	{
		if(rs[i] == '0' && i == size-1)
		{
			for(int j = size; j > decimal; j--)
			{
				if(rs[j] == '0')
					count++;
				else
					break;
			}
		}
		rs[i] = rs[i-1];
		if(i == decimal)
			rs[i] = '.';
	}
	//double형의 범위를 벗어날때
	if(Formula_Eq.size() >= 30)
	{
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
		cvPutText(original, "Out of range", cvPoint(rx-size, ry), &font, cvScalar(0,0,255));
		mx = NULL;
		return;
	}
	mx = NULL;
}