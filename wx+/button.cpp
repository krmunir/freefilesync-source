// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) Zenju (zenju AT gmx DOT de) - All Rights Reserved        *
// **************************************************************************

#include "button.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <zen/string_tools.h>
#include <wx/dcmemory.h>
#include <wx/image.h>
#include "image_tools.h"

using namespace zen;


void zen::setImage(wxBitmapButton& button, const wxBitmap& bmp)
{
    if (!isEqual(button.GetBitmapLabel(), bmp))
        button.SetBitmapLabel(bmp);
}


BitmapButton::BitmapButton(wxWindow* parent,
                           wxWindowID id,
                           const wxString& label,
                           const wxPoint& pos,
                           const wxSize& size,
                           long style,
                           const wxValidator& validator,
                           const wxString& name) :
    wxBitmapButton(parent, id, wxNullBitmap, pos, size, style | wxBU_AUTODRAW, validator, name),
    m_spaceAfter(0),
    m_spaceBefore(0)
{
    setTextLabel(label);
}


void BitmapButton::setBitmapFront(const wxBitmap& bitmap, size_t spaceAfter)
{
    if (!isEqual(bitmap, bitmapFront) || spaceAfter != m_spaceAfter) //avoid flicker
    {
        bitmapFront  = bitmap;
        m_spaceAfter = static_cast<unsigned int>(spaceAfter);
        refreshButtonLabel();
    }
}


void BitmapButton::setTextLabel(const wxString& text)
{
    if (text != textLabel) //avoid flicker
    {
        textLabel = text;
        wxBitmapButton::SetLabel(text);
        refreshButtonLabel();
    }
}


void BitmapButton::setBitmapBack(const wxBitmap& bitmap, size_t spaceBefore)
{
    if (!isEqual(bitmap, bitmapBack) || spaceBefore != m_spaceBefore) //avoid flicker
    {
        bitmapBack    = bitmap;
        m_spaceBefore = static_cast<unsigned int>(spaceBefore);
        refreshButtonLabel();
    }
}


void makeWhiteTransparent(wxImage& image) //assume black text on white background
{
    unsigned char* alphaFirst = image.GetAlpha();
    if (alphaFirst)
    {
        unsigned char* alphaLast = alphaFirst + image.GetWidth() * image.GetHeight();

        //dist(black, white)
        const double distBlackWhite = 255 * std::sqrt(3.0);

        const unsigned char* bytePos = image.GetData();

        for (unsigned char* j = alphaFirst; j != alphaLast; ++j)
        {
            unsigned char r = *bytePos++; //
            unsigned char g = *bytePos++; //each pixel consists of three chars
            unsigned char b = *bytePos++; //

            //dist((r,g,b), white)
            double distColWhite = std::sqrt((255.0 - r) * (255.0 - r) + (255.0 - g) * (255.0 - g) + (255.0 - b) * (255.0 - b));

            //black(0,0,0) becomes fully opaque(255), while white(255,255,255) becomes transparent(0)
            *j = distColWhite / distBlackWhite * wxIMAGE_ALPHA_OPAQUE;
        }
    }
}


wxSize getSizeNeeded(const wxString& text, wxFont& font)
{
    const wxString& textFormatted = replaceCpy(text, L"&", L"", false); //remove accelerator

    wxCoord width  = 0;
    wxCoord height = 0;

    wxMemoryDC dc;
    dc.GetMultiLineTextExtent(textFormatted, &width, &height, nullptr, &font);

    return wxSize(width, height);
}


wxBitmap BitmapButton::createBitmapFromText(const wxString& text)
{
    //wxDC::DrawLabel() doesn't respect alpha channel at all => apply workaround to calculate alpha values manually:

    if (text.empty())
        return wxBitmap();

    wxFont currentFont = wxBitmapButton::GetFont();
    wxColor textColor  = wxBitmapButton::GetForegroundColour();

    wxSize sizeNeeded = getSizeNeeded(text, currentFont);
    wxBitmap newBitmap(sizeNeeded.GetWidth(), sizeNeeded.GetHeight());

    {
        wxMemoryDC dc;
        dc.SelectObject(newBitmap);

        //set up white background
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();

        //find position of accelerator
        int indexAccel = -1;
        size_t accelPos;
        wxString textLabelFormatted = text;
        if ((accelPos = text.find(wxT("&"))) != wxString::npos)
        {
            replace(textLabelFormatted, L"&", L"", false); //remove accelerator
            indexAccel = static_cast<int>(accelPos);
        }

        dc.SetTextForeground(*wxBLACK); //for use in makeWhiteTransparent
        dc.SetTextBackground(*wxWHITE); //
        dc.SetFont(currentFont);

        dc.DrawLabel(textLabelFormatted, wxNullBitmap, wxRect(0, 0, newBitmap.GetWidth(), newBitmap.GetHeight()), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, indexAccel);

        dc.SelectObject(wxNullBitmap);
    }

    //add alpha channel to image
    wxImage finalImage(newBitmap.ConvertToImage());
    finalImage.SetAlpha();

    //linearInterpolation(finalImage);

    //calculate values for alpha channel
    makeWhiteTransparent(finalImage);

    //now apply real text color
    unsigned char* bytePos = finalImage.GetData();
    const int pixelCount = finalImage.GetWidth() * finalImage.GetHeight();
    for (int i = 0; i < pixelCount; ++ i)
    {
        *bytePos++ = textColor.Red();
        *bytePos++ = textColor.Green();
        *bytePos++ = textColor.Blue();
    }

    return wxBitmap(finalImage);
}


//copy one image into another, allowing arbitrary overlapping! (pos may contain negative numbers)
void writeToImage(const wxImage& source, const wxPoint& pos, wxImage& target)
{
    //determine startpositions in source and target image, as well as width and height to be copied
    wxPoint posSrc, posTrg;
    int width, height;

    //X-axis
    if (pos.x < 0)
    {
        posSrc.x = -pos.x;
        posTrg.x = 0;
        width = std::min(pos.x + source.GetWidth(), target.GetWidth());
    }
    else
    {
        posSrc.x = 0;
        posTrg.x = pos.x;
        width = std::min(target.GetWidth() - pos.x, source.GetWidth());
    }

    //Y-axis
    if (pos.y < 0)
    {
        posSrc.y = -pos.y;
        posTrg.y = 0;
        height = std::min(pos.y + source.GetHeight(), target.GetHeight());
    }
    else
    {
        posSrc.y = 0;
        posTrg.y = pos.y;
        height = std::min(target.GetHeight() - pos.y, source.GetHeight());
    }


    if (width > 0 && height > 0)
    {
        {
            //copy source to target respecting overlapping parts
            const unsigned char*       sourcePtr    = source.GetData() + 3 * (posSrc.x + posSrc.y * source.GetWidth());
            const unsigned char* const sourcePtrEnd = source.GetData() + 3 * (posSrc.x + (posSrc.y + height) * source.GetWidth());
            unsigned char*             targetPtr    = target.GetData() + 3 * (posTrg.x + posTrg.y * target.GetWidth());

            while (sourcePtr < sourcePtrEnd)
            {
                memcpy(targetPtr, sourcePtr, 3 * width);
                sourcePtr += 3 * source.GetWidth();
                targetPtr += 3 * target.GetWidth();
            }
        }

        //handle different cases concerning alpha channel
        if (source.HasAlpha())
        {
            if (!target.HasAlpha())
            {
                target.SetAlpha();
                unsigned char* alpha = target.GetAlpha();
                memset(alpha, wxIMAGE_ALPHA_OPAQUE, target.GetWidth() * target.GetHeight());
            }

            //copy alpha channel
            const unsigned char*       sourcePtr    = source.GetAlpha() + (posSrc.x + posSrc.y * source.GetWidth());
            const unsigned char* const sourcePtrEnd = source.GetAlpha() + (posSrc.x + (posSrc.y + height) * source.GetWidth());
            unsigned char*             targetPtr    = target.GetAlpha() + (posTrg.x + posTrg.y * target.GetWidth());

            while (sourcePtr < sourcePtrEnd)
            {
                memcpy(targetPtr, sourcePtr, width);
                sourcePtr += source.GetWidth();
                targetPtr += target.GetWidth();
            }
        }
        else if (target.HasAlpha())
        {
            unsigned char*             targetPtr    = target.GetAlpha() + (posTrg.x + posTrg.y * target.GetWidth());
            const unsigned char* const targetPtrEnd = target.GetAlpha() + (posTrg.x + (posTrg.y + height) * target.GetWidth());

            while (targetPtr < targetPtrEnd)
            {
                memset(targetPtr, wxIMAGE_ALPHA_OPAQUE, width);
                targetPtr += target.GetWidth();
            }
        }
    }
}


namespace
{
inline
wxSize getSize(const wxBitmap& bmp)
{
    return bmp.IsOk() ? wxSize(bmp.GetWidth(), bmp.GetHeight()) : wxSize(0, 0);
}
}


void BitmapButton::refreshButtonLabel()
{
    wxBitmap bitmapText = createBitmapFromText(textLabel);

    wxSize szFront = getSize(bitmapFront); //
    wxSize szText  = getSize(bitmapText);  //make sure to NOT access null-bitmaps!
    wxSize szBack  = getSize(bitmapBack);  //

    //calculate dimensions of new button
    const int height = std::max(std::max(szFront.GetHeight(), szText.GetHeight()), szBack.GetHeight());
    const int width  = szFront.GetWidth() + m_spaceAfter + szText.GetWidth() + m_spaceBefore + szBack.GetWidth();

    //create a transparent image
    wxImage transparentImage(width, height, false);
    transparentImage.SetAlpha();
    unsigned char* alpha = transparentImage.GetAlpha();
    ::memset(alpha, wxIMAGE_ALPHA_TRANSPARENT, width * height);

    //wxDC::DrawLabel() unfortunately isn't working for transparent images on Linux, so we need to use custom image-concatenation
    if (bitmapFront.IsOk())
        writeToImage(bitmapFront.ConvertToImage(),
                     wxPoint(0, (transparentImage.GetHeight() - bitmapFront.GetHeight()) / 2),
                     transparentImage);

    if (bitmapText.IsOk())
        writeToImage(bitmapText.ConvertToImage(),
                     wxPoint(szFront.GetWidth() + m_spaceAfter, (transparentImage.GetHeight() - bitmapText.GetHeight()) / 2),
                     transparentImage);

    if (bitmapBack.IsOk())
        writeToImage(bitmapBack.ConvertToImage(),
                     wxPoint(szFront.GetWidth() + m_spaceAfter + szText.GetWidth() + m_spaceBefore, (transparentImage.GetHeight() - bitmapBack.GetHeight()) / 2),
                     transparentImage);

    //adjust button size
    wxSize minSize = GetMinSize();

    //SetMinSize() instead of SetSize() is needed here for wxWindows layout determination to work corretly
    wxBitmapButton::SetMinSize(wxSize(std::max(width + 10, minSize.GetWidth()), std::max(height + 5, minSize.GetHeight())));

    //finally set bitmap
    wxBitmapButton::SetBitmapLabel(wxBitmap(transparentImage));
}
