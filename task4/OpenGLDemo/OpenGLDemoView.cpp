
// OpenGLDemoView.cpp : COpenGLDemoView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "OpenGLDemo.h"
#endif

#include <GL/GLU.H> // �Ѿ�����GL.h
#include "CP_Curve2D.H"

#include "OpenGLDemoDoc.h"
#include "OpenGLDemoView.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <algorithm>
#include <vector>
#include <math.h>
#include "CP_Surface3D.h"
using namespace std;


vector<CP_Vector2D> ctrlPoints;//vectorģ��㣬de Casteljau�㷨ʱ�������
bool isReady = false;//���˫����ʼ��bezier���ߣ����������߻����Ƶ�켣
CP_Vector2D hoverPoint;//��¼����ѡ��Ŀ��Ƶ�����
const int besierSegment = 30;//��Ϊ���ٶ�
bool firstClick = false;//�е�һ�����ſ�ʼ��¼ mouse���꣬��������
// COpenGLDemoView

IMPLEMENT_DYNCREATE(COpenGLDemoView, CView)

BEGIN_MESSAGE_MAP(COpenGLDemoView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenGLDemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// COpenGLDemoView ����/����

COpenGLDemoView::COpenGLDemoView()
{
	// TODO: �ڴ˴����ӹ������

}

COpenGLDemoView::~COpenGLDemoView()
{
}

BOOL COpenGLDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

CP_Vector2D getBezierPoint(CP_Vector2D controlPoints[], double t, int i, int j)
{
	if (j == 0)
	{
		return controlPoints[i];
	}
	return getBezierPoint(controlPoints, t, i-1, j-1) * (1-t)+ getBezierPoint(controlPoints, t, i, j-1) * t;
}

CP_Vector2D getBezierPoint(vector<CP_Vector2D> controlPoints, double t, int i, int j)
{
	if (j == 0)
	{
		return controlPoints[i];
	}
	return getBezierPoint(controlPoints, t, i-1, j-1) * (1-t)+ getBezierPoint(controlPoints, t, i, j-1) * t;
}
/************************************************************************/
/* B(i,n t) = n!/(i!(n-i)!) t^i (1-t)*n-i                                                                     */
/************************************************************************/
double B(int i, int n, double t)
{
	double result = .1;
	for (unsigned j = n; j > n-i ; j-- ){
		result*=n;
	}
	for (unsigned j = i; j > 1; j--){
		result /= i;
	}
	for(unsigned j = 1; j <= i; j++){
		result *= t;
	}
	for(unsigned j = n ;j >= n-i; j--){
		result *= (1 - t);
	}
	return result;
}

//demo ��λ��
void drawBizerSample()
{
	//glTranslated(-8.0, 0.0, 0.0);    
	const int maxControlPoint = 4;
	CP_Vector2D controlPoints[maxControlPoint];
	controlPoints[0] = CP_Vector2D(0.0, 0.0);
	controlPoints[1] = CP_Vector2D(1.0, 3.0);
	controlPoints[2] = CP_Vector2D(2.0, 1.0);
	controlPoints[3] = CP_Vector2D(3.0, 3.0);
	
	/*
	CP_Vector2D p = getBezierPoint(controlPoints, 0.33333, 3, 3);
	CString msg;
	msg.Format(L"x=%f,y=%f", p.m_x, p.m_y);
	AfxMessageBox(_T(" " + msg));
	*/
	glColor3d(1.0, 0, 0);
	glBegin(GL_LINE_STRIP);
	double t = 0;
	for (int i = 0; i < besierSegment; i++)
	{
		t += 1.0/besierSegment;
		CP_Vector2D p = getBezierPoint(controlPoints, t, 3, 3);
		glVertex2d(p.m_x / 10, p.m_y / 10);
	}
	glEnd();

	glTranslated(-8.0, 0.0, 0.0);   
	glColor3d(1.0, 0, 0);
	glBegin(GL_LINE_STRIP);
	double t = 0;
	for (int i = 0; i < besierSegment; i++)
	{
		t += 1.0/besierSegment;
		CP_Vector2D p = getBezierPoint(controlPoints, t, 3, 3);
		glVertex2d(p.m_x / 10, p.m_y / 10);
	}

	glEnd();
	glFlush();
}

void COpenGLDemoView::OnDraw(CDC* pDC)
{
	COpenGLDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    wglMakeCurrent(pDC->m_hDC, m_hRC);
    
	glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	drawBizerSample();
	
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);

}


// COpenGLDemoView ��ӡ


void COpenGLDemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}


BOOL COpenGLDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void COpenGLDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void COpenGLDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӵ�ӡ����е���������
}

void COpenGLDemoView::OnRButtonUp(UINT nflags, CPoint point)
{
	isReady = true;
	CRect rect;
	GetClientRect(&rect);
	int x = point.x;
	int y = rect.Height() - point.y;

	CP_Vector2D p = CP_Vector2D(x, y);
	ctrlPoints.push_back(p);
	Invalidate(TRUE);
	CView::OnRButtonUp(nflags, point);
}
/*
void COpenGLDemoView::OnContextMenu(CWnd*   pWnd  , CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
*/

// COpenGLDemoView ���

#ifdef _DEBUG
void COpenGLDemoView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDemoDoc* COpenGLDemoView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDemoDoc)));
	return (COpenGLDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLDemoView ��Ϣ��������


int COpenGLDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// The PIXELFORMATDESCRIPTOR structure describes
	//		the pixel format of a drawing surface.
	PIXELFORMATDESCRIPTOR pfd =
	{ 
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
		1,                     			// version number 
		PFD_DRAW_TO_WINDOW |   	// support window 
		PFD_SUPPORT_OPENGL |   	// support OpenGL 
		PFD_DOUBLEBUFFER,	// double buffered
		PFD_TYPE_RGBA,
		24,                    	// 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     	// no alpha buffer 
		0,                     	// shift bit ignored 
		0,                     	// no accumulation buffer 
		0, 0, 0, 0,            	// accum bits ignored 
		32,                    	// 32-bit z-buffer (depth)
		0,                     	// no stencil buffer 
		0,                     	// no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     	// reserved 
		0, 0, 0                	// layer masks ignored 
	}; 
	CClientDC dc(this);
	// Get the best available match of pixel format for the device context
	// In other words, if this computer doesn't support features that I
	// asked for, try to get the next best thing.  i.e. 16-bit color mode
	// instead of 24-bit color mode.
	int pixelFormat = ChoosePixelFormat(dc.m_hDC, &pfd);

	// Set the pixel format to the best pixel format I can get (see above)
	// and if that operation fails, bring up a message box that tells the
	// user the error.
	if (!SetPixelFormat(dc.m_hDC, pixelFormat, &pfd))
	{
		MessageBox(_T("Error: Unable to Set Pixel Format in CGLTemplate1View::OnCreate( ) "),
			_T("Application Error"), MB_ICONERROR);
	}
	// Creates an OpenGL rendering context so that OpenGL knows how to draw
	// to this view. You can't use OpenGL in MFC without using the handle
	// that this function returns
	m_hRC = wglCreateContext(dc.m_hDC);

	return 0;
}


void COpenGLDemoView::OnDestroy()
{
	CView::OnDestroy();
	// Set : a specified OpenGL rendering context ==> NULL
	// Set : current rendering context ==> NULL
	wglMakeCurrent(NULL, NULL);

	// Delete the handle to an OpenGL rendering context 
	wglDeleteContext(m_hRC);
	m_hRC=NULL;
	//TODO how?
	//delete vector<CP_Vector2D *> ctrlPoints;

}


void COpenGLDemoView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CClientDC dc(this);
	wglMakeCurrent(dc.m_hDC, m_hRC);
	/*
	glViewport(0, 0, (GLsizei)cx, (GLsizei)cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double n = 100;
	//gluOrtho2D(-cx/n, cx/n, -cy/n, cy/n);
	gluOrtho2D(0, cx, 0, cy);
	//glOrtho(-cx/n, cx/n, -cy/n, cy/n, -d, d);
	glMatrixMode(GL_MODELVIEW);
	
	//glViewport(0, 0, (GLsizei)cx, (GLsizei)cy);
	*/
	
	glViewport(0, 0, (GLsizei)cx, (GLsizei)cy);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)cx, 0.0, (GLdouble)cy);
	

	wglMakeCurrent(NULL, NULL);

}


BOOL COpenGLDemoView::OnEraseBkgnd(CDC* pDC)
{

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

//������д������ƶ���ʵʱ�и������� �ѿ��Ƶ�����������
void COpenGLDemoView::OnMouseMove(UINT nFlags, CPoint point)
{
	/*
	if(!firstClick)
		return;

	CRect rect;
	GetClientRect(&rect);
	int x = point.x;
	int y = rect.Height() - point.y;

	hoverPoint = CP_Vector2D(x, y);
	*/
	CView::OnMouseMove(nFlags, point);
}

void COpenGLDemoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	if (ctrlPoints.size() >= 10){
		MessageBox(L"�ݹ��㷨�Ƚ����������Ƶ���ˣ��ѵõ�Ŷ�����Ͳ��ټ��˹�");
		return ;
	}

	CRect rect;
	GetClientRect(&rect);
	int x = point.x;
	int y = rect.Height() - point.y;

	CP_Vector2D p = CP_Vector2D(x, y);
	ctrlPoints.push_back(p);
	Invalidate(TRUE);
	isReady = false;
	
	CView::OnLButtonUp(nFlags, point);
}

void COpenGLDemoView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	
}