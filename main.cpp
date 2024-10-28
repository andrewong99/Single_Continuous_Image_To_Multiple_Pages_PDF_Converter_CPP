#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <opencv2/opencv.hpp>
#include <hpdf.h>
#include <iostream>

class PDFCreatorApp : public wxApp {
public:
    virtual bool OnInit();
};

class PDFCreatorFrame : public wxFrame {
public:
    PDFCreatorFrame(const wxString& title);

private:
    void OnSelectImage(wxCommandEvent& event);
    void SaveAsPDF(const wxString& filePath);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_SelectImage = 1
};

wxBEGIN_EVENT_TABLE(PDFCreatorFrame, wxFrame)
    EVT_BUTTON(ID_SelectImage, PDFCreatorFrame::OnSelectImage)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(PDFCreatorApp);

bool PDFCreatorApp::OnInit() {
    PDFCreatorFrame* frame = new PDFCreatorFrame("Image to PDF Converter");
    frame->Show(true);
    return true;
}

PDFCreatorFrame::PDFCreatorFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 600)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxStaticText* label = new wxStaticText(panel, wxID_ANY, "Select an Image to Convert to PDF", wxPoint(300, 50), wxSize(300, 50), wxALIGN_CENTER);
    label->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    wxButton* selectButton = new wxButton(panel, ID_SelectImage, "Select Image", wxPoint(350, 150), wxSize(200, 50));
    selectButton->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}

void PDFCreatorFrame::OnSelectImage(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open Image file", "", "", "Image files (*.png;*.jpg;*.jpeg;*.bmp;*.gif)|*.png;*.jpg;*.jpeg;*.bmp;*.gif", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString filePath = openFileDialog.GetPath();
    SaveAsPDF(filePath);
}

void PDFCreatorFrame::SaveAsPDF(const wxString& filePath) {
    cv::Mat image = cv::imread(filePath.ToStdString(), cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        wxMessageBox("Could not open image.", "Error", wxICON_ERROR | wxOK);
        return;
    }

    int width = image.cols;
    int height = image.rows;
    int pageHeight = 2480;

    wxFileDialog saveFileDialog(this, "Save PDF file", "", "", "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString pdfPath = saveFileDialog.GetPath();

    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        wxMessageBox("Failed to create PDF object.", "Error", wxICON_ERROR | wxOK);
        return;
    }

    for (int i = 0; i < height; i += pageHeight) {
        int sliceHeight = std::min(pageHeight, height - i);
        cv::Rect roi(0, i, width, sliceHeight);
        cv::Mat slice = image(roi);

        std::vector<uchar> buf;
        cv::imencode(".png", slice, buf);  // PNG format preserves color accuracy

        HPDF_Page page = HPDF_AddPage(pdf);
        HPDF_Page_SetWidth(page, width);
        HPDF_Page_SetHeight(page, pageHeight);  // Fixed page height for consistency

        HPDF_Image pdf_image = HPDF_LoadPngImageFromMem(pdf, buf.data(), buf.size());
        if (!pdf_image) {
            wxMessageBox("Failed to create PDF image.", "Error", wxICON_ERROR | wxOK);
            HPDF_Free(pdf);
            return;
        }

        // Draw the image on the page
        HPDF_Page_DrawImage(page, pdf_image, 0, pageHeight - sliceHeight, width, sliceHeight);
    }

    if (HPDF_SaveToFile(pdf, pdfPath.ToStdString().c_str()) != HPDF_OK) {
        wxMessageBox("Failed to save PDF.", "Error", wxICON_ERROR | wxOK);
    } else {
        wxMessageBox("PDF saved successfully.", "Success", wxICON_INFORMATION | wxOK);
    }

    HPDF_Free(pdf);
}
