///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../library/customButton.h"
#include "../library/customGrid.h"

#include "guiGenerated.h"

///////////////////////////////////////////////////////////////////////////

MainDialogGenerated::MainDialogGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    m_menubar1 = new wxMenuBar( 0 );
    m_menu1 = new wxMenu();
    m_menuItem10 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("1. &Compare") ) + wxT('\t') + wxT("ALT-C"), wxEmptyString, wxITEM_NORMAL );
    m_menu1->Append( m_menuItem10 );

    m_menuItem11 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("2. &Synchronize...") ) + wxT('\t') + wxT("ALT-S"), wxEmptyString, wxITEM_NORMAL );
    m_menu1->Append( m_menuItem11 );

    m_menu1->AppendSeparator();

    wxMenuItem* m_menuItem14;
    m_menuItem14 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("S&ave configuration") ) + wxT('\t') + wxT("CTRL-S"), wxEmptyString, wxITEM_NORMAL );
    m_menu1->Append( m_menuItem14 );

    wxMenuItem* m_menuItem13;
    m_menuItem13 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("&Load configuration") ) + wxT('\t') + wxT("CTRL-L"), wxEmptyString, wxITEM_NORMAL );
    m_menu1->Append( m_menuItem13 );

    m_menu1->AppendSeparator();

    wxMenuItem* m_menuItem4;
    m_menuItem4 = new wxMenuItem( m_menu1, wxID_EXIT, wxString( _("&Quit") ) + wxT('\t') + wxT("CTRL-Q"), wxEmptyString, wxITEM_NORMAL );
    m_menu1->Append( m_menuItem4 );

    m_menubar1->Append( m_menu1, _("&File") );

    m_menu3 = new wxMenu();
    m_menu31 = new wxMenu();
    m_menuItemGerman = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("Deutsch") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemGerman );

    m_menuItemEnglish = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("English") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemEnglish );

    m_menuItemFrench = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("Français") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemFrench );

    m_menuItemItalian = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("Italiano") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemItalian );

    m_menuItemPolish = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("Język Polski") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemPolish );

    m_menuItemDutch = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("Nederlands") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemDutch );

    m_menuItemPortuguese = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("Português") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemPortuguese );

    m_menuItemJapanese = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("日本語") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemJapanese );

    m_menuItemChineseSimple = new wxMenuItem( m_menu31, wxID_ANY, wxString( _("简体中文") ) , wxEmptyString, wxITEM_RADIO );
    m_menu31->Append( m_menuItemChineseSimple );

    m_menu3->Append( -1, _("&Language"), m_menu31 );

    m_menu3->AppendSeparator();

    m_menuItemGlobSett = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("&Global settings") ) , wxEmptyString, wxITEM_NORMAL );
    m_menu3->Append( m_menuItemGlobSett );

    m_menuItem7 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("&Create batch job") ) , wxEmptyString, wxITEM_NORMAL );
    m_menu3->Append( m_menuItem7 );

    wxMenuItem* m_menuItem5;
    m_menuItem5 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("&Export file list") ) , wxEmptyString, wxITEM_NORMAL );
    m_menu3->Append( m_menuItem5 );

    m_menubar1->Append( m_menu3, _("&Advanced") );

    m_menu2 = new wxMenu();
    wxMenuItem* m_menuItem3;
    m_menuItem3 = new wxMenuItem( m_menu2, wxID_ABOUT, wxString( _("&About...") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
    m_menu2->Append( m_menuItem3 );

    m_menubar1->Append( m_menu2, _("&Help") );

    this->SetMenuBar( m_menubar1 );

    bSizer1 = new wxBoxSizer( wxVERTICAL );

    m_panel71 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER|wxTAB_TRAVERSAL );
    bSizer6 = new wxBoxSizer( wxHORIZONTAL );


    bSizer6->Add( 15, 0, 0, wxALIGN_CENTER_VERTICAL, 5 );

    wxBoxSizer* bSizer30;
    bSizer30 = new wxBoxSizer( wxHORIZONTAL );

    m_buttonCompare = new wxButtonWithImage( m_panel71, wxID_OK, _("&Compare"), wxDefaultPosition, wxSize( 180,37 ), 0 );
    m_buttonCompare->SetDefault();
    m_buttonCompare->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Black") ) );
    m_buttonCompare->SetToolTip( _("Compare both sides") );

    bSizer30->Add( m_buttonCompare, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_buttonAbort = new wxButton( m_panel71, wxID_CANCEL, _("Abort"), wxDefaultPosition, wxSize( 180,37 ), 0 );
    m_buttonAbort->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Tahoma") ) );
    m_buttonAbort->Enable( false );
    m_buttonAbort->Hide();

    bSizer30->Add( m_buttonAbort, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxRIGHT, 5 );

    bSizer6->Add( bSizer30, 0, wxALIGN_CENTER_VERTICAL, 5 );

    wxBoxSizer* bSizer55;
    bSizer55 = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sbSizer6;
    sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panel71, wxID_ANY, _("Compare by...") ), wxHORIZONTAL );

    wxBoxSizer* bSizer45;
    bSizer45 = new wxBoxSizer( wxVERTICAL );

    m_radioBtnSizeDate = new wxRadioButton( m_panel71, wxID_ANY, _("File size and date"), wxDefaultPosition, wxDefaultSize, 0 );
    m_radioBtnSizeDate->SetValue( true );
    m_radioBtnSizeDate->SetToolTip( _("Files are found equal if\n     - filesize\n     - last write time and date\nare the same.") );

    bSizer45->Add( m_radioBtnSizeDate, 0, 0, 5 );

    m_radioBtnContent = new wxRadioButton( m_panel71, wxID_ANY, _("File content"), wxDefaultPosition, wxDefaultSize, 0 );
    m_radioBtnContent->SetToolTip( _("Files are found equal if\n     - file content\nis the same.") );

    bSizer45->Add( m_radioBtnContent, 0, wxTOP, 5 );

    sbSizer6->Add( bSizer45, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton14 = new wxBitmapButton( m_panel71, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    m_bpButton14->SetToolTip( _("Help") );

    m_bpButton14->SetToolTip( _("Help") );

    sbSizer6->Add( m_bpButton14, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    bSizer55->Add( sbSizer6, 0, wxALIGN_CENTER_VERTICAL, 2 );


    bSizer55->Add( 0, 4, 0, 0, 5 );

    bSizer6->Add( bSizer55, 0, wxALIGN_CENTER_VERTICAL, 5 );


    bSizer6->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    wxBoxSizer* bSizer56;
    bSizer56 = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sbSizer9;
    sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_panel71, wxID_ANY, _("Filter files") ), wxHORIZONTAL );

    m_bpButtonFilter = new wxBitmapButton( m_panel71, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW|wxFULL_REPAINT_ON_RESIZE );
    sbSizer9->Add( m_bpButtonFilter, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    wxBoxSizer* bSizer23;
    bSizer23 = new wxBoxSizer( wxVERTICAL );

    m_hyperlinkCfgFilter = new wxHyperlinkCtrl( m_panel71, wxID_ANY, _("Configure filter..."), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );

    m_hyperlinkCfgFilter->SetNormalColour( wxColour( 0, 0, 255 ) );
    m_hyperlinkCfgFilter->SetVisitedColour( wxColour( 0, 0, 255 ) );
    bSizer23->Add( m_hyperlinkCfgFilter, 0, wxALL, 5 );

    m_checkBoxHideFilt = new wxCheckBox( m_panel71, wxID_ANY, _("Hide filtered items"), wxDefaultPosition, wxDefaultSize, 0 );

    m_checkBoxHideFilt->SetToolTip( _("Choose to hide filtered files/directories from list") );

    bSizer23->Add( m_checkBoxHideFilt, 0, 0, 5 );

    sbSizer9->Add( bSizer23, 0, 0, 5 );

    bSizer56->Add( sbSizer9, 0, wxALIGN_CENTER_VERTICAL, 5 );


    bSizer56->Add( 0, 4, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    bSizer6->Add( bSizer56, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_buttonSync = new wxButtonWithImage( m_panel71, wxID_ANY, _("&Synchronize..."), wxDefaultPosition, wxSize( 180,37 ), 0 );
    m_buttonSync->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Black") ) );
    m_buttonSync->SetToolTip( _("Open synchronization dialog") );

    bSizer6->Add( m_buttonSync, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );


    bSizer6->Add( 15, 0, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

    m_panel71->SetSizer( bSizer6 );
    m_panel71->Layout();
    bSizer6->Fit( m_panel71 );
    bSizer1->Add( m_panel71, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    wxBoxSizer* bSizer91;
    bSizer91 = new wxBoxSizer( wxHORIZONTAL );

    m_panel11 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer92;
    bSizer92 = new wxBoxSizer( wxVERTICAL );

    sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel11, wxID_ANY, _("Drag && drop") ), wxHORIZONTAL );

    m_directoryLeft = new wxTextCtrl( m_panel11, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    sbSizer2->Add( m_directoryLeft, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_dirPickerLeft = new wxDirPickerCtrl( m_panel11, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST );
    sbSizer2->Add( m_dirPickerLeft, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer92->Add( sbSizer2, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

    m_panel11->SetSizer( bSizer92 );
    m_panel11->Layout();
    bSizer92->Fit( m_panel11 );
    bSizer91->Add( m_panel11, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_panel13 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer93;
    bSizer93 = new wxBoxSizer( wxVERTICAL );


    bSizer93->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    bSizerMiddle = new wxBoxSizer( wxHORIZONTAL );

    m_bpButtonSwap = new wxBitmapButton( m_panel13, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    m_bpButtonSwap->SetToolTip( _("Swap sides") );

    m_bpButtonSwap->SetToolTip( _("Swap sides") );

    bSizerMiddle->Add( m_bpButtonSwap, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer93->Add( bSizerMiddle, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );


    bSizer93->Add( 0, 0, 0, 0, 5 );

    m_panel13->SetSizer( bSizer93 );
    m_panel13->Layout();
    bSizer93->Fit( m_panel13 );
    bSizer91->Add( m_panel13, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    m_panel12 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer94;
    bSizer94 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer77;
    bSizer77 = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* bSizer781;
    bSizer781 = new wxBoxSizer( wxVERTICAL );


    bSizer781->Add( 0, 3, 0, 0, 5 );

    m_bpButtonRemovePair = new wxBitmapButton( m_panel12, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 18,21 ), wxBU_AUTODRAW );
    m_bpButtonRemovePair->SetToolTip( _("Remove folder pair") );

    m_bpButtonRemovePair->SetToolTip( _("Remove folder pair") );

    bSizer781->Add( m_bpButtonRemovePair, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    m_bpButtonAddPair = new wxBitmapButton( m_panel12, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 18,21 ), wxBU_AUTODRAW );
    m_bpButtonAddPair->SetToolTip( _("Add folder pair") );

    m_bpButtonAddPair->SetToolTip( _("Add folder pair") );

    bSizer781->Add( m_bpButtonAddPair, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    bSizer77->Add( bSizer781, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    wxStaticBoxSizer* sbSizer3;
    sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel12, wxID_ANY, _("Drag && drop") ), wxHORIZONTAL );

    m_directoryRight = new wxTextCtrl( m_panel12, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    sbSizer3->Add( m_directoryRight, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_dirPickerRight = new wxDirPickerCtrl( m_panel12, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST );
    sbSizer3->Add( m_dirPickerRight, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer77->Add( sbSizer3, 1, wxRIGHT|wxLEFT, 5 );

    bSizer94->Add( bSizer77, 0, wxEXPAND, 5 );

    m_panel12->SetSizer( bSizer94 );
    m_panel12->Layout();
    bSizer94->Fit( m_panel12 );
    bSizer91->Add( m_panel12, 1, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer1->Add( bSizer91, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

    m_scrolledWindowFolderPairs = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxHSCROLL|wxVSCROLL );
    m_scrolledWindowFolderPairs->SetScrollRate( 5, 5 );
    m_scrolledWindowFolderPairs->SetMinSize( wxSize( -1,0 ) );

    bSizerFolderPairs = new wxBoxSizer( wxVERTICAL );

    m_scrolledWindowFolderPairs->SetSizer( bSizerFolderPairs );
    m_scrolledWindowFolderPairs->Layout();
    bSizerFolderPairs->Fit( m_scrolledWindowFolderPairs );
    bSizer1->Add( m_scrolledWindowFolderPairs, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer( wxHORIZONTAL );

    m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer7;
    bSizer7 = new wxBoxSizer( wxVERTICAL );

    m_gridLeft = new CustomGridLeft( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    // Grid
    m_gridLeft->CreateGrid( 15, 4 );
    m_gridLeft->EnableEditing( false );
    m_gridLeft->EnableGridLines( true );
    m_gridLeft->EnableDragGridSize( true );
    m_gridLeft->SetMargins( 0, 0 );

    // Columns
    m_gridLeft->EnableDragColMove( false );
    m_gridLeft->EnableDragColSize( true );
    m_gridLeft->SetColLabelSize( 20 );
    m_gridLeft->SetColLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );

    // Rows
    m_gridLeft->EnableDragRowSize( false );
    m_gridLeft->SetRowLabelSize( 38 );
    m_gridLeft->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

    // Label Appearance

    // Cell Defaults
    m_gridLeft->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );
    bSizer7->Add( m_gridLeft, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxALL, 5 );

    m_panel1->SetSizer( bSizer7 );
    m_panel1->Layout();
    bSizer7->Fit( m_panel1 );
    bSizer2->Add( m_panel1, 1, wxEXPAND, 5 );

    m_panel3 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer18;
    bSizer18 = new wxBoxSizer( wxVERTICAL );

    m_gridMiddle = new CustomGridMiddle( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    // Grid
    m_gridMiddle->CreateGrid( 15, 1 );
    m_gridMiddle->EnableEditing( false );
    m_gridMiddle->EnableGridLines( true );
    m_gridMiddle->EnableDragGridSize( false );
    m_gridMiddle->SetMargins( 0, 0 );

    // Columns
    m_gridMiddle->SetColSize( 0, 45 );
    m_gridMiddle->EnableDragColMove( false );
    m_gridMiddle->EnableDragColSize( false );
    m_gridMiddle->SetColLabelSize( 20 );
    m_gridMiddle->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

    // Rows
    m_gridMiddle->EnableDragRowSize( false );
    m_gridMiddle->SetRowLabelSize( 0 );
    m_gridMiddle->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

    // Label Appearance

    // Cell Defaults
    m_gridMiddle->SetDefaultCellFont( wxFont( 12, 74, 90, 92, false, wxT("Arial") ) );
    m_gridMiddle->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
    bSizer18->Add( m_gridMiddle, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP|wxBOTTOM, 5 );

    m_panel3->SetSizer( bSizer18 );
    m_panel3->Layout();
    bSizer18->Fit( m_panel3 );
    bSizer2->Add( m_panel3, 0, wxRIGHT|wxLEFT|wxEXPAND, 5 );

    m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer10;
    bSizer10 = new wxBoxSizer( wxVERTICAL );

    m_gridRight = new CustomGridRight( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    // Grid
    m_gridRight->CreateGrid( 15, 4 );
    m_gridRight->EnableEditing( false );
    m_gridRight->EnableGridLines( true );
    m_gridRight->EnableDragGridSize( true );
    m_gridRight->SetMargins( 0, 0 );

    // Columns
    m_gridRight->EnableDragColMove( false );
    m_gridRight->EnableDragColSize( true );
    m_gridRight->SetColLabelSize( 20 );
    m_gridRight->SetColLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );

    // Rows
    m_gridRight->EnableDragRowSize( false );
    m_gridRight->SetRowLabelSize( 38 );
    m_gridRight->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

    // Label Appearance

    // Cell Defaults
    m_gridRight->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );
    bSizer10->Add( m_gridRight, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxALL, 5 );

    m_panel2->SetSizer( bSizer10 );
    m_panel2->Layout();
    bSizer10->Fit( m_panel2 );
    bSizer2->Add( m_panel2, 1, wxEXPAND, 5 );

    bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

    wxPanel* m_panel4;
    m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    bSizer3 = new wxBoxSizer( wxHORIZONTAL );

    bSizer58 = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sbSizer16;
    sbSizer16 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, _("Configuration") ), wxHORIZONTAL );

    m_bpButtonSave = new wxBitmapButton( m_panel4, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    m_bpButtonSave->SetToolTip( _("Save current configuration to file") );

    m_bpButtonSave->SetToolTip( _("Save current configuration to file") );

    sbSizer16->Add( m_bpButtonSave, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_bpButtonLoad = new wxBitmapButton( m_panel4, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    m_bpButtonLoad->SetToolTip( _("Load configuration from file") );

    m_bpButtonLoad->SetToolTip( _("Load configuration from file") );

    sbSizer16->Add( m_bpButtonLoad, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    wxArrayString m_choiceHistoryChoices;
    m_choiceHistory = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), m_choiceHistoryChoices, 0 );
    m_choiceHistory->SetSelection( 0 );
    m_choiceHistory->SetToolTip( _("Load configuration history (press DEL to delete items)") );

    sbSizer16->Add( m_choiceHistory, 1, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer58->Add( sbSizer16, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );


    bSizer58->Add( 0, 4, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    bSizer3->Add( bSizer58, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_panel112 = new wxPanel( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer64;
    bSizer64 = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sbSizer31;
    sbSizer31 = new wxStaticBoxSizer( new wxStaticBox( m_panel112, wxID_ANY, _("Filter view") ), wxHORIZONTAL );

    sbSizer31->SetMinSize( wxSize( 100,-1 ) );

    sbSizer31->Add( 0, 0, 1, wxEXPAND, 5 );

    m_bpButtonLeftOnly = new wxBitmapButton( m_panel112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    sbSizer31->Add( m_bpButtonLeftOnly, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButtonLeftNewer = new wxBitmapButton( m_panel112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    sbSizer31->Add( m_bpButtonLeftNewer, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButtonEqual = new wxBitmapButton( m_panel112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    sbSizer31->Add( m_bpButtonEqual, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButtonDifferent = new wxBitmapButton( m_panel112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    sbSizer31->Add( m_bpButtonDifferent, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButtonRightNewer = new wxBitmapButton( m_panel112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    sbSizer31->Add( m_bpButtonRightNewer, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButtonRightOnly = new wxBitmapButton( m_panel112, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    sbSizer31->Add( m_bpButtonRightOnly, 0, wxALIGN_CENTER_VERTICAL, 5 );


    sbSizer31->Add( 0, 0, 1, wxEXPAND, 5 );

    bSizer64->Add( sbSizer31, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

    m_panel112->SetSizer( bSizer64 );
    m_panel112->Layout();
    bSizer64->Fit( m_panel112 );
    bSizer3->Add( m_panel112, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM, 4 );

    wxBoxSizer* bSizer66;
    bSizer66 = new wxBoxSizer( wxVERTICAL );

    m_bpButton10 = new wxBitmapButton( m_panel4, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW );
    m_bpButton10->SetToolTip( _("Quit") );

    m_bpButton10->SetToolTip( _("Quit") );

    bSizer66->Add( m_bpButton10, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

    bSizer3->Add( bSizer66, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_panel4->SetSizer( bSizer3 );
    m_panel4->Layout();
    bSizer3->Fit( m_panel4 );
    bSizer1->Add( m_panel4, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer451;
    bSizer451 = new wxBoxSizer( wxHORIZONTAL );

    bSizer451->SetMinSize( wxSize( -1,22 ) );
    wxBoxSizer* bSizer53;
    bSizer53 = new wxBoxSizer( wxHORIZONTAL );


    bSizer53->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticTextStatusLeft = new wxStaticText( m_panel7, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextStatusLeft->Wrap( -1 );
    m_staticTextStatusLeft->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer53->Add( m_staticTextStatusLeft, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );


    bSizer53->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer451->Add( bSizer53, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    m_staticline9 = new wxStaticLine( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    bSizer451->Add( m_staticline9, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxEXPAND, 2 );


    bSizer451->Add( 26, 0, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticTextStatusMiddle = new wxStaticText( m_panel7, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextStatusMiddle->Wrap( -1 );
    m_staticTextStatusMiddle->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer451->Add( m_staticTextStatusMiddle, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );


    bSizer451->Add( 26, 0, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticline10 = new wxStaticLine( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    bSizer451->Add( m_staticline10, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxTOP, 2 );

    wxBoxSizer* bSizer52;
    bSizer52 = new wxBoxSizer( wxHORIZONTAL );


    bSizer52->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticTextStatusRight = new wxStaticText( m_panel7, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextStatusRight->Wrap( -1 );
    m_staticTextStatusRight->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer52->Add( m_staticTextStatusRight, 0, wxALIGN_CENTER_VERTICAL, 5 );

    wxBoxSizer* bSizer50;
    bSizer50 = new wxBoxSizer( wxHORIZONTAL );


    bSizer50->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap15 = new wxStaticBitmap( m_panel7, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 10,10 ), 0 );
    bSizer50->Add( m_bitmap15, 0, wxALIGN_BOTTOM, 2 );

    bSizer52->Add( bSizer50, 1, wxALIGN_BOTTOM|wxEXPAND, 5 );

    bSizer451->Add( bSizer52, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

    m_panel7->SetSizer( bSizer451 );
    m_panel7->Layout();
    bSizer451->Fit( m_panel7 );
    bSizer1->Add( m_panel7, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    this->SetSizer( bSizer1 );
    this->Layout();

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainDialogGenerated::OnClose ) );
    this->Connect( m_menuItem10->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnCompare ) );
    this->Connect( m_menuItem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnSync ) );
    this->Connect( m_menuItem14->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuSaveConfig ) );
    this->Connect( m_menuItem13->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLoadConfig ) );
    this->Connect( m_menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuQuit ) );
    this->Connect( m_menuItemGerman->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangGerman ) );
    this->Connect( m_menuItemEnglish->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangEnglish ) );
    this->Connect( m_menuItemFrench->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangFrench ) );
    this->Connect( m_menuItemItalian->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangItalian ) );
    this->Connect( m_menuItemPolish->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangPolish ) );
    this->Connect( m_menuItemDutch->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangDutch ) );
    this->Connect( m_menuItemPortuguese->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangPortuguese ) );
    this->Connect( m_menuItemJapanese->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangJapanese ) );
    this->Connect( m_menuItemChineseSimple->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangChineseSimp ) );
    this->Connect( m_menuItemGlobSett->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuGlobalSettings ) );
    this->Connect( m_menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuBatchJob ) );
    this->Connect( m_menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuExportFileList ) );
    this->Connect( m_menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuAbout ) );
    m_buttonCompare->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnCompare ), NULL, this );
    m_buttonAbort->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnAbortCompare ), NULL, this );
    m_radioBtnSizeDate->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnCompareByTimeSize ), NULL, this );
    m_radioBtnContent->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnCompareByContent ), NULL, this );
    m_bpButton14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnShowHelpDialog ), NULL, this );
    m_bpButtonFilter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnFilterButton ), NULL, this );
    m_hyperlinkCfgFilter->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( MainDialogGenerated::OnConfigureFilter ), NULL, this );
    m_checkBoxHideFilt->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnHideFilteredButton ), NULL, this );
    m_buttonSync->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnSync ), NULL, this );
    m_directoryLeft->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainDialogGenerated::OnWriteDirManually ), NULL, this );
    m_dirPickerLeft->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainDialogGenerated::OnDirSelected ), NULL, this );
    m_bpButtonSwap->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnSwapDirs ), NULL, this );
    m_bpButtonRemovePair->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnRemoveFolderPair ), NULL, this );
    m_bpButtonAddPair->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnAddFolderPair ), NULL, this );
    m_directoryRight->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainDialogGenerated::OnWriteDirManually ), NULL, this );
    m_dirPickerRight->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainDialogGenerated::OnDirSelected ), NULL, this );
    m_gridLeft->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( MainDialogGenerated::OnLeftGridDoubleClick ), NULL, this );
    m_gridLeft->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextMenu ), NULL, this );
    m_gridLeft->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( MainDialogGenerated::OnSortLeftGrid ), NULL, this );
    m_gridLeft->Connect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextColumnLeft ), NULL, this );
    m_gridMiddle->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextMenuMiddle ), NULL, this );
    m_gridMiddle->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( MainDialogGenerated::OnSortMiddleGrid ), NULL, this );
    m_gridRight->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( MainDialogGenerated::OnRightGridDoubleClick ), NULL, this );
    m_gridRight->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextMenu ), NULL, this );
    m_gridRight->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( MainDialogGenerated::OnSortRightGrid ), NULL, this );
    m_gridRight->Connect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextColumnRight ), NULL, this );
    m_bpButtonSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnSaveConfig ), NULL, this );
    m_bpButtonLoad->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnLoadConfig ), NULL, this );
    m_choiceHistory->Connect( wxEVT_CHAR, wxKeyEventHandler( MainDialogGenerated::OnChoiceKeyEvent ), NULL, this );
    m_choiceHistory->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnLoadFromHistory ), NULL, this );
    m_bpButtonLeftOnly->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnLeftOnlyFiles ), NULL, this );
    m_bpButtonLeftNewer->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnLeftNewerFiles ), NULL, this );
    m_bpButtonEqual->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnEqualFiles ), NULL, this );
    m_bpButtonDifferent->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnDifferentFiles ), NULL, this );
    m_bpButtonRightNewer->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnRightNewerFiles ), NULL, this );
    m_bpButtonRightOnly->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnRightOnlyFiles ), NULL, this );
    m_bpButton10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnQuit ), NULL, this );
}

MainDialogGenerated::~MainDialogGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainDialogGenerated::OnClose ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnCompare ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnSync ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuSaveConfig ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLoadConfig ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuQuit ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangGerman ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangEnglish ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangFrench ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangItalian ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangPolish ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangDutch ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangPortuguese ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangJapanese ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuLangChineseSimp ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuGlobalSettings ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuBatchJob ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuExportFileList ) );
    this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnMenuAbout ) );
    m_buttonCompare->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnCompare ), NULL, this );
    m_buttonAbort->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnAbortCompare ), NULL, this );
    m_radioBtnSizeDate->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnCompareByTimeSize ), NULL, this );
    m_radioBtnContent->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnCompareByContent ), NULL, this );
    m_bpButton14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnShowHelpDialog ), NULL, this );
    m_bpButtonFilter->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnFilterButton ), NULL, this );
    m_hyperlinkCfgFilter->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( MainDialogGenerated::OnConfigureFilter ), NULL, this );
    m_checkBoxHideFilt->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnHideFilteredButton ), NULL, this );
    m_buttonSync->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnSync ), NULL, this );
    m_directoryLeft->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainDialogGenerated::OnWriteDirManually ), NULL, this );
    m_dirPickerLeft->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainDialogGenerated::OnDirSelected ), NULL, this );
    m_bpButtonSwap->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnSwapDirs ), NULL, this );
    m_bpButtonRemovePair->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnRemoveFolderPair ), NULL, this );
    m_bpButtonAddPair->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnAddFolderPair ), NULL, this );
    m_directoryRight->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MainDialogGenerated::OnWriteDirManually ), NULL, this );
    m_dirPickerRight->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( MainDialogGenerated::OnDirSelected ), NULL, this );
    m_gridLeft->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( MainDialogGenerated::OnLeftGridDoubleClick ), NULL, this );
    m_gridLeft->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextMenu ), NULL, this );
    m_gridLeft->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( MainDialogGenerated::OnSortLeftGrid ), NULL, this );
    m_gridLeft->Disconnect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextColumnLeft ), NULL, this );
    m_gridMiddle->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextMenuMiddle ), NULL, this );
    m_gridMiddle->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( MainDialogGenerated::OnSortMiddleGrid ), NULL, this );
    m_gridRight->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( MainDialogGenerated::OnRightGridDoubleClick ), NULL, this );
    m_gridRight->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextMenu ), NULL, this );
    m_gridRight->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( MainDialogGenerated::OnSortRightGrid ), NULL, this );
    m_gridRight->Disconnect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( MainDialogGenerated::OnContextColumnRight ), NULL, this );
    m_bpButtonSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnSaveConfig ), NULL, this );
    m_bpButtonLoad->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnLoadConfig ), NULL, this );
    m_choiceHistory->Disconnect( wxEVT_CHAR, wxKeyEventHandler( MainDialogGenerated::OnChoiceKeyEvent ), NULL, this );
    m_choiceHistory->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainDialogGenerated::OnLoadFromHistory ), NULL, this );
    m_bpButtonLeftOnly->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnLeftOnlyFiles ), NULL, this );
    m_bpButtonLeftNewer->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnLeftNewerFiles ), NULL, this );
    m_bpButtonEqual->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnEqualFiles ), NULL, this );
    m_bpButtonDifferent->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnDifferentFiles ), NULL, this );
    m_bpButtonRightNewer->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnRightNewerFiles ), NULL, this );
    m_bpButtonRightOnly->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnRightOnlyFiles ), NULL, this );
    m_bpButton10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainDialogGenerated::OnQuit ), NULL, this );
}

FolderPairGenerated::FolderPairGenerated( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* bSizer74;
    bSizer74 = new wxBoxSizer( wxHORIZONTAL );

    m_panelLeft = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBoxSizer* sbSizer21;
    sbSizer21 = new wxStaticBoxSizer( new wxStaticBox( m_panelLeft, wxID_ANY, wxEmptyString ), wxHORIZONTAL );

    m_directoryLeft = new wxTextCtrl( m_panelLeft, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    sbSizer21->Add( m_directoryLeft, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_dirPickerLeft = new wxDirPickerCtrl( m_panelLeft, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST );
    sbSizer21->Add( m_dirPickerLeft, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_panelLeft->SetSizer( sbSizer21 );
    m_panelLeft->Layout();
    sbSizer21->Fit( m_panelLeft );
    bSizer74->Add( m_panelLeft, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

    m_panel20 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_panel20->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );

    wxBoxSizer* bSizer95;
    bSizer95 = new wxBoxSizer( wxHORIZONTAL );

    m_panel21 = new wxPanel( m_panel20, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_panel21->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

    wxBoxSizer* bSizer96;
    bSizer96 = new wxBoxSizer( wxVERTICAL );


    bSizer96->Add( 0, 15, 0, 0, 5 );

    m_bitmap23 = new wxStaticBitmap( m_panel21, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 45,17 ), 0 );
    m_bitmap23->SetToolTip( _("Folder pair") );

    bSizer96->Add( m_bitmap23, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    m_panel21->SetSizer( bSizer96 );
    m_panel21->Layout();
    bSizer96->Fit( m_panel21 );
    bSizer95->Add( m_panel21, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    m_panel20->SetSizer( bSizer95 );
    m_panel20->Layout();
    bSizer95->Fit( m_panel20 );
    bSizer74->Add( m_panel20, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    m_panelRight = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBoxSizer* sbSizer23;
    sbSizer23 = new wxStaticBoxSizer( new wxStaticBox( m_panelRight, wxID_ANY, wxEmptyString ), wxHORIZONTAL );

    m_directoryRight = new wxTextCtrl( m_panelRight, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    sbSizer23->Add( m_directoryRight, 1, wxALIGN_CENTER_VERTICAL, 5 );

    m_dirPickerRight = new wxDirPickerCtrl( m_panelRight, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST );
    sbSizer23->Add( m_dirPickerRight, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_panelRight->SetSizer( sbSizer23 );
    m_panelRight->Layout();
    sbSizer23->Fit( m_panelRight );
    bSizer74->Add( m_panelRight, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

    this->SetSizer( bSizer74 );
    this->Layout();
    bSizer74->Fit( this );
}

FolderPairGenerated::~FolderPairGenerated()
{
}

BatchDlgGenerated::BatchDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer54;
    bSizer54 = new wxBoxSizer( wxVERTICAL );

    bSizer69 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer87;
    bSizer87 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmap27 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), 0 );
    bSizer87->Add( m_bitmap27, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_panel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    m_panel8->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );

    wxBoxSizer* bSizer72;
    bSizer72 = new wxBoxSizer( wxVERTICAL );

    m_staticText56 = new wxStaticText( m_panel8, wxID_ANY, _("Batch job"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText56->Wrap( -1 );
    m_staticText56->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer72->Add( m_staticText56, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    m_panel8->SetSizer( bSizer72 );
    m_panel8->Layout();
    bSizer72->Fit( m_panel8 );
    bSizer87->Add( m_panel8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    bSizer87->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer69->Add( bSizer87, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


    bSizer69->Add( 0, 5, 0, 0, 5 );

    m_staticText54 = new wxStaticText( this, wxID_ANY, _("Assemble a batch file for automated synchronization. To start in batch mode simply pass the name of the file to the FreeFileSync executable: FreeFileSync.exe <batchfile>. This can also be scheduled in your operating system's task planner."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText54->Wrap( 520 );
    m_staticText54->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Tahoma") ) );

    bSizer69->Add( m_staticText54, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


    bSizer69->Add( 0, 5, 0, 0, 5 );

    m_staticline10 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    bSizer69->Add( m_staticline10, 0, wxEXPAND|wxTOP, 5 );

    m_staticText531 = new wxStaticText( this, wxID_ANY, _("Configuration overview:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText531->Wrap( -1 );
    m_staticText531->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Arial Black") ) );

    bSizer69->Add( m_staticText531, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    wxBoxSizer* bSizer67;
    bSizer67 = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* bSizer100;
    bSizer100 = new wxBoxSizer( wxVERTICAL );

    m_scrolledWindow6 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
    m_scrolledWindow6->SetScrollRate( 5, 5 );
    bSizerFolderPairs = new wxBoxSizer( wxVERTICAL );

    m_scrolledWindow6->SetSizer( bSizerFolderPairs );
    m_scrolledWindow6->Layout();
    bSizerFolderPairs->Fit( m_scrolledWindow6 );
    bSizer100->Add( m_scrolledWindow6, 0, wxEXPAND, 5 );


    bSizer100->Add( 0, 10, 0, 0, 5 );

    wxBoxSizer* bSizer57;
    bSizer57 = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* bSizer71;
    bSizer71 = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* bSizer721;
    bSizer721 = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sbSizer6;
    sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Compare by...") ), wxVERTICAL );

    m_radioBtnSizeDate = new wxRadioButton( this, wxID_ANY, _("File size and date"), wxDefaultPosition, wxDefaultSize, 0 );
    m_radioBtnSizeDate->SetValue( true );
    m_radioBtnSizeDate->SetToolTip( _("Files are found equal if\n     - filesize\n     - last write time and date\nare the same.") );

    sbSizer6->Add( m_radioBtnSizeDate, 0, 0, 5 );

    m_radioBtnContent = new wxRadioButton( this, wxID_ANY, _("File content"), wxDefaultPosition, wxDefaultSize, 0 );
    m_radioBtnContent->SetToolTip( _("Files are found equal if\n     - file content\nis the same.") );

    sbSizer6->Add( m_radioBtnContent, 0, wxTOP, 5 );

    bSizer721->Add( sbSizer6, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


    bSizer721->Add( 0, 10, 0, 0, 5 );

    wxStaticBoxSizer* sbSizer25;
    sbSizer25 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Error handling") ), wxVERTICAL );

    wxArrayString m_choiceHandleErrorChoices;
    m_choiceHandleError = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceHandleErrorChoices, 0 );
    m_choiceHandleError->SetSelection( 0 );
    sbSizer25->Add( m_choiceHandleError, 0, wxALL, 5 );

    bSizer721->Add( sbSizer25, 1, wxEXPAND, 5 );


    bSizer721->Add( 0, 10, 0, 0, 5 );

    wxStaticBoxSizer* sbSizer24;
    sbSizer24 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );

    m_checkBoxUseRecycler = new wxCheckBox( this, wxID_ANY, _("Use Recycle Bin"), wxDefaultPosition, wxDefaultSize, 0 );

    m_checkBoxUseRecycler->SetToolTip( _("Use Recycle Bin when deleting or overwriting files during synchronization") );

    sbSizer24->Add( m_checkBoxUseRecycler, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_checkBoxSilent = new wxCheckBox( this, wxID_ANY, _("Silent mode"), wxDefaultPosition, wxDefaultSize, 0 );

    m_checkBoxSilent->SetToolTip( _("Do not show graphical status and error messages but write to a logfile instead") );

    sbSizer24->Add( m_checkBoxSilent, 0, wxALL, 5 );

    bSizer721->Add( sbSizer24, 0, wxEXPAND, 5 );

    bSizer71->Add( bSizer721, 0, 0, 5 );

    bSizer57->Add( bSizer71, 0, wxEXPAND, 5 );


    bSizer57->Add( 10, 0, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* sbSizer8;
    sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Filter files") ), wxHORIZONTAL );

    m_bpButtonFilter = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW|wxFULL_REPAINT_ON_RESIZE );
    sbSizer8->Add( m_bpButtonFilter, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    wxBoxSizer* bSizer101;
    bSizer101 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer671;
    bSizer671 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmap8 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), 0 );
    m_bitmap8->SetToolTip( _("Include") );

    bSizer671->Add( m_bitmap8, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_textCtrlInclude = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 250,-1 ), wxTE_MULTILINE );
    bSizer671->Add( m_textCtrlInclude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer101->Add( bSizer671, 0, 0, 5 );


    bSizer101->Add( 0, 10, 0, 0, 5 );

    wxBoxSizer* bSizer691;
    bSizer691 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmap9 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), 0 );
    m_bitmap9->SetToolTip( _("Exclude") );

    bSizer691->Add( m_bitmap9, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_textCtrlExclude = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 250,-1 ), wxTE_MULTILINE );
    bSizer691->Add( m_textCtrlExclude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer101->Add( bSizer691, 0, 0, 5 );

    sbSizer8->Add( bSizer101, 1, wxEXPAND, 5 );

    bSizer57->Add( sbSizer8, 0, 0, 5 );

    bSizer100->Add( bSizer57, 0, 0, 5 );

    bSizer67->Add( bSizer100, 0, 0, 5 );


    bSizer67->Add( 10, 0, 0, wxALIGN_CENTER_VERTICAL, 5 );

    wxStaticBoxSizer* sbSizer61;
    sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Configuration") ), wxVERTICAL );

    wxGridSizer* gSizer3;
    gSizer3 = new wxGridSizer( 1, 2, 0, 5 );

    m_staticText211 = new wxStaticText( this, wxID_ANY, _("Result"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText211->Wrap( -1 );
    m_staticText211->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    gSizer3->Add( m_staticText211, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText311 = new wxStaticText( this, wxID_ANY, _("Action"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText311->Wrap( -1 );
    m_staticText311->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    gSizer3->Add( m_staticText311, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer61->Add( gSizer3, 0, wxEXPAND, 5 );

    m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    sbSizer61->Add( m_staticline3, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP|wxBOTTOM, 5 );

    wxGridSizer* gSizer1;
    gSizer1 = new wxGridSizer( 5, 2, 0, 5 );

    m_bitmap13 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap13->SetToolTip( _("Files/folders that exist on left side only") );

    gSizer1->Add( m_bitmap13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton5 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap14 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap14->SetToolTip( _("Files/folders that exist on right side only") );

    gSizer1->Add( m_bitmap14, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton6 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton6, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap15 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap15->SetToolTip( _("Files that exist on both sides, left one is newer") );

    gSizer1->Add( m_bitmap15, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton7 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap16 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap16->SetToolTip( _("Files that exist on both sides, right one is newer") );

    gSizer1->Add( m_bitmap16, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton8 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap17 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap17->SetToolTip( _("dummy") );

    gSizer1->Add( m_bitmap17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton9 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer61->Add( gSizer1, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    bSizer67->Add( sbSizer61, 0, 0, 5 );

    bSizer69->Add( bSizer67, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    m_staticline9 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    bSizer69->Add( m_staticline9, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    wxBoxSizer* bSizer68;
    bSizer68 = new wxBoxSizer( wxHORIZONTAL );

    m_buttonSave = new wxButton( this, wxID_SAVE, _("&Save"), wxDefaultPosition, wxSize( 120,35 ), 0 );
    m_buttonSave->SetDefault();
    m_buttonSave->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer68->Add( m_buttonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_buttonLoad = new wxButton( this, wxID_OPEN, _("&Load"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_buttonLoad->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer68->Add( m_buttonLoad, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_button6 = new wxButton( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button6->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer68->Add( m_button6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer69->Add( bSizer68, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    bSizer54->Add( bSizer69, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    this->SetSizer( bSizer54 );
    this->Layout();
    bSizer54->Fit( this );

    this->Centre( wxBOTH );

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( BatchDlgGenerated::OnClose ) );
    m_radioBtnSizeDate->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BatchDlgGenerated::OnChangeCompareVar ), NULL, this );
    m_radioBtnContent->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BatchDlgGenerated::OnChangeCompareVar ), NULL, this );
    m_choiceHandleError->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BatchDlgGenerated::OnChangeErrorHandling ), NULL, this );
    m_checkBoxUseRecycler->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnSelectRecycleBin ), NULL, this );
    m_bpButtonFilter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnFilterButton ), NULL, this );
    m_bpButton5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnExLeftSideOnly ), NULL, this );
    m_bpButton6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnExRightSideOnly ), NULL, this );
    m_bpButton7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnLeftNewer ), NULL, this );
    m_bpButton8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnRightNewer ), NULL, this );
    m_bpButton9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnDifferent ), NULL, this );
    m_buttonSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnSaveBatchJob ), NULL, this );
    m_buttonLoad->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnLoadBatchJob ), NULL, this );
    m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnCancel ), NULL, this );
}

BatchDlgGenerated::~BatchDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( BatchDlgGenerated::OnClose ) );
    m_radioBtnSizeDate->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BatchDlgGenerated::OnChangeCompareVar ), NULL, this );
    m_radioBtnContent->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BatchDlgGenerated::OnChangeCompareVar ), NULL, this );
    m_choiceHandleError->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BatchDlgGenerated::OnChangeErrorHandling ), NULL, this );
    m_checkBoxUseRecycler->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnSelectRecycleBin ), NULL, this );
    m_bpButtonFilter->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnFilterButton ), NULL, this );
    m_bpButton5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnExLeftSideOnly ), NULL, this );
    m_bpButton6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnExRightSideOnly ), NULL, this );
    m_bpButton7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnLeftNewer ), NULL, this );
    m_bpButton8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnRightNewer ), NULL, this );
    m_bpButton9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnDifferent ), NULL, this );
    m_buttonSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnSaveBatchJob ), NULL, this );
    m_buttonLoad->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnLoadBatchJob ), NULL, this );
    m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchDlgGenerated::OnCancel ), NULL, this );
}

BatchFolderPairGenerated::BatchFolderPairGenerated( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxStaticBoxSizer* sbSizer20;
    sbSizer20 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );

    wxFlexGridSizer* fgSizer9;
    fgSizer9 = new wxFlexGridSizer( 2, 2, 5, 5 );
    fgSizer9->AddGrowableCol( 1 );
    fgSizer9->SetFlexibleDirection( wxHORIZONTAL );
    fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_staticText53 = new wxStaticText( this, wxID_ANY, _("Left folder:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText53->Wrap( -1 );
    m_staticText53->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer9->Add( m_staticText53, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    m_directoryLeft = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer9->Add( m_directoryLeft, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    m_staticText541 = new wxStaticText( this, wxID_ANY, _("Right folder:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText541->Wrap( -1 );
    m_staticText541->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer9->Add( m_staticText541, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    m_directoryRight = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer9->Add( m_directoryRight, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    sbSizer20->Add( fgSizer9, 0, wxEXPAND, 5 );

    this->SetSizer( sbSizer20 );
    this->Layout();
    sbSizer20->Fit( this );

    // Connect Events
    m_directoryLeft->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( BatchFolderPairGenerated::OnEnterLeftDir ), NULL, this );
    m_directoryRight->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( BatchFolderPairGenerated::OnEnterRightDir ), NULL, this );
}

BatchFolderPairGenerated::~BatchFolderPairGenerated()
{
    // Disconnect Events
    m_directoryLeft->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( BatchFolderPairGenerated::OnEnterLeftDir ), NULL, this );
    m_directoryRight->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( BatchFolderPairGenerated::OnEnterRightDir ), NULL, this );
}

CompareStatusGenerated::CompareStatusGenerated( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* bSizer40;
    bSizer40 = new wxBoxSizer( wxVERTICAL );

    bSizer42 = new wxBoxSizer( wxHORIZONTAL );

    wxStaticBoxSizer* sbSizer10;
    sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxHORIZONTAL );

    m_staticText321 = new wxStaticText( this, wxID_ANY, _("Files/folders scanned:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText321->Wrap( -1 );
    m_staticText321->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    sbSizer10->Add( m_staticText321, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxRIGHT, 5 );

    m_staticTextScanned = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextScanned->Wrap( -1 );
    m_staticTextScanned->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    sbSizer10->Add( m_staticTextScanned, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer42->Add( sbSizer10, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


    bSizer42->Add( 0, 0, 1, wxEXPAND, 5 );

    sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Comparing content") ), wxVERTICAL );

    wxFlexGridSizer* fgSizer8;
    fgSizer8 = new wxFlexGridSizer( 2, 2, 3, 0 );
    fgSizer8->SetFlexibleDirection( wxBOTH );
    fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_staticText46 = new wxStaticText( this, wxID_ANY, _("Files remaining:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText46->Wrap( -1 );
    m_staticText46->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    fgSizer8->Add( m_staticText46, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_staticTextFilesToCompare = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextFilesToCompare->Wrap( -1 );
    m_staticTextFilesToCompare->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer8->Add( m_staticTextFilesToCompare, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText32 = new wxStaticText( this, wxID_ANY, _("Data remaining:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText32->Wrap( -1 );
    m_staticText32->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    fgSizer8->Add( m_staticText32, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_staticTextDataToCompare = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextDataToCompare->Wrap( -1 );
    m_staticTextDataToCompare->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer8->Add( m_staticTextDataToCompare, 0, wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer13->Add( fgSizer8, 0, 0, 5 );

    bSizer42->Add( sbSizer13, 0, wxALIGN_CENTER_VERTICAL, 5 );


    bSizer42->Add( 0, 0, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* sbSizer131;
    sbSizer131 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxHORIZONTAL );

    m_staticText37 = new wxStaticText( this, wxID_ANY, _("Time elapsed:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText37->Wrap( -1 );
    m_staticText37->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    sbSizer131->Add( m_staticText37, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_staticTextTimeElapsed = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextTimeElapsed->Wrap( -1 );
    m_staticTextTimeElapsed->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    sbSizer131->Add( m_staticTextTimeElapsed, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer42->Add( sbSizer131, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    bSizer40->Add( bSizer42, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 5 );

    wxBoxSizer* bSizer48;
    bSizer48 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText30 = new wxStaticText( this, wxID_ANY, _("Operation:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText30->Wrap( -1 );
    m_staticText30->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer48->Add( m_staticText30, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    m_textCtrlFilename = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    m_textCtrlFilename->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );

    bSizer48->Add( m_textCtrlFilename, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer40->Add( bSizer48, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    m_gauge2 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,20 ), wxGA_HORIZONTAL );
    bSizer40->Add( m_gauge2, 0, wxALL|wxEXPAND, 5 );

    this->SetSizer( bSizer40 );
    this->Layout();
    bSizer40->Fit( this );
}

CompareStatusGenerated::~CompareStatusGenerated()
{
}

SyncDlgGenerated::SyncDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer7;
    bSizer7 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer181;
    bSizer181 = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* bSizer29;
    bSizer29 = new wxBoxSizer( wxVERTICAL );

    bSizer201 = new wxBoxSizer( wxHORIZONTAL );

    m_button18 = new wxButtonWithImage( this, wxID_OK, _("&Start"), wxDefaultPosition, wxSize( 140,58 ), 0 );
    m_button18->SetDefault();
    m_button18->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Arial Black") ) );
    m_button18->SetToolTip( _("Start synchronization") );

    bSizer201->Add( m_button18, 0, wxALIGN_CENTER_VERTICAL, 5 );


    bSizer201->Add( 18, 0, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer38;
    bSizer38 = new wxBoxSizer( wxVERTICAL );

    m_checkBoxUseRecycler = new wxCheckBox( this, wxID_ANY, _("Use Recycle Bin"), wxDefaultPosition, wxDefaultSize, 0 );

    m_checkBoxUseRecycler->SetToolTip( _("Use Recycle Bin when deleting or overwriting files during synchronization") );

    bSizer38->Add( m_checkBoxUseRecycler, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_checkBoxIgnoreErrors = new wxCheckBox( this, wxID_ANY, _("Ignore errors"), wxDefaultPosition, wxDefaultSize, 0 );

    m_checkBoxIgnoreErrors->SetToolTip( _("Hides error messages during synchronization:\nThey are collected and shown as a list at the end of the process") );

    bSizer38->Add( m_checkBoxIgnoreErrors, 0, wxALL, 5 );

    bSizer201->Add( bSizer38, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer29->Add( bSizer201, 1, 0, 5 );


    bSizer29->Add( 0, 5, 0, 0, 5 );

    wxStaticBoxSizer* sbSizer7;
    sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );

    m_staticText1 = new wxStaticText( this, wxID_ANY, _("Select variant:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText1->Wrap( -1 );
    m_staticText1->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    sbSizer7->Add( m_staticText1, 0, wxALL, 5 );

    wxFlexGridSizer* fgSizer1;
    fgSizer1 = new wxFlexGridSizer( 4, 3, 8, 5 );
    fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
    fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_radioBtn1 = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_radioBtn1->SetValue( true );
    m_radioBtn1->SetFont( wxFont( 11, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer1->Add( m_radioBtn1, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_buttonOneWay = new wxButton( this, wxID_ANY, _("Mirror ->>"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    m_buttonOneWay->SetFont( wxFont( 11, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer1->Add( m_buttonOneWay, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    m_staticText8 = new wxStaticText( this, wxID_ANY, _("Mirror backup of left folder: Right folder will be overwritten and exactly match left folder after synchronization."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText8->Wrap( 300 );
    fgSizer1->Add( m_staticText8, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

    m_radioBtnUpdate = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_radioBtnUpdate->SetFont( wxFont( 11, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer1->Add( m_radioBtnUpdate, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_buttonUpdate = new wxButton( this, wxID_ANY, _("Update ->"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    m_buttonUpdate->SetFont( wxFont( 11, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer1->Add( m_buttonUpdate, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    m_staticText101 = new wxStaticText( this, wxID_ANY, _("Copy new or updated files to right folder."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText101->Wrap( 300 );
    fgSizer1->Add( m_staticText101, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    m_radioBtn2 = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_radioBtn2->SetFont( wxFont( 11, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer1->Add( m_radioBtn2, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_buttonTwoWay = new wxButton( this, wxID_ANY, _("Two way <->"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    m_buttonTwoWay->SetFont( wxFont( 11, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer1->Add( m_buttonTwoWay, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText10 = new wxStaticText( this, wxID_ANY, _("Synchronize both sides simultaneously: Copy new or updated files in both directions."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText10->Wrap( 300 );
    fgSizer1->Add( m_staticText10, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    m_radioBtn3 = new wxRadioButton( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_radioBtn3->SetFont( wxFont( 11, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer1->Add( m_radioBtn3, 0, wxALIGN_CENTER_VERTICAL, 5 );

    wxBoxSizer* bSizer65;
    bSizer65 = new wxBoxSizer( wxVERTICAL );


    bSizer65->Add( 0, 0, 1, wxEXPAND, 5 );

    m_staticText23 = new wxStaticText( this, wxID_ANY, _("Custom"), wxDefaultPosition, wxSize( -1,-1 ), wxALIGN_CENTRE|wxSTATIC_BORDER );
    m_staticText23->Wrap( -1 );
    m_staticText23->SetFont( wxFont( 11, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer65->Add( m_staticText23, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


    bSizer65->Add( 0, 0, 1, wxEXPAND, 5 );

    fgSizer1->Add( bSizer65, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    m_staticText9 = new wxStaticText( this, wxID_ANY, _("Configure your own synchronization rules."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText9->Wrap( 300 );
    fgSizer1->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    sbSizer7->Add( fgSizer1, 0, 0, 5 );

    bSizer29->Add( sbSizer7, 0, wxEXPAND, 5 );


    bSizer29->Add( 0, 5, 0, 0, 5 );

    wxBoxSizer* bSizer291;
    bSizer291 = new wxBoxSizer( wxHORIZONTAL );

    m_button6 = new wxButton( this, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxSize( 100,32 ), 0 );
    m_button6->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer291->Add( m_button6, 0, wxALIGN_BOTTOM, 5 );

    m_button16 = new wxButton( this, wxID_CANCEL, _("dummy"), wxDefaultPosition, wxSize( 0,0 ), 0 );
    bSizer291->Add( m_button16, 0, wxALIGN_BOTTOM, 5 );


    bSizer291->Add( 20, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    wxStaticBoxSizer* sbSizer16;
    sbSizer16 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Preview") ), wxHORIZONTAL );

    wxFlexGridSizer* fgSizer5;
    fgSizer5 = new wxFlexGridSizer( 2, 2, 0, 5 );
    fgSizer5->SetFlexibleDirection( wxHORIZONTAL );
    fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_staticText37 = new wxStaticText( this, wxID_ANY, _("Create:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText37->Wrap( -1 );
    m_staticText37->SetFont( wxFont( 10, 74, 93, 90, false, wxT("Tahoma") ) );
    m_staticText37->SetToolTip( _("Number of files and directories that will be created") );

    fgSizer5->Add( m_staticText37, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_textCtrlCreate = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), wxTE_READONLY );
    m_textCtrlCreate->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Tahoma") ) );
    m_textCtrlCreate->SetBackgroundColour( wxColour( 222, 222, 236 ) );
    m_textCtrlCreate->SetToolTip( _("Number of files and directories that will be created") );

    fgSizer5->Add( m_textCtrlCreate, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText14 = new wxStaticText( this, wxID_ANY, _("Delete:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText14->Wrap( -1 );
    m_staticText14->SetFont( wxFont( 10, 74, 93, 90, false, wxT("Tahoma") ) );
    m_staticText14->SetToolTip( _("Number of files and directories that will be deleted") );

    fgSizer5->Add( m_staticText14, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_textCtrlDelete = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), wxTE_READONLY );
    m_textCtrlDelete->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Tahoma") ) );
    m_textCtrlDelete->SetBackgroundColour( wxColour( 222, 222, 236 ) );
    m_textCtrlDelete->SetToolTip( _("Number of files and directories that will be deleted") );

    fgSizer5->Add( m_textCtrlDelete, 0, wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer16->Add( fgSizer5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    wxFlexGridSizer* fgSizer6;
    fgSizer6 = new wxFlexGridSizer( 2, 2, 0, 5 );
    fgSizer6->SetFlexibleDirection( wxHORIZONTAL );
    fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_staticText42 = new wxStaticText( this, wxID_ANY, _("Update:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText42->Wrap( -1 );
    m_staticText42->SetFont( wxFont( 10, 74, 93, 90, false, wxT("Tahoma") ) );
    m_staticText42->SetToolTip( _("Number of files that will be overwritten") );

    fgSizer6->Add( m_staticText42, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_textCtrlUpdate = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), wxTE_READONLY );
    m_textCtrlUpdate->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Tahoma") ) );
    m_textCtrlUpdate->SetBackgroundColour( wxColour( 222, 222, 236 ) );
    m_textCtrlUpdate->SetToolTip( _("Number of files that will be overwritten") );

    fgSizer6->Add( m_textCtrlUpdate, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText43 = new wxStaticText( this, wxID_ANY, _("Data:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText43->Wrap( -1 );
    m_staticText43->SetFont( wxFont( 10, 74, 93, 90, false, wxT("Tahoma") ) );
    m_staticText43->SetToolTip( _("Total amount of data that will be transferred") );

    fgSizer6->Add( m_staticText43, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_textCtrlData = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), wxTE_READONLY );
    m_textCtrlData->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Tahoma") ) );
    m_textCtrlData->SetBackgroundColour( wxColour( 222, 222, 236 ) );
    m_textCtrlData->SetToolTip( _("Total amount of data that will be transferred") );

    fgSizer6->Add( m_textCtrlData, 0, wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer16->Add( fgSizer6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    bSizer291->Add( sbSizer16, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    bSizer29->Add( bSizer291, 0, wxEXPAND, 5 );

    bSizer181->Add( bSizer29, 0, 0, 5 );


    bSizer181->Add( 10, 0, 0, 0, 5 );

    wxStaticBoxSizer* sbSizer6;
    sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Configuration") ), wxVERTICAL );

    wxGridSizer* gSizer3;
    gSizer3 = new wxGridSizer( 1, 2, 0, 5 );

    m_staticText21 = new wxStaticText( this, wxID_ANY, _("Result"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText21->Wrap( -1 );
    m_staticText21->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    gSizer3->Add( m_staticText21, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText31 = new wxStaticText( this, wxID_ANY, _("Action"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText31->Wrap( -1 );
    m_staticText31->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    gSizer3->Add( m_staticText31, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer6->Add( gSizer3, 0, wxEXPAND, 5 );

    m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    sbSizer6->Add( m_staticline3, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP|wxBOTTOM, 5 );

    wxGridSizer* gSizer1;
    gSizer1 = new wxGridSizer( 5, 2, 0, 5 );

    m_bitmap13 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap13->SetToolTip( _("Files/folders that exist on left side only") );

    gSizer1->Add( m_bitmap13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton5 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap14 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap14->SetToolTip( _("Files/folders that exist on right side only") );

    gSizer1->Add( m_bitmap14, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton6 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton6, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap15 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap15->SetToolTip( _("Files that exist on both sides, left one is newer") );

    gSizer1->Add( m_bitmap15, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton7 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap16 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap16->SetToolTip( _("Files that exist on both sides, right one is newer") );

    gSizer1->Add( m_bitmap16, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton8 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap17 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    m_bitmap17->SetToolTip( _("dummy") );

    gSizer1->Add( m_bitmap17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButton9 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 42,42 ), wxBU_AUTODRAW );
    gSizer1->Add( m_bpButton9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer6->Add( gSizer1, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    bSizer181->Add( sbSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_BOTTOM, 5 );

    bSizer7->Add( bSizer181, 0, wxALL, 5 );

    this->SetSizer( bSizer7 );
    this->Layout();
    bSizer7->Fit( this );

    this->Centre( wxBOTH );

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SyncDlgGenerated::OnClose ) );
    m_button18->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnStartSync ), NULL, this );
    m_checkBoxUseRecycler->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnSelectRecycleBin ), NULL, this );
    m_radioBtn1->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SyncDlgGenerated::OnSyncLeftToRight ), NULL, this );
    m_buttonOneWay->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnSyncLeftToRight ), NULL, this );
    m_radioBtnUpdate->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SyncDlgGenerated::OnSyncUpdate ), NULL, this );
    m_buttonUpdate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnSyncUpdate ), NULL, this );
    m_radioBtn2->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SyncDlgGenerated::OnSyncBothSides ), NULL, this );
    m_buttonTwoWay->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnSyncBothSides ), NULL, this );
    m_radioBtn3->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SyncDlgGenerated::OnSyncCostum ), NULL, this );
    m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnBack ), NULL, this );
    m_button16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnCancel ), NULL, this );
    m_bpButton5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnExLeftSideOnly ), NULL, this );
    m_bpButton6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnExRightSideOnly ), NULL, this );
    m_bpButton7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnLeftNewer ), NULL, this );
    m_bpButton8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnRightNewer ), NULL, this );
    m_bpButton9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnDifferent ), NULL, this );
}

SyncDlgGenerated::~SyncDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SyncDlgGenerated::OnClose ) );
    m_button18->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnStartSync ), NULL, this );
    m_checkBoxUseRecycler->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnSelectRecycleBin ), NULL, this );
    m_radioBtn1->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SyncDlgGenerated::OnSyncLeftToRight ), NULL, this );
    m_buttonOneWay->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnSyncLeftToRight ), NULL, this );
    m_radioBtnUpdate->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SyncDlgGenerated::OnSyncUpdate ), NULL, this );
    m_buttonUpdate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnSyncUpdate ), NULL, this );
    m_radioBtn2->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SyncDlgGenerated::OnSyncBothSides ), NULL, this );
    m_buttonTwoWay->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnSyncBothSides ), NULL, this );
    m_radioBtn3->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SyncDlgGenerated::OnSyncCostum ), NULL, this );
    m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnBack ), NULL, this );
    m_button16->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnCancel ), NULL, this );
    m_bpButton5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnExLeftSideOnly ), NULL, this );
    m_bpButton6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnExRightSideOnly ), NULL, this );
    m_bpButton7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnLeftNewer ), NULL, this );
    m_bpButton8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnRightNewer ), NULL, this );
    m_bpButton9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncDlgGenerated::OnDifferent ), NULL, this );
}

SyncStatusDlgGenerated::SyncStatusDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer27;
    bSizer27 = new wxBoxSizer( wxVERTICAL );


    bSizer27->Add( 0, 15, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer37;
    bSizer37 = new wxBoxSizer( wxHORIZONTAL );

    m_animationControl1 = new wxAnimationCtrl(this, wxID_ANY, wxNullAnimation, wxDefaultPosition, wxSize( 45,45 ));
    bSizer37->Add( m_animationControl1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_panel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    m_panel8->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );

    wxBoxSizer* bSizer72;
    bSizer72 = new wxBoxSizer( wxVERTICAL );

    m_staticText56 = new wxStaticText( m_panel8, wxID_ANY, _("Synchronization status"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText56->Wrap( -1 );
    m_staticText56->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer72->Add( m_staticText56, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    m_panel8->SetSizer( bSizer72 );
    m_panel8->Layout();
    bSizer72->Fit( m_panel8 );
    bSizer37->Add( m_panel8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer27->Add( bSizer37, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    wxBoxSizer* bSizer42;
    bSizer42 = new wxBoxSizer( wxHORIZONTAL );


    bSizer42->Add( 0, 0, 1, wxEXPAND, 5 );

    m_bitmapStatus = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 28,28 ), 0 );
    bSizer42->Add( m_bitmapStatus, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_staticTextStatus = new wxStaticText( this, wxID_ANY, _("Synchronizing..."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextStatus->Wrap( -1 );
    m_staticTextStatus->SetFont( wxFont( 14, 74, 93, 90, false, wxT("Tahoma") ) );

    bSizer42->Add( m_staticTextStatus, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );


    bSizer42->Add( 0, 0, 1, wxEXPAND, 5 );

    bSizer27->Add( bSizer42, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

    bSizer31 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText21 = new wxStaticText( this, wxID_ANY, _("Current operation:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText21->Wrap( -1 );
    m_staticText21->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer31->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


    bSizer31->Add( 0, 0, 1, wxEXPAND, 5 );

    m_staticText55 = new wxStaticText( this, wxID_ANY, _("Time elapsed:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText55->Wrap( -1 );
    m_staticText55->SetFont( wxFont( 10, 74, 93, 90, false, wxT("Tahoma") ) );

    bSizer31->Add( m_staticText55, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );

    m_staticTextTimeElapsed = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextTimeElapsed->Wrap( -1 );
    m_staticTextTimeElapsed->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer31->Add( m_staticTextTimeElapsed, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer27->Add( bSizer31, 0, wxEXPAND, 5 );

    m_textCtrlInfo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
    m_textCtrlInfo->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );

    bSizer27->Add( m_textCtrlInfo, 3, wxALL|wxEXPAND, 5 );

    m_gauge1 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,20 ), wxGA_HORIZONTAL );
    bSizer27->Add( m_gauge1, 0, wxALL|wxEXPAND, 5 );

    bSizer28 = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* bSizer33;
    bSizer33 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText25 = new wxStaticText( this, wxID_ANY, _("Files/folders remaining:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText25->Wrap( -1 );
    m_staticText25->SetFont( wxFont( 10, 74, 93, 90, false, wxT("Tahoma") ) );

    bSizer33->Add( m_staticText25, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_staticTextRemainingObj = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
    m_staticTextRemainingObj->Wrap( -1 );
    m_staticTextRemainingObj->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer33->Add( m_staticTextRemainingObj, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer28->Add( bSizer33, 0, wxALIGN_CENTER_VERTICAL, 5 );


    bSizer28->Add( 0, 0, 1, 0, 5 );

    m_buttonOK = new wxButton( this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize( 100,32 ), 0 );
    m_buttonOK->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );
    m_buttonOK->Hide();

    bSizer28->Add( m_buttonOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_buttonPause = new wxButton( this, wxID_ANY, _("&Pause"), wxDefaultPosition, wxSize( 100,32 ), 0 );
    m_buttonPause->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer28->Add( m_buttonPause, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_buttonAbort = new wxButton( this, wxID_CANCEL, _("&Abort"), wxDefaultPosition, wxSize( 100,32 ), 0 );
    m_buttonAbort->SetDefault();
    m_buttonAbort->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer28->Add( m_buttonAbort, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    bSizer28->Add( 0, 0, 1, 0, 5 );

    wxBoxSizer* bSizer32;
    bSizer32 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText26 = new wxStaticText( this, wxID_ANY, _("Data remaining:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText26->Wrap( -1 );
    m_staticText26->SetFont( wxFont( 10, 74, 93, 90, false, wxT("Tahoma") ) );

    bSizer32->Add( m_staticText26, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_staticTextDataRemaining = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextDataRemaining->Wrap( -1 );
    m_staticTextDataRemaining->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer32->Add( m_staticTextDataRemaining, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    bSizer28->Add( bSizer32, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer27->Add( bSizer28, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );


    bSizer27->Add( 0, 5, 0, wxEXPAND, 5 );

    this->SetSizer( bSizer27 );
    this->Layout();

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SyncStatusDlgGenerated::OnClose ) );
    m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncStatusDlgGenerated::OnOkay ), NULL, this );
    m_buttonPause->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncStatusDlgGenerated::OnPause ), NULL, this );
    m_buttonAbort->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncStatusDlgGenerated::OnAbort ), NULL, this );
}

SyncStatusDlgGenerated::~SyncStatusDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SyncStatusDlgGenerated::OnClose ) );
    m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncStatusDlgGenerated::OnOkay ), NULL, this );
    m_buttonPause->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncStatusDlgGenerated::OnPause ), NULL, this );
    m_buttonAbort->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SyncStatusDlgGenerated::OnAbort ), NULL, this );
}

HelpDlgGenerated::HelpDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer20;
    bSizer20 = new wxBoxSizer( wxVERTICAL );


    bSizer20->Add( 0, 10, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer85;
    bSizer85 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmap25 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), 0 );
    bSizer85->Add( m_bitmap25, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_panel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    m_panel8->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );

    wxBoxSizer* bSizer72;
    bSizer72 = new wxBoxSizer( wxHORIZONTAL );


    bSizer72->Add( 20, 0, 0, 0, 5 );

    m_staticText56 = new wxStaticText( m_panel8, wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText56->Wrap( -1 );
    m_staticText56->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer72->Add( m_staticText56, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 5 );


    bSizer72->Add( 20, 0, 0, 0, 5 );

    m_panel8->SetSizer( bSizer72 );
    m_panel8->Layout();
    bSizer72->Fit( m_panel8 );
    bSizer85->Add( m_panel8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    bSizer85->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer20->Add( bSizer85, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
    m_scrolledWindow1 = new wxScrolledWindow( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxSIMPLE_BORDER|wxVSCROLL );
    m_scrolledWindow1->SetScrollRate( 5, 5 );
    m_scrolledWindow1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVEBORDER ) );

    wxBoxSizer* bSizer70;
    bSizer70 = new wxBoxSizer( wxVERTICAL );

    m_staticText59 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("Compare by \"File size and date\""), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText59->Wrap( 500 );
    m_staticText59->SetFont( wxFont( 10, 74, 90, 92, true, wxT("Tahoma") ) );

    bSizer70->Add( m_staticText59, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

    m_staticText60 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("This variant evaluates two equally named files as being equal when they have the same file size AND the same last write date and time. Notice that the file time is allowed to deviate by up to 2 seconds. This ensures synchronization with the lower-precision file system FAT32 works correctly."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText60->Wrap( 500 );
    bSizer70->Add( m_staticText60, 0, wxALL, 5 );

    m_staticText61 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("When \"Compare\" is triggered with this option set the following decision tree is processed:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText61->Wrap( 500 );
    bSizer70->Add( m_staticText61, 0, wxALL, 5 );

    m_treeCtrl1 = new wxTreeCtrl( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxSize( -1,180 ), wxTR_DEFAULT_STYLE );
    m_treeCtrl1->SetBackgroundColour( wxColour( 237, 236, 235 ) );

    bSizer70->Add( m_treeCtrl1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    m_staticText63 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("As a result the files are separated into the following categories:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText63->Wrap( 500 );
    bSizer70->Add( m_staticText63, 0, wxALL, 5 );

    m_staticText75 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("- equal"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText75->Wrap( -1 );
    bSizer70->Add( m_staticText75, 0, wxRIGHT|wxLEFT, 5 );

    m_staticText76 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("- left newer"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText76->Wrap( -1 );
    bSizer70->Add( m_staticText76, 0, wxRIGHT|wxLEFT, 5 );

    m_staticText77 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("- right newer"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText77->Wrap( -1 );
    bSizer70->Add( m_staticText77, 0, wxRIGHT|wxLEFT, 5 );

    m_staticText78 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("- different (same date, different size)"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText78->Wrap( -1 );
    bSizer70->Add( m_staticText78, 0, wxRIGHT|wxLEFT, 5 );

    m_staticText79 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("- exists left only"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText79->Wrap( -1 );
    bSizer70->Add( m_staticText79, 0, wxRIGHT|wxLEFT, 5 );

    m_staticText80 = new wxStaticText( m_scrolledWindow1, wxID_ANY, _("- exists right only"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText80->Wrap( -1 );
    bSizer70->Add( m_staticText80, 0, wxRIGHT|wxLEFT, 5 );

    m_scrolledWindow1->SetSizer( bSizer70 );
    m_scrolledWindow1->Layout();
    bSizer70->Fit( m_scrolledWindow1 );
    m_notebook1->AddPage( m_scrolledWindow1, _("File size and date"), true );
    m_scrolledWindow5 = new wxScrolledWindow( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
    m_scrolledWindow5->SetScrollRate( 5, 5 );
    m_scrolledWindow5->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVEBORDER ) );

    wxBoxSizer* bSizer74;
    bSizer74 = new wxBoxSizer( wxVERTICAL );

    m_staticText65 = new wxStaticText( m_scrolledWindow5, wxID_ANY, _("Compare by \"File content\""), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText65->Wrap( 500 );
    m_staticText65->SetFont( wxFont( 10, 74, 90, 92, true, wxT("Tahoma") ) );

    bSizer74->Add( m_staticText65, 0, wxALL, 5 );

    m_staticText66 = new wxStaticText( m_scrolledWindow5, wxID_ANY, _("As the name suggests, two files which share the same name are marked as equal if and only if they have the same content. This option is useful for consistency checks rather than backup operations. Therefore the file times are not taken into account at all.\n\nWith this option enabled the decision tree is smaller:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText66->Wrap( 500 );
    bSizer74->Add( m_staticText66, 0, wxALL, 5 );

    m_treeCtrl2 = new wxTreeCtrl( m_scrolledWindow5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
    m_treeCtrl2->SetBackgroundColour( wxColour( 237, 236, 235 ) );
    m_treeCtrl2->SetMinSize( wxSize( -1,130 ) );

    bSizer74->Add( m_treeCtrl2, 0, wxALL|wxEXPAND, 5 );

    m_staticText69 = new wxStaticText( m_scrolledWindow5, wxID_ANY, _("As a result the files are separated into the following categories:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText69->Wrap( 500 );
    bSizer74->Add( m_staticText69, 0, wxALL, 5 );

    m_staticText81 = new wxStaticText( m_scrolledWindow5, wxID_ANY, _("- equal"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText81->Wrap( -1 );
    bSizer74->Add( m_staticText81, 0, wxRIGHT|wxLEFT, 5 );

    m_staticText82 = new wxStaticText( m_scrolledWindow5, wxID_ANY, _("- different"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText82->Wrap( -1 );
    bSizer74->Add( m_staticText82, 0, wxRIGHT|wxLEFT, 5 );

    m_staticText83 = new wxStaticText( m_scrolledWindow5, wxID_ANY, _("- exists left only"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText83->Wrap( -1 );
    bSizer74->Add( m_staticText83, 0, wxRIGHT|wxLEFT, 5 );

    m_staticText84 = new wxStaticText( m_scrolledWindow5, wxID_ANY, _("- exists right only"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText84->Wrap( -1 );
    bSizer74->Add( m_staticText84, 0, wxRIGHT|wxLEFT, 5 );

    m_scrolledWindow5->SetSizer( bSizer74 );
    m_scrolledWindow5->Layout();
    bSizer74->Fit( m_scrolledWindow5 );
    m_notebook1->AddPage( m_scrolledWindow5, _("File content"), false );

    bSizer20->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );

    m_button8 = new wxButton( this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize( 100,32 ), 0 );
    m_button8->SetDefault();
    m_button8->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer20->Add( m_button8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    this->SetSizer( bSizer20 );
    this->Layout();

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( HelpDlgGenerated::OnClose ) );
    m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HelpDlgGenerated::OnOK ), NULL, this );
}

HelpDlgGenerated::~HelpDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( HelpDlgGenerated::OnClose ) );
    m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( HelpDlgGenerated::OnOK ), NULL, this );
}

AboutDlgGenerated::AboutDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer31;
    bSizer31 = new wxBoxSizer( wxVERTICAL );


    bSizer31->Add( 0, 5, 0, 0, 5 );

    m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    m_panel5->SetBackgroundColour( wxColour( 255, 255, 255 ) );

    wxBoxSizer* bSizer36;
    bSizer36 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmap11 = new wxStaticBitmap( m_panel5, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 404,55 ), 0 );
    bSizer36->Add( m_bitmap11, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_panel5->SetSizer( bSizer36 );
    m_panel5->Layout();
    bSizer36->Fit( m_panel5 );
    bSizer31->Add( m_panel5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    m_staticText15 = new wxStaticText( this, wxID_ANY, _("-Open-Source file synchronization-"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText15->Wrap( -1 );
    m_staticText15->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer31->Add( m_staticText15, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    m_build = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_build->Wrap( -1 );
    m_build->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer31->Add( m_build, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    bSizer31->Add( 0, 10, 0, 0, 5 );

    wxBoxSizer* bSizer53;
    bSizer53 = new wxBoxSizer( wxVERTICAL );

    m_scrolledWindow4 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxHSCROLL|wxVSCROLL );
    m_scrolledWindow4->SetScrollRate( 5, 5 );
    m_scrolledWindow4->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
    m_scrolledWindow4->SetMinSize( wxSize( -1,125 ) );

    wxBoxSizer* bSizer73;
    bSizer73 = new wxBoxSizer( wxVERTICAL );

    m_staticText72 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("Source code written completely in C++ utilizing:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText72->Wrap( -1 );
    m_staticText72->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer73->Add( m_staticText72, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    m_staticText73 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("  MinGW \t- Windows port of GNU Compiler Collection\n  wxWidgets \t- Open-Source GUI framework\n  wxFormBuilder\t- wxWidgets GUI-builder\n  CodeBlocks \t- Open-Source IDE"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText73->Wrap( -1 );
    bSizer73->Add( m_staticText73, 0, wxEXPAND|wxALL, 5 );

    m_staticText74 = new wxStaticText( m_scrolledWindow4, wxID_ANY, _("- ZenJu -"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText74->Wrap( -1 );
    m_staticText74->SetFont( wxFont( 10, 74, 93, 90, false, wxT("Tahoma") ) );

    bSizer73->Add( m_staticText74, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

    m_scrolledWindow4->SetSizer( bSizer73 );
    m_scrolledWindow4->Layout();
    bSizer73->Fit( m_scrolledWindow4 );
    bSizer53->Add( m_scrolledWindow4, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxBOTTOM, 10 );

    m_scrolledWindow3 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxDOUBLE_BORDER|wxHSCROLL|wxVSCROLL );
    m_scrolledWindow3->SetScrollRate( 5, 5 );
    m_scrolledWindow3->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
    m_scrolledWindow3->SetMinSize( wxSize( -1,90 ) );
    m_scrolledWindow3->SetMaxSize( wxSize( -1,100 ) );

    wxBoxSizer* bSizer72;
    bSizer72 = new wxBoxSizer( wxVERTICAL );

    m_staticText54 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Big thanks for localizing FreeFileSync goes out to:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText54->Wrap( -1 );
    m_staticText54->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer72->Add( m_staticText54, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5 );

    wxFlexGridSizer* fgSizer9;
    fgSizer9 = new wxFlexGridSizer( 1, 2, 5, 20 );
    fgSizer9->SetFlexibleDirection( wxBOTH );
    fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_staticText68 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Jean-François Hartmann"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText68->Wrap( -1 );
    fgSizer9->Add( m_staticText68, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText69 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Français"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText69->Wrap( -1 );
    fgSizer9->Add( m_staticText69, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText70 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Tilt"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText70->Wrap( -1 );
    fgSizer9->Add( m_staticText70, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText71 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("日本語"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText71->Wrap( -1 );
    fgSizer9->Add( m_staticText71, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText711 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("M.D. Vrakking"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText711->Wrap( -1 );
    fgSizer9->Add( m_staticText711, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText712 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Nederlands"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText712->Wrap( -1 );
    fgSizer9->Add( m_staticText712, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText91 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Misty Wu"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText91->Wrap( -1 );
    fgSizer9->Add( m_staticText91, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText92 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("简体中文"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText92->Wrap( -1 );
    fgSizer9->Add( m_staticText92, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText911 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Wojtek Pietruszewski"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText911->Wrap( -1 );
    fgSizer9->Add( m_staticText911, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText921 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Język Polski"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText921->Wrap( -1 );
    fgSizer9->Add( m_staticText921, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText9211 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("QuestMark"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText9211->Wrap( -1 );
    fgSizer9->Add( m_staticText9211, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText9212 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Português"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText9212->Wrap( -1 );
    fgSizer9->Add( m_staticText9212, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText92121 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Emmo"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText92121->Wrap( -1 );
    fgSizer9->Add( m_staticText92121, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText92122 = new wxStaticText( m_scrolledWindow3, wxID_ANY, _("Italiano"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText92122->Wrap( -1 );
    fgSizer9->Add( m_staticText92122, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer72->Add( fgSizer9, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    m_scrolledWindow3->SetSizer( bSizer72 );
    m_scrolledWindow3->Layout();
    bSizer72->Fit( m_scrolledWindow3 );
    bSizer53->Add( m_scrolledWindow3, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    bSizer31->Add( bSizer53, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxRIGHT|wxLEFT, 25 );

    m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    bSizer31->Add( m_staticline3, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

    wxStaticBoxSizer* sbSizer7;
    sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );

    m_staticText131 = new wxStaticText( this, wxID_ANY, _("Feedback and suggestions are welcome at:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText131->Wrap( -1 );
    m_staticText131->SetFont( wxFont( 11, 74, 93, 92, false, wxT("Tahoma") ) );

    sbSizer7->Add( m_staticText131, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    bSizer31->Add( sbSizer7, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    wxFlexGridSizer* fgSizer2;
    fgSizer2 = new wxFlexGridSizer( 3, 3, 0, 0 );
    fgSizer2->SetFlexibleDirection( wxHORIZONTAL );
    fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_bitmap9 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    fgSizer2->Add( m_bitmap9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_staticText11 = new wxStaticText( this, wxID_ANY, _("Homepage:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText11->Wrap( -1 );
    m_staticText11->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer2->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, _("FreeFileSync at Sourceforge"), wxT("http://sourceforge.net/projects/freefilesync/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    m_hyperlink1->SetToolTip( _("http://sourceforge.net/projects/freefilesync/") );

    fgSizer2->Add( m_hyperlink1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap10 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    fgSizer2->Add( m_bitmap10, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

    m_staticText13 = new wxStaticText( this, wxID_ANY, _("Email:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText13->Wrap( -1 );
    m_staticText13->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer2->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_hyperlink2 = new wxHyperlinkCtrl( this, wxID_ANY, _("zhnmju123@gmx.de"), wxT("mailto:zhnmju123@gmx.de"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    fgSizer2->Add( m_hyperlink2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_animationControl1 = new wxAnimationCtrl(this, wxID_ANY, wxNullAnimation);
    m_animationControl1->Hide();

    fgSizer2->Add( m_animationControl1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    m_staticText151 = new wxStaticText( this, wxID_ANY, _("If you like FFS:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText151->Wrap( -1 );
    m_staticText151->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer2->Add( m_staticText151, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_hyperlink3 = new wxHyperlinkCtrl( this, wxID_ANY, _("Donate with PayPal"), wxT("https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=zhnmju123%40gmx%2ede&no_shipping=0&no_note=1&tax=0&currency_code=EUR&lc=EN&bn=PP%2dDonationsBF&charset=UTF%2d8"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    fgSizer2->Add( m_hyperlink3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer31->Add( fgSizer2, 0, wxLEFT|wxEXPAND, 10 );

    m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    bSizer31->Add( m_staticline2, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

    wxStaticBoxSizer* sbSizer14;
    sbSizer14 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Published under the GNU General Public License:") ), wxHORIZONTAL );


    sbSizer14->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

    m_bitmap13 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 88,31 ), 0 );
    sbSizer14->Add( m_bitmap13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_hyperlink5 = new wxHyperlinkCtrl( this, wxID_ANY, _("http://www.gnu.org/licenses/gpl.html"), wxT("http://www.gnu.org/licenses/gpl.html"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    sbSizer14->Add( m_hyperlink5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


    sbSizer14->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer31->Add( sbSizer14, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxRIGHT|wxLEFT, 5 );

    m_button8 = new wxButton( this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize( 100,32 ), 0 );
    m_button8->SetDefault();
    m_button8->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer31->Add( m_button8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    this->SetSizer( bSizer31 );
    this->Layout();
    bSizer31->Fit( this );

    this->Centre( wxBOTH );

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AboutDlgGenerated::OnClose ) );
    m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDlgGenerated::OnOK ), NULL, this );
}

AboutDlgGenerated::~AboutDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AboutDlgGenerated::OnClose ) );
    m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDlgGenerated::OnOK ), NULL, this );
}

ErrorDlgGenerated::ErrorDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer24;
    bSizer24 = new wxBoxSizer( wxVERTICAL );


    bSizer24->Add( 0, 10, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer26;
    bSizer26 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmap10 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    bSizer26->Add( m_bitmap10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_textCtrl8 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE|wxTE_READONLY );
    m_textCtrl8->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );

    bSizer26->Add( m_textCtrl8, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    bSizer24->Add( bSizer26, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxBOTTOM, 5 );

    m_checkBoxIgnoreErrors = new wxCheckBox( this, wxID_ANY, _("Ignore subsequent errors"), wxDefaultPosition, wxDefaultSize, 0 );

    m_checkBoxIgnoreErrors->SetToolTip( _("Hide further error messages during the current process") );

    bSizer24->Add( m_checkBoxIgnoreErrors, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


    bSizer24->Add( 0, 10, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer25;
    bSizer25 = new wxBoxSizer( wxHORIZONTAL );

    m_buttonIgnore = new wxButton( this, wxID_OK, _("&Ignore"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_buttonIgnore->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer25->Add( m_buttonIgnore, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_buttonRetry = new wxButton( this, wxID_RETRY, _("&Retry"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_buttonRetry->SetDefault();
    m_buttonRetry->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer25->Add( m_buttonRetry, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_buttonAbort = new wxButton( this, wxID_CANCEL, _("&Abort"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_buttonAbort->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer25->Add( m_buttonAbort, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );


    bSizer25->Add( 5, 0, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer24->Add( bSizer25, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    this->SetSizer( bSizer24 );
    this->Layout();

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ErrorDlgGenerated::OnClose ) );
    m_buttonIgnore->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ErrorDlgGenerated::OnIgnore ), NULL, this );
    m_buttonRetry->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ErrorDlgGenerated::OnRetry ), NULL, this );
    m_buttonAbort->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ErrorDlgGenerated::OnAbort ), NULL, this );
}

ErrorDlgGenerated::~ErrorDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ErrorDlgGenerated::OnClose ) );
    m_buttonIgnore->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ErrorDlgGenerated::OnIgnore ), NULL, this );
    m_buttonRetry->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ErrorDlgGenerated::OnRetry ), NULL, this );
    m_buttonAbort->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ErrorDlgGenerated::OnAbort ), NULL, this );
}

WarningDlgGenerated::WarningDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer24;
    bSizer24 = new wxBoxSizer( wxVERTICAL );


    bSizer24->Add( 0, 10, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer26;
    bSizer26 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmap10 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
    bSizer26->Add( m_bitmap10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_textCtrl8 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE|wxTE_READONLY );
    m_textCtrl8->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );

    bSizer26->Add( m_textCtrl8, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    bSizer24->Add( bSizer26, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );


    bSizer24->Add( 0, 10, 0, wxEXPAND, 5 );

    m_checkBoxDontShowAgain = new wxCheckBox( this, wxID_ANY, _("Do not show this warning again"), wxDefaultPosition, wxDefaultSize, 0 );

    bSizer24->Add( m_checkBoxDontShowAgain, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


    bSizer24->Add( 0, 10, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer25;
    bSizer25 = new wxBoxSizer( wxHORIZONTAL );

    m_buttonIgnore = new wxButton( this, wxID_IGNORE, _("&Ignore"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_buttonIgnore->SetDefault();
    m_buttonIgnore->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer25->Add( m_buttonIgnore, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_buttonAbort = new wxButton( this, wxID_CANCEL, _("&Abort"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_buttonAbort->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer25->Add( m_buttonAbort, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );


    bSizer25->Add( 5, 0, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer24->Add( bSizer25, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    this->SetSizer( bSizer24 );
    this->Layout();

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( WarningDlgGenerated::OnClose ) );
    m_buttonIgnore->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WarningDlgGenerated::OnIgnore ), NULL, this );
    m_buttonAbort->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WarningDlgGenerated::OnAbort ), NULL, this );
}

WarningDlgGenerated::~WarningDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( WarningDlgGenerated::OnClose ) );
    m_buttonIgnore->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WarningDlgGenerated::OnIgnore ), NULL, this );
    m_buttonAbort->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WarningDlgGenerated::OnAbort ), NULL, this );
}

DeleteDlgGenerated::DeleteDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer24;
    bSizer24 = new wxBoxSizer( wxVERTICAL );


    bSizer24->Add( 0, 10, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer41;
    bSizer41 = new wxBoxSizer( wxHORIZONTAL );


    bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );

    m_bitmap12 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), 0 );
    bSizer41->Add( m_bitmap12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_staticTextHeader = new wxStaticText( this, wxID_ANY, _("dummy"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticTextHeader->Wrap( -1 );
    m_staticTextHeader->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer41->Add( m_staticTextHeader, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    bSizer41->Add( 0, 0, 1, wxEXPAND, 5 );

    bSizer24->Add( bSizer41, 0, wxEXPAND, 5 );

    wxBoxSizer* bSizer99;
    bSizer99 = new wxBoxSizer( wxHORIZONTAL );

    m_checkBoxDeleteBothSides = new wxCheckBox( this, wxID_ANY, _("Delete on both sides"), wxDefaultPosition, wxDefaultSize, 0 );

    m_checkBoxDeleteBothSides->SetToolTip( _("Delete on both sides even if the file is selected on one side only") );

    bSizer99->Add( m_checkBoxDeleteBothSides, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


    bSizer99->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

    m_checkBoxUseRecycler = new wxCheckBox( this, wxID_ANY, _("Use Recycle Bin"), wxDefaultPosition, wxDefaultSize, 0 );
    m_checkBoxUseRecycler->SetValue(true);

    bSizer99->Add( m_checkBoxUseRecycler, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    bSizer24->Add( bSizer99, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxLEFT, 10 );

    m_textCtrlMessage = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE|wxTE_READONLY );
    m_textCtrlMessage->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );

    bSizer24->Add( m_textCtrlMessage, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

    wxBoxSizer* bSizer25;
    bSizer25 = new wxBoxSizer( wxHORIZONTAL );

    m_buttonOK = new wxButton( this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_buttonOK->SetDefault();
    m_buttonOK->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer25->Add( m_buttonOK, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_buttonCancel = new wxButton( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_buttonCancel->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer25->Add( m_buttonCancel, 0, wxALL, 5 );

    bSizer24->Add( bSizer25, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    this->SetSizer( bSizer24 );
    this->Layout();

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DeleteDlgGenerated::OnClose ) );
    m_checkBoxDeleteBothSides->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DeleteDlgGenerated::OnDelOnBothSides ), NULL, this );
    m_checkBoxUseRecycler->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DeleteDlgGenerated::OnUseRecycler ), NULL, this );
    m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DeleteDlgGenerated::OnOK ), NULL, this );
    m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DeleteDlgGenerated::OnCancel ), NULL, this );
}

DeleteDlgGenerated::~DeleteDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DeleteDlgGenerated::OnClose ) );
    m_checkBoxDeleteBothSides->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DeleteDlgGenerated::OnDelOnBothSides ), NULL, this );
    m_checkBoxUseRecycler->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DeleteDlgGenerated::OnUseRecycler ), NULL, this );
    m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DeleteDlgGenerated::OnOK ), NULL, this );
    m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DeleteDlgGenerated::OnCancel ), NULL, this );
}

FilterDlgGenerated::FilterDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer21;
    bSizer21 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer86;
    bSizer86 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmap26 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), 0 );
    bSizer86->Add( m_bitmap26, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_panel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    m_panel8->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );

    wxBoxSizer* bSizer72;
    bSizer72 = new wxBoxSizer( wxVERTICAL );

    m_staticText56 = new wxStaticText( m_panel8, wxID_ANY, _("Synchronization filter"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText56->Wrap( -1 );
    m_staticText56->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer72->Add( m_staticText56, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    m_panel8->SetSizer( bSizer72 );
    m_panel8->Layout();
    bSizer72->Fit( m_panel8 );
    bSizer86->Add( m_panel8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    bSizer86->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer21->Add( bSizer86, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 5 );


    bSizer21->Add( 0, 0, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    wxBoxSizer* bSizer70;
    bSizer70 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText44 = new wxStaticText( this, wxID_ANY, _("Only files/directories that pass filtering will be selected for synchronization. The filter will be applied to the full name including path prefix."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText44->Wrap( 400 );
    bSizer70->Add( m_staticText44, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

    m_bpButtonHelp = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    m_bpButtonHelp->SetToolTip( _("Help") );

    m_bpButtonHelp->SetToolTip( _("Help") );

    bSizer70->Add( m_bpButtonHelp, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    bSizer21->Add( bSizer70, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP, 5 );


    bSizer21->Add( 0, 5, 0, 0, 5 );

    m_panel13 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer69;
    bSizer69 = new wxBoxSizer( wxVERTICAL );

    m_staticline10 = new wxStaticLine( m_panel13, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    bSizer69->Add( m_staticline10, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

    wxBoxSizer* bSizer52;
    bSizer52 = new wxBoxSizer( wxVERTICAL );

    m_staticText45 = new wxStaticText( m_panel13, wxID_ANY, _("Hints:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText45->Wrap( -1 );
    m_staticText45->SetFont( wxFont( 10, 74, 90, 92, true, wxT("Tahoma") ) );

    bSizer52->Add( m_staticText45, 0, wxBOTTOM, 5 );

    m_staticText83 = new wxStaticText( m_panel13, wxID_ANY, _("1. Enter full file or directory names separated by ';' or a new line."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText83->Wrap( -1 );
    bSizer52->Add( m_staticText83, 0, 0, 5 );

    m_staticText84 = new wxStaticText( m_panel13, wxID_ANY, _("2. Use wildcard characters '*' and '?'."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText84->Wrap( -1 );
    bSizer52->Add( m_staticText84, 0, 0, 5 );

    m_staticText85 = new wxStaticText( m_panel13, wxID_ANY, _("3. Exclude files directly on main grid via context menu."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText85->Wrap( -1 );
    bSizer52->Add( m_staticText85, 0, 0, 5 );

    m_staticText86 = new wxStaticText( m_panel13, wxID_ANY, _("4. Keep the number of entries small for best performance."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText86->Wrap( -1 );
    bSizer52->Add( m_staticText86, 0, wxBOTTOM, 5 );

    bSizer69->Add( bSizer52, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    wxStaticBoxSizer* sbSizer21;
    sbSizer21 = new wxStaticBoxSizer( new wxStaticBox( m_panel13, wxID_ANY, _("Example") ), wxVERTICAL );

    wxBoxSizer* bSizer66;
    bSizer66 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText181 = new wxStaticText( m_panel13, wxID_ANY, _("Include: *.doc;*.zip;*.exe\nExclude: *\\temp\\*"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText181->Wrap( -1 );
    bSizer66->Add( m_staticText181, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    m_staticText1811 = new wxStaticText( m_panel13, wxID_ANY, _("Synchronize all .doc, .zip and .exe files except everything from folder \"temp\"."), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText1811->Wrap( 250 );
    m_staticText1811->SetFont( wxFont( 8, 74, 93, 90, false, wxT("Tahoma") ) );

    bSizer66->Add( m_staticText1811, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

    sbSizer21->Add( bSizer66, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxLEFT, 5 );

    bSizer69->Add( sbSizer21, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxRIGHT|wxLEFT, 5 );

    m_panel13->SetSizer( bSizer69 );
    m_panel13->Layout();
    bSizer69->Fit( m_panel13 );
    bSizer21->Add( m_panel13, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxEXPAND, 5 );

    wxStaticBoxSizer* sbSizer8;
    sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );

    wxFlexGridSizer* fgSizer3;
    fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
    fgSizer3->SetFlexibleDirection( wxBOTH );
    fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


    fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

    m_staticText15 = new wxStaticText( this, wxID_ANY, _("Include"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText15->Wrap( -1 );
    m_staticText15->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer3->Add( m_staticText15, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    m_bitmap8 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), 0 );
    fgSizer3->Add( m_bitmap8, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_textCtrlInclude = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 400,-1 ), wxTE_MULTILINE );
    fgSizer3->Add( m_textCtrlInclude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer8->Add( fgSizer3, 0, 0, 5 );

    wxFlexGridSizer* fgSizer4;
    fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
    fgSizer4->SetFlexibleDirection( wxBOTH );
    fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


    fgSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

    m_staticText16 = new wxStaticText( this, wxID_ANY, _("Exclude"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText16->Wrap( -1 );
    m_staticText16->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    fgSizer4->Add( m_staticText16, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    m_bitmap9 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), 0 );
    fgSizer4->Add( m_bitmap9, 0, wxTOP|wxBOTTOM|wxLEFT, 5 );

    m_textCtrlExclude = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 400,-1 ), wxTE_MULTILINE );
    fgSizer4->Add( m_textCtrlExclude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer8->Add( fgSizer4, 0, 0, 5 );

    bSizer21->Add( sbSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxLEFT|wxEXPAND, 5 );


    bSizer21->Add( 0, 0, 0, 0, 5 );

    wxBoxSizer* bSizer22;
    bSizer22 = new wxBoxSizer( wxHORIZONTAL );

    m_button9 = new wxButton( this, wxID_DEFAULT, _("&Default"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button9->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer22->Add( m_button9, 0, wxALL, 5 );


    bSizer22->Add( 0, 0, 1, wxEXPAND, 5 );

    m_button10 = new wxButton( this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button10->SetDefault();
    m_button10->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer22->Add( m_button10, 0, wxALL, 5 );

    m_button17 = new wxButton( this, wxID_CANCEL, _("dummy"), wxDefaultPosition, wxSize( 0,0 ), 0 );
    bSizer22->Add( m_button17, 0, wxALIGN_BOTTOM, 5 );

    bSizer21->Add( bSizer22, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 5 );

    this->SetSizer( bSizer21 );
    this->Layout();
    bSizer21->Fit( this );

    this->Centre( wxBOTH );

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( FilterDlgGenerated::OnClose ) );
    m_bpButtonHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlgGenerated::OnHelp ), NULL, this );
    m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlgGenerated::OnDefault ), NULL, this );
    m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlgGenerated::OnOK ), NULL, this );
    m_button17->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlgGenerated::OnCancel ), NULL, this );
}

FilterDlgGenerated::~FilterDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( FilterDlgGenerated::OnClose ) );
    m_bpButtonHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlgGenerated::OnHelp ), NULL, this );
    m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlgGenerated::OnDefault ), NULL, this );
    m_button10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlgGenerated::OnOK ), NULL, this );
    m_button17->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlgGenerated::OnCancel ), NULL, this );
}

CustomizeColsDlgGenerated::CustomizeColsDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer96;
    bSizer96 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer99;
    bSizer99 = new wxBoxSizer( wxHORIZONTAL );

    wxArrayString m_checkListColumnsChoices;
    m_checkListColumns = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkListColumnsChoices, 0 );
    bSizer99->Add( m_checkListColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    wxBoxSizer* bSizer98;
    bSizer98 = new wxBoxSizer( wxVERTICAL );

    m_bpButton29 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    m_bpButton29->SetToolTip( _("Move column up") );

    m_bpButton29->SetToolTip( _("Move column up") );

    bSizer98->Add( m_bpButton29, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

    m_bpButton30 = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), wxBU_AUTODRAW );
    m_bpButton30->SetToolTip( _("Move column down") );

    m_bpButton30->SetToolTip( _("Move column down") );

    bSizer98->Add( m_bpButton30, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );

    bSizer99->Add( bSizer98, 0, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer96->Add( bSizer99, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    wxBoxSizer* bSizer97;
    bSizer97 = new wxBoxSizer( wxHORIZONTAL );

    m_button28 = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button28->SetDefault();
    m_button28->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer97->Add( m_button28, 0, wxALL, 5 );

    m_button9 = new wxButton( this, wxID_DEFAULT, _("&Default"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button9->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer97->Add( m_button9, 0, wxALL, 5 );

    m_button29 = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button29->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer97->Add( m_button29, 0, wxALL, 5 );

    bSizer96->Add( bSizer97, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


    bSizer96->Add( 0, 0, 0, wxALIGN_CENTER_HORIZONTAL, 20 );

    this->SetSizer( bSizer96 );
    this->Layout();
    bSizer96->Fit( this );

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CustomizeColsDlgGenerated::OnClose ) );
    m_bpButton29->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnMoveUp ), NULL, this );
    m_bpButton30->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnMoveDown ), NULL, this );
    m_button28->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnOkay ), NULL, this );
    m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnDefault ), NULL, this );
    m_button29->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnCancel ), NULL, this );
}

CustomizeColsDlgGenerated::~CustomizeColsDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CustomizeColsDlgGenerated::OnClose ) );
    m_bpButton29->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnMoveUp ), NULL, this );
    m_bpButton30->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnMoveDown ), NULL, this );
    m_button28->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnOkay ), NULL, this );
    m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnDefault ), NULL, this );
    m_button29->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CustomizeColsDlgGenerated::OnCancel ), NULL, this );
}

GlobalSettingsDlgGenerated::GlobalSettingsDlgGenerated( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer95;
    bSizer95 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* bSizer86;
    bSizer86 = new wxBoxSizer( wxHORIZONTAL );

    m_bitmapSettings = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 40,40 ), 0 );
    bSizer86->Add( m_bitmapSettings, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

    m_panel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
    m_panel8->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );

    wxBoxSizer* bSizer72;
    bSizer72 = new wxBoxSizer( wxVERTICAL );

    m_staticText56 = new wxStaticText( m_panel8, wxID_ANY, _("Global settings"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText56->Wrap( -1 );
    m_staticText56->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer72->Add( m_staticText56, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    m_panel8->SetSizer( bSizer72 );
    m_panel8->Layout();
    bSizer72->Fit( m_panel8 );
    bSizer86->Add( m_panel8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    bSizer86->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

    bSizer95->Add( bSizer86, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


    bSizer95->Add( 0, 10, 0, 0, 5 );

    wxStaticBoxSizer* sbSizer23;
    sbSizer23 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );

    m_checkBoxHandleDstFat = new wxCheckBox( this, wxID_ANY, _("FAT32: Handle Daylight Saving Time"), wxDefaultPosition, wxDefaultSize, 0 );
    m_checkBoxHandleDstFat->SetValue(true);

    m_checkBoxHandleDstFat->SetToolTip( _("Active for FAT/FAT32 drives only: When comparing filetimes treat files that differ by less or equal than 1 hour as equal. This ensures daylight saving time switches are handled properly.") );

    sbSizer23->Add( m_checkBoxHandleDstFat, 0, wxALL, 5 );

    wxBoxSizer* bSizer104;
    bSizer104 = new wxBoxSizer( wxHORIZONTAL );

    m_staticText97 = new wxStaticText( this, wxID_ANY, _("File Manager integration:"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText97->Wrap( -1 );
    m_staticText97->SetToolTip( _("This commandline will be executed each time you doubleclick on a filename. %x serves as a placeholder for the selected file.") );

    bSizer104->Add( m_staticText97, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_textCtrlFileManager = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
    m_textCtrlFileManager->SetToolTip( _("This commandline will be executed each time you doubleclick on a filename. %name serves as a placeholder for the selected file.") );

    bSizer104->Add( m_textCtrlFileManager, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    sbSizer23->Add( bSizer104, 1, wxEXPAND, 5 );

    m_buttonResetWarnings = new wxButtonWithImage( this, wxID_ANY, _("Reset"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
    m_buttonResetWarnings->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );
    m_buttonResetWarnings->SetToolTip( _("Resets all warning messages") );

    sbSizer23->Add( m_buttonResetWarnings, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxLEFT, 5 );

    bSizer95->Add( sbSizer23, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxLEFT, 5 );


    bSizer95->Add( 0, 10, 0, 0, 5 );

    wxBoxSizer* bSizer97;
    bSizer97 = new wxBoxSizer( wxHORIZONTAL );

    m_button28 = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button28->SetDefault();
    m_button28->SetFont( wxFont( 10, 74, 90, 92, false, wxT("Tahoma") ) );

    bSizer97->Add( m_button28, 0, wxALL, 5 );

    m_button9 = new wxButton( this, wxID_DEFAULT, _("&Default"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button9->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer97->Add( m_button9, 0, wxALL, 5 );

    m_button29 = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxSize( -1,30 ), 0 );
    m_button29->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Tahoma") ) );

    bSizer97->Add( m_button29, 0, wxALL, 5 );

    bSizer95->Add( bSizer97, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

    this->SetSizer( bSizer95 );
    this->Layout();
    bSizer95->Fit( this );

    // Connect Events
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GlobalSettingsDlgGenerated::OnClose ) );
    m_buttonResetWarnings->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlobalSettingsDlgGenerated::OnResetWarnings ), NULL, this );
    m_button28->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlobalSettingsDlgGenerated::OnOkay ), NULL, this );
    m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlobalSettingsDlgGenerated::OnDefault ), NULL, this );
    m_button29->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlobalSettingsDlgGenerated::OnCancel ), NULL, this );
}

GlobalSettingsDlgGenerated::~GlobalSettingsDlgGenerated()
{
    // Disconnect Events
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GlobalSettingsDlgGenerated::OnClose ) );
    m_buttonResetWarnings->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlobalSettingsDlgGenerated::OnResetWarnings ), NULL, this );
    m_button28->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlobalSettingsDlgGenerated::OnOkay ), NULL, this );
    m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlobalSettingsDlgGenerated::OnDefault ), NULL, this );
    m_button29->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlobalSettingsDlgGenerated::OnCancel ), NULL, this );
}
