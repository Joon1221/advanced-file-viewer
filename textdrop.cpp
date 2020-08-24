#include <iostream>
#include <fstream>
#include <strstream>
#include <bitset>

#include "textdrop.h"
#include "wx/treectrl.h"
#include "wx/dirctrl.h"
#include "wx/dir.h"
#include "wx/splitter.h"
#include "wx/wfstream.h"
#include "wx/aui/auibook.h"
#include "wx/radiobox.h"
#include "wx/config.h"
#include "wx/confbase.h"
#include "wx/fileconf.h"

#include "lz77.h"
#include "input_window.h"

using namespace std;

string string_to_hex(const string& input);
string string_to_hex(const char *input, int len);


wxString TextDrop::removeLastDir(wxString path) {
    int lastBackSlashIndex = -1;
    for (int i = path.length()-1; i >= 0; i--) {
        if (path[i] == '/') {
            lastBackSlashIndex = i;
            break;
        }
    }
    
    if (lastBackSlashIndex == -1) {
        return path;
    }
    else if (lastBackSlashIndex == 0) {
        return path.substr(0, 1);
    }
    else {
        return path.substr(0, lastBackSlashIndex);
    }
}

typedef hash_map<const char *, int> NameIntMap;

TextDrop::TextDrop(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 850))
{
    /* Source code removed for privacy purposes */
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// tabBarMoved Handler
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TextDrop::tabBarMoved(wxCommandEvent& event) {
    cout << "TextDrop::tabBarMoved: GetSelection():" << m_contentViewNotebook->GetSelection() << endl;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// tabBarSelection Handler
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define CONTENTVIEW_RADIOBOX_ASCII 0
#define CONTENTVIEW_RADIOBOX_HEX 1
#define CONTENTVIEW_RADIOBOX_BINARY 2

void TextDrop::tabBarSelection(wxCommandEvent& event) {
    
    cout << "tabBarOriginalIndex.size():" << tabBarOriginalIndex.size() << endl;
    for (OrgIndexHashMap::iterator iter = tabBarOriginalIndex.begin();
         iter != tabBarOriginalIndex.end();
         ++iter) {
        cout << "Age of " << (iter->first).first << " is " << iter->second << endl;
    }
    
    int indexFound = -1;
    string keyToFind = m_contentViewNotebook->GetPageText(m_contentViewNotebook->GetSelection()).ToStdString();
    OrgIndexHashMap::iterator found = tabBarOriginalIndex.find(KEY(keyToFind, 0));
    if (found == tabBarOriginalIndex.end()) {
        cout << "not found" << endl;
    }
    else {
        cout << "(found->first).first: " << (found->first).first << " / found->second: " << found->second << endl;
        indexFound = found->second;
    }
    
    if (indexFound != -1) {
        contentViewRadioBox->SetSelection(textFieldRadioBoxSelection[indexFound]);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Radio Buttons(ASCII/Hex/Binary) Handler
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TextDrop::contentViewRadioBoxHandler(wxCommandEvent& event) {
    
    if (m_contentViewNotebook->GetPageCount() == 0) {
        return;
    }

    int index = tabBarOriginalIndex[KEY(m_contentViewNotebook->GetPageText(m_contentViewNotebook->GetSelection()).ToStdString(), 0)];
    textFieldRadioBoxSelection[index] = event.GetSelection();
    
    // get file name
    const wxString text = m_contentViewNotebook->GetPageText(m_contentViewNotebook->GetSelection());
    
    wxTextDataObject tdo(text);
    wxDropSource tds(tdo, m_dirContents);
    tds.DoDragDrop(wxDrag_CopyOnly);
    
    // get file path
    wxString path = m_fileTree->GetPath();
    
    wxString mystring = path + "/" + text;
    

    //==========================================================================
    // Process Radiobox Selection
    //==========================================================================

    if (event.GetSelection() == CONTENTVIEW_RADIOBOX_ASCII) {
        //======================================================================
        // ASCII
        //======================================================================

        // create new text control
        wxTextCtrl *newTextCtrl = new wxTextCtrl(m_contentViewNotebook, -1, wxEmptyString, wxPoint(-1, -1),
                                                 wxSize(800, 400), wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL, wxDefaultValidator, wxTextCtrlNameStr);
        newTextCtrl->SetFont( wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL) );
        
        // read in file data
        newTextCtrl->LoadFile(mystring);
        
        // add new notebook page
        m_contentViewNotebook->InsertPage(m_contentViewNotebook->GetSelection(), newTextCtrl, text.ToStdString(), true, wxNullBitmap);
    }

    else if (event.GetSelection() == CONTENTVIEW_RADIOBOX_HEX) {
        //======================================================================
        // Hexadecimal
        //======================================================================

        int widthOfHexSection = 300;
        // create ascii window
        wxSplitterWindow *tempSpl1 = new wxSplitterWindow(m_contentViewNotebook, -1, wxPoint(-1, -1), wxSize(-1, -1), wxSP_THIN_SASH);
        // create hexadecimal window
        wxSplitterWindow *tempSpl2 = new wxSplitterWindow(tempSpl1, -1, wxPoint(-1, -1), wxSize(widthOfHexSection, -1), wxSP_THIN_SASH);
        
        // create ascii text control
        wxTextCtrl *newAsciiTextCtrl = new wxTextCtrl(tempSpl1, -1, wxEmptyString, wxPoint(-1, -1),
                                                      wxSize(800, 400), wxTE_READONLY, wxDefaultValidator, wxTextCtrlNameStr);
        
        // create hexadecimal text control
        wxTextCtrl *newTextCtrl = new wxTextCtrl(tempSpl2, -1, wxEmptyString, wxPoint(-1, -1),
                                                 wxSize(widthOfHexSection, 400), wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL, wxDefaultValidator, wxTextCtrlNameStr);
        newTextCtrl->SetFont( wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL) );

        

        tempSpl1->SplitVertically(tempSpl2, newAsciiTextCtrl);
        
        //----------------------------------------------------------------------
        // format hexadecimal text
        //----------------------------------------------------------------------
        ifstream fin;
        fin.open(mystring.c_str());
        char data[10000];
        int size = 0;
        while (!fin.eof()) {
            data[size] = fin.get();
            if (!fin.eof()) {
                size++;
            }
        }
        cout << "-----------------------------------" << endl;
        cout << "size: " << size << endl;
        fin.close();

        string curStrInHex = string_to_hex(data, size);
        string curStrInHexAfterFormatting = "";
        for (int i = 0; i < curStrInHex.length(); i++) {
            curStrInHexAfterFormatting += curStrInHex[i];
            if (i % 32 == 31) {
                curStrInHexAfterFormatting += "\n";
            }
            else if (i < curStrInHex.length()-1 && i % 8 == 7) {
                curStrInHexAfterFormatting += " ";
            }
        }
        
        newTextCtrl->AppendText(curStrInHexAfterFormatting);
        //----------------------------------------------------------------------

        
        // add new noteboook page
        m_contentViewNotebook->InsertPage(m_contentViewNotebook->GetSelection(), tempSpl1, text.ToStdString(), true, wxNullBitmap);

    }
    else { // if (event.GetSelection() == CONTENTVIEW_RADIOBOX_BINARY) {
        //======================================================================
        // Binary
        //======================================================================

        // create new text control
        wxTextCtrl *newTextCtrl = new wxTextCtrl(m_contentViewNotebook, -1, wxEmptyString, wxPoint(-1, -1),
                                                 wxSize(800, 400), wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL, wxDefaultValidator, wxTextCtrlNameStr);
        newTextCtrl->SetFont( wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL) );


        //----------------------------------------------------------------------
        // format decimal text
        //----------------------------------------------------------------------
        ifstream fin;
        fin.open(mystring.c_str());
        char data[10000];
        int size = 0;
        while (!fin.eof()) {
            data[size] = fin.get();
            if (!fin.eof()) {
                size++;
            }
        }
        cout << "-----------------------------------" << endl;
        cout << "size: " << size << endl;
        fin.close();

        string curStrAfterFormatting = "";
        
        for (int j = 0; j < size; j++) {
            curStrAfterFormatting += bitset<8>(data[j]).to_string();
            
            cout << j << endl;
            
            if (j % 8 == 7) {
                curStrAfterFormatting += "\n";
            }
            else if (j < size-1 && j % 2 == 1) {
                curStrAfterFormatting += " ";
            }
        }
        
        newTextCtrl->AppendText(curStrAfterFormatting);
        //----------------------------------------------------------------------
    
        // add new notebook page
        m_contentViewNotebook->InsertPage(m_contentViewNotebook->GetSelection(), newTextCtrl, text.ToStdString(), true, wxNullBitmap);
    }
    
    // since new notebook page is added, remove old one
    m_contentViewNotebook->RemovePage(m_contentViewNotebook->GetSelection()+1);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// OnSelectFile Handler
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TextDrop::OnSelectFile(wxListEvent& event) {
    curSelectedFileIndex = event.GetIndex();
    
    // get filename
    const wxString text = m_dirContents->GetItemText(event.GetIndex());
    

    wxTextDataObject tdo(text);
    wxDropSource tds(tdo, m_dirContents);
    tds.DoDragDrop(wxDrag_CopyOnly);
    
    // get file path
    wxString path = m_fileTree->GetPath();
    
    wxString mystring = path + "/" + text;
    
    // update tab bar
    tabBarOriginalIndex[KEY(text.ToStdString(), 0)] = textFieldRadioBoxSelection.size();

    // intialize new radio box selection value as 0 (ASCII)
    textFieldRadioBoxSelection.push_back(0);
    
    // create text control
    wxTextCtrl *newTextCtrl = new wxTextCtrl(m_contentViewNotebook, -1, wxEmptyString, wxPoint(-1, -1),
                                             wxSize(800, 400), wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL, wxDefaultValidator, wxTextCtrlNameStr);
    newTextCtrl->SetFont( wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL) );
    
    // add page onto notebook
    m_contentViewNotebook->AddPage(newTextCtrl, text);
    
    // change notebook selection
    m_contentViewNotebook->ChangeSelection(m_contentViewNotebook->GetPageCount()-1);
    newTextCtrl->LoadFile(mystring);
    contentViewRadioBox->SetSelection(0);

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Compress Button Handler
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TextDrop::OnBtnCompressPressed(wxCommandEvent& event) {
    
    //--------------------------------------------------------------------------
    // Set-up
    //--------------------------------------------------------------------------
    if (curSelectedFileIndex == -1) {
        wxMessageBox( wxT("No file selected"));
        return;
    }

    // get selected file name
    wxString text = m_dirContents->GetItemText(curSelectedFileIndex);
    
    wxTextDataObject tdo(text);
    wxDropSource tds(tdo, m_dirContents);
    tds.DoDragDrop(wxDrag_CopyOnly);
    
    // get selected file path
    wxString path = m_fileTree->GetPath();
    
    ifstream inFile;
    wxString filename;
    wxDir dir(path);
    string fileText;
    
    // convert wxString to const char*
    wxString mystring = path + "/" + text;
    string stlstring = string(mystring.mb_str());
    const char* stlstring2 = stlstring.c_str();
    
    // open original file stream
    int fileSize = InputWindow::getFileSize(stlstring2);
    ifstream input(stlstring2);
    
    // open compressed file stream
    ofstream of(path.ToStdString() + "/" + text + ".lz77");

    //--------------------------------------------------------------------------
    // Compress File
    //--------------------------------------------------------------------------

    // compress file
    lz77Compress(input, fileSize, of);
    
    //--------------------------------------------------------------------------
    // Clean Up
    //--------------------------------------------------------------------------
    // deselect original file and select compressed(.lz77) file

    of.close();
    
    const wxString textToFind = m_dirContents->GetItemText(curSelectedFileIndex) + ".lz77";
    
    long orgItemFound = m_dirContents->FindItem(-1, text, true);
    long itemFound = m_dirContents->FindItem(-1, textToFind, true);
    
    OnSelect(event);
    
    if (orgItemFound != -1) {
        m_dirContents->SetItemState(orgItemFound, 0, wxLIST_STATE_SELECTED);
    }
    if (itemFound != -1) {
        m_dirContents->SetItemState(itemFound, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Decompress Button Handler
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void TextDrop::OnBtnDecompressPressed(wxCommandEvent& event) {
    
    //--------------------------------------------------------------------------
    // Set-up
    //--------------------------------------------------------------------------
    if (curSelectedFileIndex == -1) {
        wxMessageBox( wxT("No file selected"));
        return;
    }
    
    // get selected file name
    wxString text = m_dirContents->GetItemText(curSelectedFileIndex);
    
    wxTextDataObject tdo(text);
    wxDropSource tds(tdo, m_dirContents);
    tds.DoDragDrop(wxDrag_CopyOnly);
    
    // get selected file path
    wxString path = m_fileTree->GetPath();
    
    
    ifstream inFile;
    wxString filename;
    wxDir dir(path);
    string fileText;
    
    // convert wxString to const char*
    wxString mystring = path + "/" + text;
    string stlstring = string(mystring.mb_str());
    const char* stlstring2 = stlstring.c_str();
    
    // open compressed file stream
    inFile.open(stlstring2);
    if (!inFile) {
        cout << "Unable to open file";
        exit(1);
    }
    
    // open decompressed file stream
    ofstream of(path.ToStdString() + "/" + text + ".org");

    //--------------------------------------------------------------------------
    // Decompress File
    //--------------------------------------------------------------------------

    // decompress file
    lz77Decompress(inFile, of);
    
    //--------------------------------------------------------------------------
    // Clean Up
    //--------------------------------------------------------------------------
    // deselect original file and select decompressed(.org) file

    of.close();
    inFile.close();
    
    const wxString textToFind = m_dirContents->GetItemText(curSelectedFileIndex) + ".org";
    
    long orgItemFound = m_dirContents->FindItem(-1, text, true);
    long itemFound = m_dirContents->FindItem(-1, textToFind, true);
    
    OnSelect(event);
    
    if (orgItemFound != -1) {
        m_dirContents->SetItemState(orgItemFound, 0, wxLIST_STATE_SELECTED);
    }
    if (itemFound != -1) {
        m_dirContents->SetItemState(itemFound, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    }
}

MyTextDropTarget::MyTextDropTarget(wxListCtrl *owner)
{
    m_owner = owner;
}

bool MyTextDropTarget::OnDropText(wxCoord x, wxCoord y,
                                  const wxString& data)
{
    
    return true;
    
}

void TextDrop::OnSelect(wxCommandEvent& event) {
    
    curSelectedFileIndex = -1;
    
    wxString path = m_fileTree->GetPath();
    
    // using wxConfig instead of writing wxFileConfig or wxRegConfig enhances
    // portability of the code
    wxConfig *config = new wxConfig("MyWxFramework");
    config->Write("LastWorkingPath", path);
    delete config; // save the config changed.
    
    showDirContent(path);
}

void TextDrop::showDirContent(wxString& path) {
    wxDir dir(path);
    
    wxString filename;
    bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
    
    int i = 0;
    
    m_dirContents->ClearAll();
    
    while ( cont )
    {
        m_dirContents->InsertItem(i, filename);
        cont = dir.GetNext(&filename);
        i++;
    }
    cout << path << endl;
}

void TextDrop::OnOpen(wxCommandEvent& WXUNUSED(event))
{

    wxFileDialog openFileDialog(this, _("Open XYZ file"), "", "",
                   "XYZ files (*.xyz)|*.xyz", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }

}

// convert ascii to hex
string string_to_hex(const string& input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    
    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

// convert hex to ascii
string string_to_hex(const char *input, int len) {
    static const char* const lut = "0123456789ABCDEF";
//    size_t len = input.length();
    
    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}
