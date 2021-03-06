// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
#include <GdiPlus.h>
#include <vector>
#include <stdlib.h>
#pragma comment(lib, "gdiplus.lib")
//#include <atlbase.h>

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

bool GetEncoderClsid2(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0, size = 0;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	bool found = false;
	for (UINT ix = 0; !found && ix < num; ++ix) 
	{
		if (_wcsicmp(pImageCodecInfo[ix].MimeType, format) == 0) 
		{
			*pClsid = pImageCodecInfo[ix].Clsid;
			found = true;
			break;
		}
	}

	free(pImageCodecInfo);
	return found;
}

char lettter[62] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','s','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','S','Y','Z'};

int RangedRand(int range_min, int range_max)
{
	int u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min)
		+ range_min;

	return u;
}

//画一个字符
Gdiplus::Bitmap *letterToPic(wchar_t *s/*, Gdiplus::Color c,float py*/)
{
	using namespace Gdiplus;
	Gdiplus::Bitmap *bmp = new Gdiplus::Bitmap(24, 40, PixelFormat32bppARGB);
	Gdiplus::Graphics g(bmp);

	for (int i = 0; i < bmp->GetWidth(); i++)
	{
		for (int j = 0; j < bmp->GetHeight(); j++){
			bmp->SetPixel(i, j, Gdiplus::Color::Transparent);
		}
	}

	//设置画板的坐标原点为中点
	g.TranslateTransform(bmp->GetWidth()/2, bmp->GetHeight()/2);
	//以指定角度对画板进行旋转
	g.RotateTransform(-30+rand()%60);

	
	std::vector<std::wstring> fonts;
	fonts.push_back(L"微软雅黑");
	fonts.push_back(L"宋体");
	fonts.push_back(L"楷体");
	fonts.push_back(L"仿宋");

	std::vector<DWORD> colors;
	colors.push_back(Color::Yellow);
	colors.push_back(Color::Blue);
	colors.push_back(Color::Red);
	colors.push_back(Color::Green);
	colors.push_back(Color::Purple);
	Color clr;
	clr.SetFromCOLORREF(colors[rand() % colors.size()]);

	Gdiplus::PointF p(0,0);
	Gdiplus::RectF size;
	int fontSize = rand()%5+15;
	g.MeasureString((const WCHAR*)s,1,&Gdiplus::Font(&FontFamily(L"微软雅黑"/*fonts[rand() % fonts.size()].c_str()*/), fontSize, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPoint), p, &size);
	Gdiplus::PointF p1((bmp->GetWidth() - size.Width) / 2 - bmp->GetWidth() / 2, (bmp->GetHeight() - size.Height) / 2 - bmp->GetHeight() / 2);

	//Gdiplus::TextRenderingHint()   TextRenderingHintAntiAliasGridFit  TextRenderingHintSingleBitPerPixelGridFit   TextRenderingHintSingleBitPerPixel
	g.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);//使文字变得平滑
	g.DrawString((const WCHAR*)s,
		1,
		&Gdiplus::Font(&FontFamily(L"微软雅黑"/*fonts[rand() % fonts.size()].c_str()*/), fontSize, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPoint),
		p1, 
		&SolidBrush(clr));

	
	return bmp;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	
	OnYanzhengma();
	
	return 0;
}
//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::createCodePic()
{
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput StartupInput;  
	GdiplusStartup(&m_gdiplusToken,&StartupInput,NULL);

	DWORD dwBegin = GetTickCount();
	using namespace Gdiplus;

	srand((unsigned)time(NULL));
	std::string yzm;
	
	Gdiplus::Bitmap bmp(120, 40, PixelFormat32bppARGB);
	Graphics g(&bmp);

	std::vector<DWORD> colors;
	colors.push_back(Color::Yellow);
	colors.push_back(Color::Blue);
	colors.push_back(Color::Red);
	colors.push_back(Color::Green);
	colors.push_back(Color::Purple);

	for (int i = 0; i < bmp.GetWidth(); i++)
	{
		for (int j = 0; j < bmp.GetHeight(); j++){
			bmp.SetPixel(i, j, Color::Transparent);
		}
	}

	for(int i = 0; i < 5;i++){
		yzm = lettter[rand()%62];
		wchar_t toDrawText[2] = {0};
		swprintf_s(toDrawText, 2, L"%s", S_CA2T(yzm.c_str()));

		Gdiplus::Bitmap *sonBitmap = letterToPic(toDrawText);
			
	
		//Gdiplus::Point point(0, 24*i);//小图画到大图上的位置
		Gdiplus::RectF rct(24*i,0,24,40);
		g.DrawImage(sonBitmap,24*i,0/*(const Gdiplus::RectF)rct*/);//把小图画到大图上
	}


	// 干扰线
	Color lineColor(colors[rand() % colors.size()]);  //随机颜色
	for (int i = 0; i < 3; i++){
		PointF p1(RangedRand(0, bmp.GetWidth()), RangedRand(0, bmp.GetHeight()));
		PointF p2(RangedRand(0, bmp.GetWidth()), RangedRand(0, bmp.GetHeight()));

		g.DrawLine(&Gdiplus::Pen(lineColor), p1, p2);
	}

	Color lineColor2(colors[rand() % colors.size()]);
	for (int i = 0; i < 3; i++){
		PointF p1(RangedRand(0, bmp.GetWidth()), RangedRand(0, bmp.GetHeight()));
		PointF p2(RangedRand(0, bmp.GetWidth()), RangedRand(0, bmp.GetHeight()));

		g.DrawLine(&Gdiplus::Pen(lineColor2), p1, p2);
	}

	// 干扰点
	for (int i = 0; i < 50; i++)
	{
		PointF p(RangedRand(0, bmp.GetWidth()), RangedRand(0, bmp.GetHeight()));
		bmp.SetPixel(p.X, p.Y, colors[rand() % colors.size()]);
	}

	IStream* pIStream = NULL;
	CreateStreamOnHGlobal(NULL, TRUE, &pIStream);
	
	std::vector<BYTE> data;
	CLSID   clsID			 = GUID_NULL;
	wchar_t ext[_MAX_EXT]    = {0};
	wchar_t format[_MAX_EXT] = {L"image/"};
	std::wstring strPicPath = L".\\uires\\image\\yanzhengma.png";
	_wsplitpath_s(strPicPath.c_str(), NULL, 0, NULL, 0, NULL, 0, ext, _MAX_EXT);
	wcscat_s(format, _MAX_EXT, ext + 1);
	if (GetEncoderClsid2(format, &clsID))
	{
		Gdiplus::Status status = bmp.Save(pIStream, &clsID);
		if (status != Gdiplus::Status::Ok)
			return ;

		HGLOBAL hg = NULL;
		GetHGlobalFromStream(pIStream, &hg);

		int bufSize = GlobalSize(hg);
		data.resize(bufSize);

		LPVOID pImage = GlobalLock(hg);
		memcpy(&data[0], pImage, bufSize);
		GlobalUnlock(hg);

		pIStream->Release();
		//printf("success\n");
		bmp.Save(strPicPath.c_str(), &clsID, NULL);
	}
	else{
		printf("failed\n");
	}
}

void CMainDlg::OnYanzhengma()
{
	createCodePic();

	SImageWnd *img = FindChildByName2<SImageWnd>("code");
//	img->SetAttribute(L"skin",L"skin_yzm");
//	img->SetVisible(TRUE, TRUE);

	SStringT location_imgpath = L".\\uires\\image\\yanzhengma.png";
	IBitmap *bmp1  = SResLoadFromFile::LoadImage(location_imgpath);
	img->SetImage(bmp1);
}

void CMainDlg::OnClick(UINT nHitTest, CPoint point)
{
	 CRect codeRect;
	 SImageWnd *img = FindChildByName2<SImageWnd>("code");
	 if(img){
		  codeRect = img->GetWindowRect();
	 }
	 //判断点击区域是否是图片区域
	 if (codeRect.PtInRect(point)) { 
        createCodePic();

		SImageWnd *img = FindChildByName2<SImageWnd>("code");

		//加载新生成的验证码图片到控件上
		SStringT location_imgpath = L".\\uires\\image\\yanzhengma.png";
		IBitmap *bmp1  = SResLoadFromFile::LoadImage(location_imgpath);
		img->SetImage(bmp1);
    }

    SetMsgHandled(FALSE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

