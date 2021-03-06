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
Gdiplus::Bitmap letterToPic(wchar_t s, Gdiplus::Color c,float py)
{
	using namespace Gdiplus;
	Gdiplus::Bitmap bmp(24, 40, PixelFormat32bppARGB);
	Gdiplus::Graphics g(&bmp);

	for (int i = 0; i < bmp.GetWidth(); i++)
	{
		for (int j = 0; j < bmp.GetHeight(); j++){
			bmp.SetPixel(i, j, Gdiplus::Color::Transparent);
		}
	}

	//设置画板的坐标原点为中点
	g.TranslateTransform(bmp.GetWidth()/2, bmp.GetHeight()/2);
	//以指定角度对画板进行旋转
	g.RotateTransform(rand()%30);

	Gdiplus::PointF p(5, 0);

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

	Gdiplus::RectF rect(0,0,24,40);
	//g.MeasureString((const WCHAR*)s,1,&Gdiplus::Font(&FontFamily(fonts[rand() % fonts.size()].c_str()), 25/*rand()%10+15*/, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPoint), p,&Gdiplus::RectF(0,0,24,40));

	//g.SetTextRenderingHint()
	g.DrawString((const WCHAR*)s,
		1,
		&Gdiplus::Font(&FontFamily(fonts[rand() % fonts.size()].c_str()), 25/*rand()%10+15*/, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPoint),
		p, 
		&SolidBrush(clr));

	return bitmap;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput StartupInput;  
	GdiplusStartup(&m_gdiplusToken,&StartupInput,NULL);

	DWORD dwBegin = GetTickCount();
	using namespace Gdiplus;
	wchar_t toDrawText[64] = {0};
	//w_char CA2W(LPCSTR)

	srand((unsigned)time(NULL));
	std::string yzm;
	for(int i = 0; i < 5;i++){
		yzm += lettter[rand()%62];
	}

	swprintf_s(toDrawText, 63, L"%s", S_CA2T(yzm.c_str()));
	Gdiplus::Bitmap bmp(120, 40, PixelFormat32bppARGB);
	Graphics g(&bmp);

	for (int i = 0; i < bmp.GetWidth(); i++)
	{
		for (int j = 0; j < bmp.GetHeight(); j++){
			bmp.SetPixel(i, j, Color::Transparent);
		}
	}

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

	int len = wcslen(toDrawText);
	for (int i = 0; i < len; i++)
	{
		Gdiplus::PointF p(i * 20, 0);

		Color clr;
		clr.SetFromCOLORREF(colors[rand() % colors.size()]);

		g.DrawString(toDrawText + i, 
			1, 
			&Gdiplus::Font(&FontFamily(fonts[rand() % fonts.size()].c_str()), 25/*rand()%10+15*/, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPoint), 
			p, 
			&SolidBrush(clr));

		//g.RotateTransform(30); 
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
			return 0;

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

	SImageWnd *img = FindChildByName2<SImageWnd>("code");
	img->SetAttribute(L"skin",L"skin_yzm");
	img->SetVisible(TRUE, TRUE);
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

