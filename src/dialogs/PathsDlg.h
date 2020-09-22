#pragma once

#define PATH_COLUMN_PATH    0
#define PATH_COLUMN_SIZE    1

namespace dialogs
{
    class CPathsDlg;

    class CPathsDlg : public controls::CMyDialogEx
    {
        DECLARE_DYNAMIC(CPathsDlg)
    public:
        CPathsDlg(CWnd* pParent = nullptr);
        virtual ~CPathsDlg();
        enum { IDD = IDD_DIALOG_PATHS };
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);
        DECLARE_MESSAGE_MAP()
    protected:
        HICON m_hIcon;
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
    public:
        virtual BOOL OnInitDialog();
    public:
        config::CConfig *pConfig;
        std::wstring szPathsDialogResize;
        std::wstring szPathsListColumns;
        volatile bool bUpdate;
        std::vector<config::CPath> m_Paths;
    public:
        controls::CMyStatic m_StcPath;
        controls::CMyStatic m_StcSize;
        controls::CMyListCtrl m_LstPaths;
        controls::CMyEdit m_EdtPath;
        controls::CMyEdit m_EdtSize;
        controls::CMyButton m_BtnOK;
        controls::CMyButton m_BtnCancel;
        controls::CMyButton m_BtnDuplicate;
        controls::CMyButton m_BtnRemoveAll;
        controls::CMyButton m_BtnRemove;
        controls::CMyButton m_BtnAdd;
        controls::CMyButton m_BtnMoveUp;
        controls::CMyButton m_BtnMoveDown;
        controls::CMyButton m_BtnUpdate;
        controls::CMyButton m_BtnImport;
        controls::CMyButton m_BtnExport;
        controls::CMyButton m_BtnBrowse;
    public:
        afx_msg void OnDropFiles(HDROP hDropInfo);
        afx_msg void OnBnClickedOk();
        afx_msg void OnBnClickedCancel();
        afx_msg void OnLvnGetdispinfoListPaths(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnOdfindListPaths(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnLvnItemchangedListPaths(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg LRESULT OnNotifyFormat(WPARAM wParam, LPARAM lParam);
        afx_msg void OnBnClickedButtonDuplicate();
        afx_msg void OnBnClickedButtonRemoveAllPaths();
        afx_msg void OnBnClickedButtonRemovePath();
        afx_msg void OnBnClickedButtonAddPath();
        afx_msg void OnBnClickedButtonPathUp();
        afx_msg void OnBnClickedButtonPathDown();
        afx_msg void OnBnClickedButtonUpdatePath();
        afx_msg void OnEnChangeEditPathPath();
        afx_msg void OnEnChangeEditPathSize();
        afx_msg void OnBnClickedButtonImportPaths();
        afx_msg void OnBnClickedButtonExportPaths();
        afx_msg void OnBnClickedButtonBrowsePath();
        afx_msg void OnClose();
    public:
        void LoadWindowSettings();
        void SaveWindowSettings();
        void SetLanguage();
        bool AddToList(const std::wstring& szPath);
        void RedrawPath(int nId);
        void RedrawPaths();
        void UpdateFields(config::CPath &path);
        void ListSelectionChange();
    public:
        bool LoadPaths(const std::wstring& szFileXml);
        bool LoadPaths(config::xml::XmlDocumnent &doc);
        bool SavePaths(const std::wstring& szFileXml);
    };
}
