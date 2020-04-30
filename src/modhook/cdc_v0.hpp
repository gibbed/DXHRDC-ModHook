/* Copyright (c) 2020 Rick (rick 'at' gibbed 'dot' us)
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 */

#ifndef __CDC_HPP
#define __CDC_HPP

#include <cstdint>

#pragma pack(push, 4)

namespace cdc
{
struct AllocGlobals;
struct GameHeapAllocator;
struct MultiFileSystem;
struct ArchiveFileSystem;

struct AllocGlobals
{
  virtual void __undefined0();

  char __data0[68];
  GameHeapAllocator* DefaultHeapAllocator;
};

struct GameHeapAllocator
{
  virtual void __undefined0();
  virtual void __undefined1();
  virtual void __undefined2();
  virtual void __undefined3();
  virtual void __undefined4();
  virtual void __undefined5();
  virtual void __undefined6();
  virtual void __undefined7();
  virtual void __undefined8();
  virtual void __undefined9();
  virtual void __undefined10();
  virtual void __undefined11();
  virtual void __undefined12();
  virtual void* Allocate(int bytes, int unk1);
  virtual void __undefined14();
  virtual void __undefined15();
  virtual void __undefined16();
  virtual void __undefined17();
  virtual void __undefined18();
  virtual void __undefined19();
  virtual void __undefined20();
  virtual void __undefined21();
  virtual void __undefined22();
  virtual void __undefined23();
  virtual void __undefined24();
  virtual void __undefined25();
  virtual void __undefined26();
};

struct FileSystem
{
  virtual void __undefined0();
  virtual void __undefined1();
  virtual bool Exists(const char* path) = 0;
  virtual void __undefined3();
  virtual void __undefined4();
  virtual void __undefined5();
  virtual void __undefined6();
  virtual void __undefined7();
  virtual void __undefined8();
  virtual void __undefined9();
  virtual void __undefined10();
  virtual void __undefined11();
  virtual void __undefined12();
  virtual void __undefined13();
  virtual void __undefined14();
  virtual void __undefined15();
  virtual void __undefined16();
  virtual void __undefined17();
  virtual void __undefined18();
  virtual void __undefined19();
  virtual void __undefined20();
  virtual void __undefined21();
  virtual void __undefined22();
  virtual void __undefined23();
  virtual void Free(bool unk1);
};

struct FileSystemLayer : FileSystem
{
};

struct MSFileSystem : FileSystem
{
};

struct MultiFileSystem : FileSystem
{
  typedef ArchiveFileSystem*(__thiscall* AddFileSystem)(MultiFileSystem* self, FileSystem* fs, bool unk2, bool unk3);
};

struct ArchiveFileSystem : FileSystemLayer
{
  uint8_t __undefined[104];

  typedef ArchiveFileSystem*(__thiscall* Construct)(void* self, FileSystem* parent);
  typedef bool(__thiscall* Load)(ArchiveFileSystem* self, const char* path, int archiveIndex);
};

}  // namespace cdc

#pragma pack(pop)

#endif
