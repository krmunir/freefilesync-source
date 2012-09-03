// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) ZenJu (zenju AT gmx DOT de) - All Rights Reserved        *
// **************************************************************************

#ifndef SIMPLE_H_INCLUDED_18134135134135345489
#define SIMPLE_H_INCLUDED_18134135134135345489

#include <zen/serialize.h>

namespace zen
{
class ZlibInternalError {};

// compression level must be between 0 and 9:
// 0: no compression
// 9: best compression
template <class BinContainer> //as specified in serialize.h
BinContainer compress(const BinContainer& stream, int level); //throw ZlibInternalError
//caveat: output stream is physically larger than input! => strip additional reserved space if needed: "BinContainer(output.begin(), output.end())"

template <class BinContainer>
BinContainer decompress(const BinContainer& stream);          //throw ZlibInternalError




















//######################## implementation ##########################
namespace impl
{
size_t zlib_compressBound(size_t len);
size_t zlib_compress  (const void* src, size_t srcLen, void* trg, size_t trgLen, int level); //throw ZlibInternalError
size_t zlib_decompress(const void* src, size_t srcLen, void* trg, size_t trgLen);            //throw ZlibInternalError
}


template <class BinContainer>
BinContainer compress(const BinContainer& stream, int level) //throw ZlibInternalError
{
    BinContainer contOut;
    if (!stream.empty()) //don't dereference iterator into empty container!
    {
        //save uncompressed stream size for decompression
        const std::uint64_t uncompressedSize = stream.size(); //use portable number type!
        contOut.resize(sizeof(uncompressedSize));
        std::copy(reinterpret_cast<const char*>(&uncompressedSize),
                  reinterpret_cast<const char*>(&uncompressedSize) + sizeof(uncompressedSize),
                  &*contOut.begin());

        const size_t bufferEstimate = impl::zlib_compressBound(stream.size()); //upper limit for buffer size, larger than input size!!!

        contOut.resize(contOut.size() + bufferEstimate);

        const size_t bytesWritten = impl::zlib_compress(&*stream.begin(),
                                                        stream.size(),
                                                        &*contOut.begin() + contOut.size() - bufferEstimate,
                                                        bufferEstimate,
                                                        level); //throw ZlibInternalError
        if (bytesWritten < bufferEstimate)
            contOut.resize(contOut.size() - (bufferEstimate - bytesWritten)); //caveat: unsigned arithmetics
        //caveat: physical memory consumption still *unchanged*!
    }
    return contOut;
}


template <class BinContainer>
BinContainer decompress(const BinContainer& stream) //throw ZlibInternalError
{
    BinContainer contOut;
    if (!stream.empty()) //don't dereference iterator into empty container!
    {
        //retrieve size of uncompressed data
        std::uint64_t uncompressedSize = 0; //use portable number type!
        if (stream.size() < sizeof(uncompressedSize))
            throw ZlibInternalError();
        std::copy(&*stream.begin(),
                  &*stream.begin() + sizeof(uncompressedSize),
                  reinterpret_cast<char*>(&uncompressedSize));
        try
        {
            //attention: contOut MUST NOT be empty! Else it will pass a nullptr to zlib_decompress() => Z_STREAM_ERROR although "uncompressedSize == 0"!!!
            //secondary bug: don't dereference iterator into empty container!
            if (uncompressedSize == 0) //cannot be 0: compress() directly maps empty -> empty container skipping zlib!
                throw ZlibInternalError();

            contOut.resize(uncompressedSize); //throw std::bad_alloc
        }
        catch (std::bad_alloc&) //most likely due to data corruption!
        {
            throw ZlibInternalError();
        }

        const size_t bytesWritten = impl::zlib_decompress(&*stream.begin() + sizeof(uncompressedSize),
                                                          stream.size() - sizeof(uncompressedSize),
                                                          &*contOut.begin(),
                                                          uncompressedSize); //throw ZlibInternalError
        if (bytesWritten != uncompressedSize)
            throw ZlibInternalError();
    }
    return contOut;
}
}

#endif //SIMPLE_H_INCLUDED_18134135134135345489
