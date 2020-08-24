#include "wx/wx.h"
#include "wx/filedlg.h"
#include "wx/menu.h"

using namespace std;

#include "textdrop.h"

class my_first_app : public wxApp {
public:
    bool OnInit() {

        TextDrop *td = new TextDrop(wxT("TextDrop"));
        td->Show(true);
        
        return true;
    }

};
IMPLEMENT_APP(my_first_app)
