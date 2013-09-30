// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) Zenju (zenju AT gmx DOT de) - All Rights Reserved        *
// **************************************************************************

#include "read_txt.h"

using namespace zen;


namespace
{
warn_static("superfluous method")
std::string detectLineBreak(const Zstring& filename) //throw FileError
{
    //read a (hopefully) significant portion of data
    zen::FileInput input(filename);

    std::vector<char> buffer(64 * 1024);
    size_t bytesRead = input.read(&buffer[0], buffer.size()); //throw FileError
    buffer.resize(bytesRead);

    //detect line break
    std::string linebreakChars = "\r\n";
    std::vector<char>::iterator iter = std::find_first_of(buffer.begin(), buffer.end(),
                                                          linebreakChars.begin(), linebreakChars.end());
    if (iter != buffer.end())
    {
        if (*iter == '\r')
        {
            ++iter;
            if (iter != buffer.end())
            {

                if (*iter == '\n')
                    return "\r\n"; //Windows
                else
                    return "\r"; //Mac
            }
        }
        else if (*iter == '\n')
            return "\n"; //Linux
    }
    //fallback
    return "\n";
}
}


LineExtractor::LineExtractor(const Zstring& filename, const std::string& lineBreak) :  //throw FileError
    inputStream(filename), bufferLogBegin(buffer.begin()), lineBreak_(lineBreak)
{
    if (lineBreak.empty())
        lineBreak_ = detectLineBreak(filename); //throw FileError
}


bool LineExtractor::getLine(std::string& output) //throw FileError
{
    warn_static("don't use lineBreak, but support any of r, n, rn!!!")
    for (;;)
    {
        //check if full line is in buffer
        std::vector<char>::iterator iter = std::search(bufferLogBegin, buffer.end(), lineBreak_.begin(), lineBreak_.end());
        if (iter != buffer.end())
        {
            output.assign(bufferLogBegin, iter);
            bufferLogBegin = iter + lineBreak_.size();
            return true;
        }

        buffer.erase(buffer.begin(), bufferLogBegin);
        bufferLogBegin = buffer.begin();

        //if done: cleanup
        if (inputStream.eof())
        {
            if (buffer.empty())
                return false;

            output.assign(buffer.begin(), buffer.end());
            buffer.clear();
            return true;
        }

        //read next block
        const size_t BLOCK_SIZE = 512 * 1024;
        buffer.resize(buffer.size() + BLOCK_SIZE);

        size_t bytesRead = inputStream.read(&buffer[0] + buffer.size() - BLOCK_SIZE, BLOCK_SIZE); //throw FileError
        assert(bytesRead <= BLOCK_SIZE); //promised by FileInput()

        if (bytesRead < BLOCK_SIZE)
            buffer.resize(buffer.size() - (BLOCK_SIZE - bytesRead));

        bufferLogBegin = buffer.begin();
    }
}
