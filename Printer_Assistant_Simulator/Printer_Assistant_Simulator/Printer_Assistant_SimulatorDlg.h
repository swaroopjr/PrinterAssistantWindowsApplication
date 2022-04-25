// Printer_Assistant_SimulatorDlg.h : header file
//

#pragma once


// CPrinter_Assistant_SimulatorDlg dialog
class CPrinter_Assistant_SimulatorDlg : public CDialog
{
// Construction
public:
	CPrinter_Assistant_SimulatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PRINTER_ASSISTANT_SIMULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	BOOL CPrinter_Assistant_SimulatorDlg::ManualDialogInit();
	void CPrinter_Assistant_SimulatorDlg::OnSerialComPortConnected();
	void CPrinter_Assistant_SimulatorDlg::OnSerialComPortDisConnected();
	afx_msg void OnBnClickedConnectButton();
	afx_msg void OnBnClickedDisconnectButton();
	afx_msg void OnBnClickedButtonSerialClean();
	afx_msg void OnBnClickedCheckHexFlag();
	afx_msg void OnBnClickedButtonBrowse();

	afx_msg void OnBnClickedButtonProcessPaperEmpty();
	afx_msg void OnBnClickedButtonCheckIfPrintEnabled();
	afx_msg void OnBnClickedButtonVeryHighDotCountPrint();
	afx_msg void OnBnClickedButtonHighDotCountPrint();
	afx_msg void OnBnClickedButtonMediumDotCountPrint();
	afx_msg void OnBnClickedButtonLowDotCountPrint();
	afx_msg void OnBnClickedButtonVeryLowDotCountPrint();	
	afx_msg void OnBnClickedWriteUserdataButton();
	afx_msg void OnBnClickedReadUserdataButton();
	afx_msg void OnBnClickedWriteSystemdataButton();
	afx_msg void OnBnClickedDisplayUserdataButton();
	afx_msg void OnBnClickedButtonPrintStatusDisplay();
	afx_msg void OnBnClickedReadSystemdataButton();
	afx_msg void OnBnClickedDisplaySystemdataButton();
};
