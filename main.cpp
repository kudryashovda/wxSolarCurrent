#include "interpolator.h"
#include "integrator.h"

#include <wx/wx.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MainFrame : public wxFrame {
public:
    explicit MainFrame(const wxString &title);
private:
    wxPanel *pnl;
    wxButton *btnCalculate;
    wxBoxSizer *mainSizer, *inputSizer, *memoSizer;
    wxTextCtrl *edtFirst, *edtLast;
    wxTextCtrl *memoInput, *memoOutput;

private:
    void OnPressbtnCalculate(wxMouseEvent& event);
    void onClickTextCtrl(wxMouseEvent& event);
};

IMPLEMENT_APP(MyApp)

[[maybe_unused]] bool MyApp::OnInit() {
    MainFrame *frame = new MainFrame(_("wxSolarCurrent"));
    frame->Show(true);

    return true;
}

MainFrame::MainFrame(const wxString &title):wxFrame(NULL, wxID_ANY, title) {
    CreateStatusBar(1);
    SetStatusText(wxT("To start enter or paste x,y data to the left field"), 0);

    pnl = new wxPanel(this, wxID_ANY);

    btnCalculate = new wxButton(pnl, wxID_ANY, "Calculate");

    btnCalculate->Bind(wxEVT_LEFT_UP, &MainFrame::OnPressbtnCalculate, this);

    edtFirst = new wxTextCtrl(pnl, wxID_ANY, wxT("300"));
    edtLast = new wxTextCtrl(pnl, wxID_ANY, wxT("1200"));

    memoInput = new wxTextCtrl(pnl, wxID_ANY, wxT(""),
                               wxDefaultPosition, wxDefaultSize,
                               wxTE_MULTILINE | wxTE_RICH,
                               wxDefaultValidator, wxTextCtrlNameStr);

    memoOutput = new wxTextCtrl(pnl, wxID_ANY, wxT(""),
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_MULTILINE | wxTE_RICH,
                                wxDefaultValidator, wxTextCtrlNameStr);

    edtFirst->Bind(wxEVT_LEFT_UP, &MainFrame::onClickTextCtrl, this);
    edtLast->Bind(wxEVT_LEFT_UP, &MainFrame::onClickTextCtrl, this);

    inputSizer = new wxBoxSizer(wxHORIZONTAL);

    inputSizer->Add(edtFirst, 1,  wxEXPAND | wxALL, 5);
    inputSizer->Add(edtLast, 1,  wxEXPAND | wxALL, 5);
    inputSizer->Add(btnCalculate, 0, wxALL, 5);

    memoSizer =  new wxBoxSizer(wxHORIZONTAL);
    memoSizer->Add(memoInput, 1, wxEXPAND | wxALL, 5);
    memoSizer->Add(memoOutput, 1, wxEXPAND | wxALL, 5);

    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(inputSizer, 0, wxALL, 5);
    mainSizer->Add(memoSizer, 1, wxEXPAND | wxALL, 5);

    pnl->SetSizer(mainSizer);

    mainSizer->Fit(this); // resize (fit) main window based on elements inside sizer

    wxSize ws = this->GetSize();

    SetMinSize(wxSize(ws.GetWidth(), 500));
    SetMaxSize(wxSize(ws.GetWidth(), 500));
    Centre();

    *memoInput << "Example:\nWavelength (nm)\tEQE\n300\t1\n1200\t1\n";
    *memoOutput << "Results:\n";
}

void MainFrame::OnPressbtnCalculate(wxMouseEvent& event) {
    event.Skip(); // if skip it - bugs with text selection appear
    SetStatusText("", 0);

    wxString tFirst = edtFirst->GetValue();
    wxString tLast = edtLast->GetValue();

    double dFirst, dLast;

    if(!tFirst.ToCDouble(&dFirst) or
            !tLast.ToCDouble(&dLast)) {
        SetStatusText("Error! Please enter real numbers", 0);
        return;
    }

    if (dLast < dFirst) {
        SetStatusText("Error! Last value mast be greater that First", 0);
        return;
    }

    wxString wxMemoText = MainFrame::memoInput->GetValue();

    std::string solarFluxFile = "amst.dat"s;
    Integrator solarCurrent(dFirst, dLast, std::string(wxMemoText.mb_str()), '\n', '\t', solarFluxFile);

    if (solarCurrent.getAmstDataSize() == 0) {
        SetStatusText("Error! No amst.dat found or incorrect file format", 0);
    }

    string str_value = to_string(solarCurrent.getSolarCurrent());

    // Memo output displays digits with zeros. This trick removes them
    constexpr int digits = 2; // after point

    std::size_t pos = str_value.find('.');
    if (pos != std::string::npos && str_value.size() > (pos + digits)) {
        str_value = str_value.substr(0, pos + digits);
    }

    *memoOutput << str_value << " mA/cm2\n";
}

// If entered any char then the field is cleared
void MainFrame::onClickTextCtrl(wxMouseEvent& event) {
    // get object where the event was generated
    wxTextCtrl *tc = (wxTextCtrl*)event.GetEventObject();
    event.Skip();

    wxString textValue = tc->GetValue();

    double nd = 0; // double value of number

    if(!textValue.ToCDouble(&nd)) tc->Clear();
}
