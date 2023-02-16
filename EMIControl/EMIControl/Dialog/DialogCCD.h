#pragma once

#define CCD_IMAGE_WIDTH							400
#define CCD_IMAGE_HEIGHT						400
#define CCD_IMG_RECT_MAX_NUM					1

// CDialogCCD ��ȭ �����Դϴ�.

class CDialogCCD : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogCCD)

public:
	CDialogCCD(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogCCD();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CCD };
public:
	void SetReadImage(char *path, double x, double y, double width, double height, double area, char* cell_id);
protected:
	C_CtrlImageRect *m_dlgImgRect[CCD_IMG_RECT_MAX_NUM];
	Hobject m_HalImage[CCD_IMG_RECT_MAX_NUM];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	double m_edit_ccd_x;
	double m_edit_ccd_y;
	double m_edit_ccd_width;
	double m_edit_ccd_height;
	double m_edit_ccd_area;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_edit_strCellID;
};
