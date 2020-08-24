#include <iostream>
#include <ext/hash_map>
#include "wx/wx.h"
#include "wx/dnd.h"
#include "wx/listctrl.h"
#include "wx/dirctrl.h"
//#include "wx/notebook.h"
#include "wx/aui/auibook.h"

using namespace std;
//using namespace __gnu_cxx;
using __gnu_cxx::hash_map;

// __gnu_cxx::hash_map은 아래처럼 특수한 KEY를 만들어야만, 제대로 string을 비교할 수 있다.
typedef std::pair<std::string, unsigned int> KEY;

struct pairhash {
    size_t operator()(const KEY &p) const {
        return
        __gnu_cxx::hash<const char*>()(p.first.c_str()) ^
        __gnu_cxx::hash<unsigned int>()(p.second);
    }
};

typedef hash_map<KEY, int, pairhash> OrgIndexHashMap;

class TextDrop : public wxFrame
{
private:
    wxString removeLastDir(wxString path); // "/a/b/c" -> "/a/b"
public:
    TextDrop(const wxString& title);
    
    void OnSelect(wxCommandEvent& event);
    void showDirContent(wxString& path);

    void OnDragInit(wxListEvent& event);
    void OnSelectFile(wxListEvent& event);
    void OnBtnCompressPressed(wxCommandEvent &event);
    void OnBtnDecompressPressed(wxCommandEvent &event);

    void tabBarMoved(wxCommandEvent& event);
    void tabBarSelection(wxCommandEvent& event);
    void contentViewRadioBoxHandler(wxCommandEvent& event);
    

    wxGenericDirCtrl *m_fileTree;
    wxListCtrl *m_dirContents;
    wxPanel *m_contentViewPanel;
    
    wxAuiNotebook *m_contentViewNotebook;

    int curSelectedFileIndex;
    
    wxButton *btn_compress;
    wxButton *btn_decompress;
    
    void OnOpen(wxCommandEvent& event);
    
    wxMenuBar *menubar;
    wxMenu *file;
    
    wxRadioBox *contentViewRadioBox;
    
    OrgIndexHashMap tabBarOriginalIndex;
    vector<int> textFieldRadioBoxSelection;
};


class MyTextDropTarget : public wxTextDropTarget
{
public:
    MyTextDropTarget(wxListCtrl *owner);
    
    virtual bool OnDropText(wxCoord x, wxCoord y,
                            const wxString& data);
    
    wxListCtrl *m_owner;
    
};
