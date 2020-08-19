/**
 * ELF binary image wrapper
 */
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
#ifndef EDHELIND_ELFIMAGE_H
#define EDHELIND_ELFIMAGE_H

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>


class ElfImage;


/*!
 * A proxy object to access a subsection of the ELF file.
 *
 * An ElfImageView is effective a view proxy into an actial ElfImage object
 * starting at an offset from the beginning.  The ElfImageView knows all about
 * the target wordsize (32 or 64 bits) and the target endianness.  All of the
 * extractor functions (getXXX()) read from a relative offset from the beginning
 * of the view.
 *
 * This is a fairly flyweight object.
 */
class ElfImageView
{
public:
    ElfImageView() = default;
    ~ElfImageView() = default;

    /*!
     * Create a new sub-ElfImageView within this ElfImageView.
     * @param[in] offset  Offset of the new view from the start of this view
     * @param[in] size    SIze of the new view in bytes
     */
    ElfImageView
    view(std::size_t offset, std::size_t size) const;

    std::size_t
    size() const;

    /*! Get the raw bytes at @p offset */
    std::byte const*
    get_bytes(std::size_t offset) const;

    /*! Read the byte at @p offset as an unsigned 8-bit host-native integer */
    std::uint8_t
    get_uint8(std::size_t offset) const;

    /*! Read the 2 bytes at @p offset as an unsigned 16-bit host-native integer */
    std::uint16_t
    get_uint16(std::size_t offset) const;

    /*! Read the 4 bytes at @p offset as an unsigned 32-bit host-native integer */
    std::uint32_t
    get_uint32(std::size_t offset) const;

    /*! Read the 8 bytes at @p offset as an unsigned 64-bit host-native integer */
    std::uint64_t
    get_uint64(std::size_t offset) const;

    /*! Read bytes at the @p offset and interpret as a (zero-terminated) string */
    std::string
    get_string(std::size_t offset, std::size_t maxlen) const;

private:
    friend class ElfImage;

    /*!
     * Constructs a proxy into @p elfFile at offset @p offset.
     * @param[in] elfFile The ElfImage to create a view on
     * @param[in] offset  The number of bytes from the start of @p elfFile where
     *                    this view starts
     * @param[in] size    The size of the view
     */
    ElfImageView(ElfImage const* elfFile, std::size_t offset, std::size_t size);

private:
    ElfImage const* elf_file_;
    std::size_t     offset_;
    std::size_t     size_;
};


/*!
 * Wrap the actual ELF file image in a RAII object.
 *
 * This object stores the image of the ELF binary in target format. It provides
 * accessors to read from the image in host format.
 *
 * The target ELF file can be memory mapped or buffered (according to what's
 * available on the host platform) and access to it metered out through proxy
 * objects.
 */
class ElfImage
{
public:
    /*! Constructs an ElfImage from a named file */
    ElfImage(std::string const& filename);

    /*! Constructs an ElfImage from an istream object */
    ElfImage(std::basic_istream<std::byte>& istr);

    ElfImage(ElfImage const&) = delete;

    ElfImage& operator=(ElfImage const&) = delete;

    ~ElfImage() = default;

    void
    setBigEndian(bool isBigEndian);

    std::size_t
    size() const;

    std::string
    md5() const;

    std::string
    sha1() const;

    std::string
    sha256() const;

    /*! Get a proxy object to a subrange of the image. */
    ElfImageView
    view(std::size_t offset, std::size_t size) const;

    /*! Get the raw bytes at @p offset */
    std::byte const*
    get_bytes(std::size_t offset) const;

    /*! Read byte at @p offset as a host-order 8-bit integer */
    std::uint8_t
    get_uint8(std::size_t offset) const;

    /*! Read bytes at @p offset as a host-order 16-bit integer */
    std::uint16_t
    get_uint16(std::size_t offset) const;

    /*! Read bytes at @p offset as a host-order 32-bit integer */
    std::uint32_t
    get_uint32(std::size_t offset) const;

    /*! Read bytes at @p offset as a host-order 64-bit integer */
    std::uint64_t
    get_uint64(std::size_t offset) const;

    /*! Read bytes at @p offset and interpret as a (zero-terminated) string */
    std::string
    get_string(std::size_t offset, std::size_t maxlen) const;

private:
    void
    createFromStream(std::basic_istream<std::byte>& istr);

private:
    std::vector<std::byte> data_;
    bool                   is_be_;
};

#endif /* EDHELIND_ELFIMAGE_H */

