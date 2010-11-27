// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) 2008-2010 ZenJu (zhnmju123 AT gmx.de)                    *
// **************************************************************************
//
#include "custom_combo_box.h"


CustomComboBox::CustomComboBox(wxWindow* parent,
                               wxWindowID id,
                               const wxString& value,
                               const wxPoint& pos,
                               const wxSize& size,
                               int n,
                               const wxString choices[],
                               long style,
                               const wxValidator& validator,
                               const wxString& name) :
    wxComboBox(parent, id, value, pos, size, n, choices, style, validator, name)
#if wxCHECK_VERSION(2, 9, 1)
    , dropDownShown(false)
#endif
{
    //register key event to enable item deletion
    this->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CustomComboBox::OnKeyEvent), NULL, this );

#if wxCHECK_VERSION(2, 9, 1)
    this->Connect(wxEVT_COMMAND_COMBOBOX_DROPDOWN, wxCommandEventHandler(CustomComboBox::OnShowDropDown), NULL, this );
    this->Connect(wxEVT_COMMAND_COMBOBOX_CLOSEUP, wxCommandEventHandler(CustomComboBox::OnHideDropDown), NULL, this );
#endif
}


#if wxCHECK_VERSION(2, 9, 1)
void CustomComboBox::OnShowDropDown(wxCommandEvent& event)
{
    dropDownShown = true;
    event.Skip();
}


void CustomComboBox::OnHideDropDown(wxCommandEvent& event)
{
    dropDownShown = false;
    event.Skip();
}
#endif


void CustomComboBox::OnKeyEvent(wxKeyEvent& event)
{
    const int keyCode = event.GetKeyCode();
    if (keyCode == WXK_DELETE || keyCode == WXK_NUMPAD_DELETE)
    {
        //try to delete the currently selected config history item
        const int selectedItem = this->GetCurrentSelection();
        if (0 <= selectedItem && selectedItem < static_cast<int>(this->GetCount()) &&
#if wxCHECK_VERSION(2, 9, 1)
                    dropDownShown)
#else
                    //what a mess...:
                    (GetValue() != GetString(selectedItem) || //avoid problems when a character shall be deleted instead of list item
                    GetValue() == wxEmptyString)) //exception: always allow removing empty entry
#endif
    {
        //save old (selected) value: deletion seems to have influence on this
        const wxString currentVal = this->GetValue();
            this->SetSelection(wxNOT_FOUND);

            //delete selected row
            this->Delete(selectedItem);

            //(re-)set value
            this->SetValue(currentVal);

            //eat up key event
            return;
        }
    }
    event.Skip();
}


void CustomComboBox::addPairToFolderHistory(const wxString& newFolder, unsigned int maxHistSize)
{
    //don't add empty directories
    if (newFolder.empty())
        return;

    const wxString oldVal = this->GetValue();

    //insert new folder or put it to the front if already existing
#ifdef FFS_WIN //don't respect case in windows build
    const int pos = FindString(newFolder, false);
#elif defined FFS_LINUX
    const int pos = FindString(newFolder, true);
#endif
    if (pos != wxNOT_FOUND)
        this->Delete(pos);

    this->Insert(newFolder, 0);

    //keep maximal size of history list
    if (this->GetCount() > maxHistSize)
        this->Delete(maxHistSize);

    this->SetSelection(wxNOT_FOUND);  //don't select anything
    this->SetValue(oldVal);           //but preserve main text!
}
