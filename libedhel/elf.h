/**
 * Edhelind standard ELF definitions
 *
 * This elf.h is written specifically for the Edhelind project based on both the
 * various ELF specification docuemnets and by examining several ELF
 * implementations.
 *
 * The Edhelind project is intended to be built and run on a number of popular
 * hosts, most of which have no native ELF headers, and targeting any ELF binary
 * for any system. To that end, it is better to have a project-owned screened
 * version of the elf.h file.
 *
 * Because Eldhelind is written in C++, these ELF definitions are implemented
 * using C++ idioms instead of traditional C idioms as documented in the various
 * ELF specifications.  Some of the names may deviate from the well-known C
 * symbols but are kept as close as practical.
 *
 * Instead of using all the wacky ELF typedefs for fixed-size types, the C++
 * standard names for the fixed-size types are used instead. Giving ELF-specific
 * names to well-named things adds no value but does add cost.
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
#ifndef EDHELIND_ELF_H
#define EDHELIND_ELF_H

#include <cstddef>
#include <cstdint>


/**
 * @defgroup ELF file header
 * @{
 */

/** The magic that identifies an ELF file */
constexpr std::uint8_t elf_magic[] = { '\177', 'E', 'L', 'F' };

/** Classification of internal srtucture sizes */
enum class ehi_class: std::uint8_t
{
    ELFCLASSNONE = 0,   /**< invalid class */
    ELFCLASS32 = 1,     /**< 32-bit objects */
    ELFCLASS64 = 2,     /**< 64-bit objects */
};

/** The byte ordering of words within the file */
enum class ehi_data: std::uint8_t
{
    ELFDATANONE = 0,    /**< invalid data encoding */
    ELFDATA2LSB = 1,    /**< twos-complement little-endian */
    ELFDATA2MSB = 2,    /**< twos-complement big-endian */
};

enum class ehi_version: std::uint8_t
{
    EV_NONE = 0,
    EV_CURRENT = 1,
};

/** The target operating system */
enum class ehi_osabi: std::uint8_t
{
    EO_SYSV = 0,        /**< AT&T System V */
    EO_HPUX = 1,        /**< Hewlett-Packaerd HP-UX */
    EO_NETBSD = 2,      /**< NetBSD */
    EO_GNU = 3,
    EO_HURD = 4,        /**< GNU/HURD */
    EO_86OPEN = 5,
    EO_SOLARIS = 6,     /**< Sun Solsaris */
    EO_AIX = 7,         /**< IBM AIX */
    EO_IRIX = 8,        /**< Silicon Graphics IRIX */
    EO_FREEBSD = 8,     /**< FreeBSD */
    EO_TRU64 = 10,      /**< Digital/Compaq Tru64 UNIX */
    EO_MODESTO = 11,    /**< Novell Modesto */
    EO_OPENBSD = 12,    /**< Open BSD */
    EO_OPENVMS = 13,    /**< Open VMS */
    EO_NSK = 14,        /**< Hewlett-Packard Non-Stop Kernel */
    EO_AROS = 15,       /**< Amiga Research OS */
    EO_FENIXOS = 16,
    EO_ARM_AEABI = 64,
    EO_ARM = 97,
    EO_STANDALONE = 255 /** Embedded (standalone) */
};


/** Intended use of the ELF file */
enum class eh_type: std::uint16_t
{
    ET_NONE = 0,        /**< no file type */
    ET_REL = 1,         /**< relocatable (object) file */
    ET_EXEC = 2,        /**< (loadable) executable file */
    ET_DYN = 3,         /**< shared object file */
    ET_CORE = 4,        /**< core dump file */
};

/** Target machine architecture */
enum class eh_machine: std::uint16_t
{
    EM_NONE = 0,        /**< no machine */
    EM_M32 = 1,         /**< AT&T WE 32100 */
    EM_SPARC = 2,       /**< SPARC */
    EM_386 = 3,         /**< Intel 80386 */
    EM_68K = 4,         /**< Motorola 68000 */
    EM_88K = 5,         /**< Motorola 88000 */
    EM_860 = 7,         /**< Intel 80860 */
    EM_MIPS = 8,        /**< MIPS I */
    EM_PPC = 20,        /**< PowerPC */
    EM_PPC64 = 21,      /**< 64-bit PowerPC */
    EM_S390 = 21,       /**< IBM System/390 */
    RM_ARM = 40,        /**< Advanced Risc Machines ARMv7 */
    EM_X86_64 = 62,     /**< AMD x86_64 */
    EM_MMIX = 80,       /**< Knuth's educational 64-bit processor */
    EM_AARCH64 = 183,   /**< ARM8v2 64-bit (aarch64) */
    EM_AVR32 = 185,     /**< Atmel 32-bit microporcessor family */
    EM_CUDA = 190,      /**< nVidia CUDA */
    EM_RISCV = 243,     /**< RISC-V */
};

struct Elf32_Ehdr {
    std::uint8_t  ei_magic[4];
    ehi_class     ei_class;
    ehi_data      ei_data;
    ehi_version   ei_version;
    ehi_osabi     ei_osabi;
    std::byte     ei_pad[8];
    eh_type       e_type;
    eh_machine    e_machine;
    std::uint32_t e_version;
    std::uint32_t e_entry;  /* Entry point */
    std::uint32_t e_phoff;
    std::uint32_t e_shoff;
    std::uint32_t e_flags;
    std::uint16_t e_ehsize;
    std::uint16_t e_phentsize;
    std::uint16_t e_phnum;
    std::uint16_t e_shentsize;
    std::uint16_t e_shnum;
    std::uint16_t e_shstrndx;
};
static_assert(sizeof(Elf32_Ehdr) == 52, "invalid 32-bit ELF header size");

struct Elf64_Ehdr {
    std::uint8_t  ei_magic[4];
    ehi_class     ei_class;
    ehi_data      ei_data;
    ehi_version   ei_version;
    ehi_osabi     ei_osabi;
    std::byte     ei_pad[8];
    eh_type       e_type;
    eh_machine    e_machine;
    std::uint32_t e_version;
    std::uint64_t e_entry;		/* Entry point virtual address */
    std::uint64_t e_phoff;		/* Program header table file offset */
    std::uint64_t e_shoff;		/* Section header table file offset */
    std::uint32_t e_flags;
    std::uint16_t e_ehsize;
    std::uint16_t e_phentsize;
    std::uint16_t e_phnum;
    std::uint16_t e_shentsize;
    std::uint16_t e_shnum;
    std::uint16_t e_shstrndx;
};
static_assert(sizeof(Elf64_Ehdr) == 64, "invalid 64-bit ELF header size");

/** @} */

/**
 * @defgroup Section header
 * @{
 */

enum class s_type: std::uint32_t
{
    SHT_NULL           = 0,  /*!< unused */
    SHT_PROGBITS       = 1,  /*!< program data */
    SHT_SYMTAB         = 2,  /*!< symbol table */
    SHT_STRTAB         = 3,  /*!< a string table */
    SHT_RELA           = 4,  /*!< relocation entries with addends */
    SHT_HASH           = 5,  /*!< symbol hash table */
    SHT_DYNAMIC        = 6,  /*!< information for dynamic linking */
    SHT_NOTE           = 7,  /*!< additional notes */
    SHT_NOBITS         = 8,  /*!< section occupying no space */
    SHT_REL            = 9,  /*!< relocation entires without addends */
    SHT_SHLIB          = 10,  /*!< reserved */
    SHT_DYNSYM         = 11,  /*!< symbol table */
    SHT_INIT_ARRAY     = 14,  /*!< array of constructors */
    SHT_FINI_ARRAY     = 15,  /*!< array of destructors */
    SHT_PREINIT_ARRAY  = 16,  /*!< array of pre-constructors */
    SHT_GROUP          = 17,  /*!< section group definitions */
    SHT_SYMTAB_SHNDX   = 18,  /*!< extended section numbering */
    SHT_NUM            = 19,  /*!< number of defined types */
    SHT_LOOS           = 0x60000000,  /*!< start of OS-specific range */
    SHT_QNXREL         = 0x60000000,  /*!< QNX4 relocation table */
    SHT_GNU_ATTRIBUTES = 0x6ffffff5,
    SHT_GNU_HASH       = 0x6ffffff6,  /*!< GNU-style hash table */
    SHT_GNU_LIBLIST    = 0x6ffffff7,  /*!< prelink library list */
    SHT_GNU_CHECKSUM   = 0x6ffffff8,  /*!< checksum for DSO */
    SHT_GNU_VERDEF     = 0x6ffffffd,  /*!< version definition section */
    SHT_GNU_VERNEED    = 0x6ffffffe,  /*!< version needs section */
    SHT_GNU_VERSYM     = 0x6fffffff,  /*!< version symbol table */
    SHT_HIOS           = 0x6fffffff,  /*!< end of OS-specific range */
    SHT_LOPROC         = 0x70000000,  /*!< start of processor-specific range */
    SHT_ARM_EXIDX      = 0x70000001,  /*!< ARM exception index table */
    SHT_ARM_PREEMPMAP  = 0x70000002,  /*!< ARM dynamic linking preemption map */
    SHT_ARM_ATTRIBUTES = 0x70000003,  /*!< ARM file compatibility attributes */
    SHT_HIPROC         = 0x7fffffff,  /*!< end of processor-specific range */
    SHT_LOUSER         = 0x80000000,
    SHT_HIUSER         = 0xffffffff
};

enum section_flags
{
    SHF_WRITE       = (1 << 0),   /*!< section should be writable during execution */
    SHF_ALLOC       = (1 << 1),   /*!< section occupies memory during execution */
    SHF_EXECINSTR   = (1 << 2),   /*!< section containes executable instructions */
    SHF_MERGE       = (1 << 3),   /*!< section may be merged to prevent duplication */
    SHF_STRINGS     = (1 << 4),   /*!< section contains null-terminated character strings */
    SHF_GROUP       = (1 << 9),   /*!< section is a member of a group */
    SHF_TLS         = (1 << 10),  /*!< section holds thread-local storage */
    SHF_COMPRESSED  = (1 << 11),  /*!< section is compressed */
    SHF_MASKOS      = 0x0ff00000, /*!< OS-specific flags mask */
    SHF_MASKPROC    = 0xf0000000  /*!< processor-specific flags mask */
};

struct Elf32_Shdr
{
    std::uint32_t sh_name;
    s_type        sh_type;
    std::uint32_t sh_flags;
    std::uint32_t sh_addr;
    std::uint32_t sh_offset;
    std::uint32_t sh_size;
    std::uint32_t sh_link;
    std::uint32_t sh_info;
    std::uint32_t sh_addralign;
    std::uint32_t sh_entsize;
};
static_assert(sizeof(Elf32_Shdr) == 40, "invalid 32-bit section header size");

struct Elf64_Shdr
{
    std::uint32_t sh_name;
    s_type        sh_type;
    std::uint64_t sh_flags;
    std::uint64_t sh_addr;
    std::uint64_t sh_offset;
    std::uint64_t sh_size;
    std::uint32_t sh_link;
    std::uint32_t sh_info;
    std::uint64_t sh_addralign;
    std::uint64_t sh_entsize;
};
static_assert(sizeof(Elf64_Shdr) == 64, "invalid 64-bit section header size");

#endif /* EDHELIND_ELF_H */

