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

#include "stdafx.hpp"

#include "cdc_v0.hpp"

template <typename T>
void __forceinline Resolve(HMODULE module, uint32_t offset, T& value)
{
  auto rva = offset - 0x00400000u;
  auto va = reinterpret_cast<uint32_t>(module);
  value = reinterpret_cast<T>(va + rva);
}

class HookV0 : public Hook
{
 public:
  HookV0(const HookV0Config& config);
  ~HookV0() override;

  bool Install(void) override;
  void Uninstall(void) override;

  void LoadModFileSystems(void);

 private:
  bool LoadArchiveFileSystem(const char* path);

  struct
  {
    bool is_available;
    void* LoadBigFileTail;
    struct
    {
      cdc::MSFileSystem** Instance;
    } MSFileSystem;
    struct
    {
      cdc::AllocGlobals** Instance;
    } AllocGlobals;
    struct
    {
      cdc::ArchiveFileSystem::Construct Construct;
      cdc::ArchiveFileSystem::Load Load;
    } ArchiveFileSystem;
    struct
    {
      cdc::MultiFileSystem** Instance;
      cdc::MultiFileSystem::AddFileSystem AddFileSystem;
    } MultiFileSystem;
  } game_;

  bool is_installed_;
  UnpatchJumpData unpatch_;
};

HookV0* gHook = nullptr;

void __declspec(naked) MyLoadModFileSystemsThunk()
{
  __asm
  {
      mov ecx, [gHook]
      call HookV0::LoadModFileSystems
      ret
  }
}

HookV0::HookV0(const HookV0Config& config) : game_({}), is_installed_(false), unpatch_({})
{
  auto module = GetModuleHandleW(nullptr);
  if (module != nullptr)
  {
#define RESOLVE(x) Resolve(module, config.x##Address, game_.x)
    RESOLVE(LoadBigFileTail);
    RESOLVE(MSFileSystem.Instance);
    RESOLVE(AllocGlobals.Instance);
    RESOLVE(ArchiveFileSystem.Construct);
    RESOLVE(ArchiveFileSystem.Load);
    RESOLVE(MultiFileSystem.Instance);
    RESOLVE(MultiFileSystem.AddFileSystem);
#undef RESOLVE
    game_.is_available = true;
  }
}

HookV0::~HookV0() { this->Uninstall(); }

void __stdcall LoadModFileSystemsV0(HookV0* hook) { hook->LoadModFileSystems(); }

bool HookV0::Install()
{
  if (is_installed_ == false)
  {
    is_installed_ = PatchJump(game_.LoadBigFileTail, MyLoadModFileSystemsThunk, &unpatch_);
  }
  return is_installed_;
}

void HookV0::Uninstall()
{
  if (is_installed_ == true)
  {
    UnpatchJump(&unpatch_);
    is_installed_ = false;
  }
}

void HookV0::LoadModFileSystems(void)
{
  WIN32_FIND_DATAA fileData;
  HANDLE handle = FindFirstFileA("mods\\*.000", &fileData);
  if (handle != INVALID_HANDLE_VALUE)
  {
    do
    {
      auto path = std::string("mods\\") + std::string(fileData.cFileName, strlen(fileData.cFileName));
      this->LoadArchiveFileSystem(path.c_str());
    } while (FindNextFileA(handle, &fileData) == TRUE);
    FindClose(handle);
  }
}

bool HookV0::LoadArchiveFileSystem(const char* path)
{
  if (game_.is_available == false)
  {
    return false;
  }

  auto msfs = *game_.MSFileSystem.Instance;
  auto allocGlobals = *game_.AllocGlobals.Instance;
  auto multifs = *game_.MultiFileSystem.Instance;

  if (msfs == nullptr || allocGlobals == nullptr || multifs == nullptr)
  {
    return false;
  }

  auto heapAllocator = allocGlobals->DefaultHeapAllocator;
  if (heapAllocator == nullptr)
  {
    return false;
  }

  if (msfs->Exists(path) == false)
  {
    return false;
  }

  void* memory = heapAllocator->Allocate(104, 0);
  if (memory == nullptr)
  {
    return false;
  }

  auto archivefs = game_.ArchiveFileSystem.Construct(memory, msfs);
  if (game_.ArchiveFileSystem.Load(archivefs, path, 0) == false)
  {
    archivefs->Free(true);
    return false;
  }

  game_.MultiFileSystem.AddFileSystem(multifs, archivefs, false, true);
  return true;
}

Hook* CreateHookV0(const HookV0Config& config)
{
  if (gHook != nullptr)
  {
    gHook->Uninstall();
    delete gHook;
  }
  gHook = new HookV0(config);
  return gHook;
}
