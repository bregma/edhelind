/*
 * Copyright 2020  Stephen M. Webb <stephen.webb@bregmasoft.ca>
 * 
 * This file is part of Edhelind.
 * 
 * Edhelind is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "libedhel/elfimage.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>

#ifdef DUDE_LATER_DUDE
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include "structures/elfheader.h"
#include "structures/programheader.h"
#endif


ElfImageView::
ElfImageView(ElfImage const* elfFile, std::size_t offset, std::size_t size)
: elf_file_(elfFile)
, offset_(offset)
, size_(size)
{ }


ElfImageView ElfImageView::
view(std::size_t offset, std::size_t size) const
{
    if (offset + size > size_)
    {
        std::ostringstream ostr;
        ostr << "subview size " << size << " is larger than view size " << size_;
        throw std::runtime_error(ostr.str());
    }
    return ElfImageView(elf_file_, offset_ + offset, size);
}


std::size_t ElfImageView::
size() const
{
    return size_;
}


std::uint8_t ElfImageView::
get_uint8(std::size_t offset) const
{
    return elf_file_->get_uint8(offset_ + offset);
}


std::byte const* ElfImageView::
get_bytes(std::size_t offset) const
{
    return elf_file_->get_bytes(offset_ + offset);
}


std::uint16_t ElfImageView::
get_uint16(std::size_t offset) const
{
    return elf_file_->get_uint16(offset_ + offset);
}


std::uint32_t ElfImageView::
get_uint32(std::size_t offset) const
{
    return elf_file_->get_uint32(offset_ + offset);
}


std::uint64_t ElfImageView::
get_uint64(std::size_t offset) const
{
    return elf_file_->get_uint64(offset_ + offset);
}


std::string ElfImageView::
get_string(std::size_t offset, std::size_t maxlen) const
{
    return elf_file_->get_string(offset_ + offset, maxlen);
}


ElfImage::
ElfImage(std::string const& filename)
: is_be_(false)
{
    // Use C fileio because C++ is broken when it comes to binary file I/O
    FILE* file = std::fopen(filename.c_str(), "rb");
    if (file == nullptr)
    {
        std::ostringstream ostr;
        ostr << "error " << errno << " opening '" << filename << "': " << std::strerror(errno);
        throw std::runtime_error(ostr.str());
    }

    if (std::fseek(file, 0, SEEK_END) == -1)
    {
        std::ostringstream ostr;
        ostr << "error " << errno << " positioning '" << filename << "': " << std::strerror(errno);
        fclose(file);
        throw std::runtime_error(ostr.str());
    }

    long fileSize = std::ftell(file);
    if (fileSize == -1)
    {
        std::ostringstream ostr;
        ostr << "error " << errno << " determining size of '" << filename << "': " << std::strerror(errno);
        fclose(file);
        throw std::runtime_error(ostr.str());
    }

    std::rewind(file);
    data_.resize(fileSize);
    std::size_t bytesRead = std::fread(data_.data(), sizeof(std::byte), fileSize, file);
    if (bytesRead != static_cast<std::size_t>(fileSize))
    {
        std::ostringstream ostr;
        ostr << "error " << errno << " reading '" << filename << "': " << std::strerror(errno);
        fclose(file);
        throw std::runtime_error(ostr.str());
    }

    fclose(file);
}


ElfImage::
ElfImage(ByteSequence const& byte_sequence)
: data_{byte_sequence}
, is_be_{false}
{
}


void ElfImage::
setBigEndian(bool isBigEndian)
{
    is_be_ = isBigEndian;
}


std::size_t ElfImage::
size() const
{
    return data_.size();
}


#ifdef DUDE_LATER_DUDE
std::string ElfImage::
md5() const
{
    MD5 md5hash;
    return md5hash(data_.data(), data_.size());
}


std::string ElfImage::
sha1() const
{
    SHA1 sha1hash;
    return sha1hash(data_.data(), data_.size());
}


std::string ElfImage::
sha256() const
{
    SHA256 sha256hash;
    return sha256hash(data_.data(), data_.size());
}
#endif


ElfImageView ElfImage::
view(std::size_t offset, std::size_t size) const
{
    if (offset > data_.size())
    {
        std::ostringstream ostr;
        ostr << "offset " << std::hex << offset
             << " is larger than filesize " << std::hex << data_.size();
        throw std::runtime_error(ostr.str());
    }
    std::size_t bytesLeft = data_.size() - offset;
        return ElfImageView(this, offset, (size < bytesLeft ? size : bytesLeft));
}


std::byte const* ElfImage::
get_bytes(std::size_t offset) const
{
    return &data_[offset];
}


std::uint8_t ElfImage::
get_uint8(std::size_t offset) const
{
    return std::to_integer<std::uint8_t>(data_[offset]);
}


std::uint16_t ElfImage::
get_uint16(std::size_t offset) const
{
    if (is_be_)
    {
        return (std::to_integer<std::uint16_t>(data_[offset+0]) << 8)
             | (std::to_integer<std::uint16_t>(data_[offset+1]) << 0);
    }
    return (std::to_integer<std::uint16_t>(data_[offset+0]) << 0)
         | (std::to_integer<std::uint16_t>(data_[offset+1]) << 8);
}


std::uint32_t ElfImage::
get_uint32(std::size_t offset) const 
{
    if (is_be_)
    {
        return (std::to_integer<std::uint32_t>(data_[offset+0])<<24)
             | (std::to_integer<std::uint32_t>(data_[offset+1])<<16)
             | (std::to_integer<std::uint32_t>(data_[offset+2])<<8)
             | (std::to_integer<std::uint32_t>(data_[offset+3])<<0);
    }
    return (std::to_integer<std::uint32_t>(data_[offset+0])<<0)
         | (std::to_integer<std::uint32_t>(data_[offset+1])<<8)
         | (std::to_integer<std::uint32_t>(data_[offset+2])<<16)
         | (std::to_integer<std::uint32_t>(data_[offset+3])<<24);
}


std::uint64_t ElfImage::
get_uint64(std::size_t offset) const
{
    if (is_be_)
    {
        return (std::to_integer<std::uint64_t>(data_[offset+0])<<56)
             | (std::to_integer<std::uint64_t>(data_[offset+1])<<48)
             | (std::to_integer<std::uint64_t>(data_[offset+2])<<40)
             | (std::to_integer<std::uint64_t>(data_[offset+3])<<32)
             | (std::to_integer<std::uint64_t>(data_[offset+4])<<24)
             | (std::to_integer<std::uint64_t>(data_[offset+5])<<16)
             | (std::to_integer<std::uint64_t>(data_[offset+6])<<8)
             | (std::to_integer<std::uint64_t>(data_[offset+7])<<0);
    }
    return (std::to_integer<std::uint64_t>(data_[offset+0])<<0)
         | (std::to_integer<std::uint64_t>(data_[offset+1])<<8)
         | (std::to_integer<std::uint64_t>(data_[offset+2])<<16)
         | (std::to_integer<std::uint64_t>(data_[offset+3])<<24)
         | (std::to_integer<std::uint64_t>(data_[offset+4])<<32)
         | (std::to_integer<std::uint64_t>(data_[offset+5])<<40)
         | (std::to_integer<std::uint64_t>(data_[offset+6])<<48)
         | (std::to_integer<std::uint64_t>(data_[offset+7])<<56);
}


std::string ElfImage::
get_string(std::size_t offset, std::size_t maxlen) const
{
    const char* b = reinterpret_cast<char const*>(&data_[offset]);
    std::size_t i = 0;
    while (i < maxlen && b[i] != '\0')
    {
        ++i;
    }
    return std::string(b, i);
}
