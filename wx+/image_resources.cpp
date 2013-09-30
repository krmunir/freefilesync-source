// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) Zenju (zenju AT gmx DOT de) - All Rights Reserved        *
// **************************************************************************

#include "image_resources.h"
#include <memory>
#include <map>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include <zen/utf.h>

using namespace zen;


namespace
{
void loadAnimFromZip(wxZipInputStream& zipInput, wxAnimation& anim)
{
    //work around wxWidgets bug:
    //construct seekable input stream (zip-input stream is non-seekable) for wxAnimation::Load()
    //luckily this method call is very fast: below measurement precision!
    std::vector<char> data;
    data.reserve(10000);

    int newValue = 0;
    while ((newValue = zipInput.GetC()) != wxEOF)
        data.push_back(newValue);

    wxMemoryInputStream seekAbleStream(&data.front(), data.size()); //stream does not take ownership of data

    anim.Load(seekAbleStream, wxANIMATION_TYPE_GIF);
}


class GlobalResources
{
public:
    static GlobalResources& instance()
    {
        static GlobalResources inst;
        return inst;
    }

    void init(const Zstring& filename);

    const wxBitmap& getImage(const wxString& name) const;
    const wxAnimation& getAnimation(const wxString& name) const;

private:
    GlobalResources() {}
    GlobalResources(const GlobalResources&);
    GlobalResources& operator=(const GlobalResources&);

    std::map<wxString, wxBitmap> bitmaps;
    std::map<wxString, wxAnimation> anims;
};


void GlobalResources::init(const Zstring& filename)
{
    wxFFileInputStream input(utfCvrtTo<wxString>(filename));
    if (input.IsOk()) //if not... we don't want to react too harsh here
    {
        //activate support for .png files
        wxImage::AddHandler(new wxPNGHandler); //ownership passed

        wxZipInputStream resourceFile(input, wxConvUTF8);
        //do NOT rely on wxConvLocal! On failure shows unhelpful popup "Cannot convert from the charset 'Unknown encoding (-1)'!"

        while (true)
        {
            std::unique_ptr<wxZipEntry> entry(resourceFile.GetNextEntry()); //take ownership!
            if (!entry)
                break;

            const wxString name = entry->GetName();

            //generic image loading
            if (endsWith(name, L".png"))
                bitmaps.insert(std::make_pair(name, wxImage(resourceFile, wxBITMAP_TYPE_PNG)));
            else if (endsWith(name, L".gif"))
                loadAnimFromZip(resourceFile, anims[name]);
        }
    }
}


const wxBitmap& GlobalResources::getImage(const wxString& name) const
{
    auto it = bitmaps.find(contains(name, L'.') ? name : name + L".png"); //assume .png ending if nothing else specified
    if (it != bitmaps.end())
        return it->second;

    assert(false);
    return wxNullBitmap;
}


const wxAnimation& GlobalResources::getAnimation(const wxString& name) const
{
    auto it = anims.find(contains(name, L'.') ? name : name + L".gif");
    if (it != anims.end())
        return it->second;

    assert(false);
    return wxNullAnimation;
}
}


void zen::initResourceImages(const Zstring& filename) { GlobalResources::instance().init(filename); }

const wxBitmap& zen::getResourceImage(const wxString& name) { return GlobalResources::instance().getImage(name); }

const wxAnimation& zen::getResourceAnimation(const wxString& name) { return GlobalResources::instance().getAnimation(name); }