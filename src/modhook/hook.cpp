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

Hook* gHook = nullptr;

bool HashFile(std::filesystem::path path, uint32_t& hash)
{
  HANDLE handle =
      CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (handle == INVALID_HANDLE_VALUE)
  {
    return false;
  }

  LARGE_INTEGER size;
  if (GetFileSizeEx(handle, &size) == FALSE)
  {
    CloseHandle(handle);
    return false;
  }

  hash = 0xFFFFFFFFu;
  unsigned char buffer[0x1000];
  int64_t left = size.QuadPart;
  while (left > 0)
  {
    DWORD read;
    if (ReadFile(handle, buffer, sizeof(buffer), &read, NULL) == FALSE)
    {
      CloseHandle(handle);
      return false;
    }

    if (read == 0)
    {
      break;
    }

    hash = crc32(buffer, read, hash);
    left -= read;
  }

  CloseHandle(handle);
  return true;
}

std::filesystem::path GetExecutablePath()
{
  wchar_t buffer[MAX_PATH];
  auto length = GetModuleFileNameW(NULL, buffer, MAX_PATH);
  return std::filesystem::path(std::wstring_view(buffer, length));
}

void Error(const std::wstring_view message)
{
  auto text = std::wstring(message) + L"\n\nMods will not be enabled, sorry. :(";
  MessageBoxW(0, text.c_str(), L"Gibbed ModHook Error", 0);
}

void HookAttach()
{
  auto executable_path = GetExecutablePath();
  if (executable_path.empty() == true)
  {
    Error(L"Could not get game executable path.");
    return;
  }

  uint32_t hash;
  if (HashFile(executable_path, hash) == false)
  {
    Error(L"Could not hash game executable.");
    return;
  }

  Hook* hook = nullptr;
  switch (hash)
  {
    case 0xB5357A94:  // 2.0.0.0 Steam, original release
    case 0x076F8048:  // 2.0.0.0 Steam, original release, no ASLR
    {
      HookV0Config config = {};
      config.LoadBigFileTailAddress = 0x0049ED73;
      config.MSFileSystem.InstanceAddress = 0x00E3FBCC;
      config.AllocGlobals.InstanceAddress = 0x019ED7B0;
      config.ArchiveFileSystem.ConstructAddress = 0x0049E680;
      config.ArchiveFileSystem.LoadAddress = 0x0049E930;
      config.MultiFileSystem.InstanceAddress = 0x00E3FBC8;
      config.MultiFileSystem.AddFileSystemAddress = 0x0049F250;
      hook = CreateHookV0(config);
      break;
    }

    case 0x74E16208:  // 2.0.0.0 Steam, patch #1
    case 0xFBE1FC23:  // 2.0.0.0 GOG
    {
      HookV0Config config = {};
      config.LoadBigFileTailAddress = 0x0049EDD3;
      config.MSFileSystem.InstanceAddress = 0x00E3FBCC;
      config.AllocGlobals.InstanceAddress = 0x019ED7B0;
      config.ArchiveFileSystem.ConstructAddress = 0x0049E690;
      config.ArchiveFileSystem.LoadAddress = 0x0049E990;
      config.MultiFileSystem.InstanceAddress = 0x00E3FBC8;
      config.MultiFileSystem.AddFileSystemAddress = 0x0049F2F0;
      hook = CreateHookV0(config);
      break;
    }
  }

  if (hook == nullptr)
  {
    wchar_t message[64];
    auto message_length = _snwprintf_s(message, sizeof(message), L"Could not identify game version (%08X).", hash);
    Error(std::wstring_view(message, message_length));
    return;
  }

  if (hook->Install() == false)
  {
    Error(L"Failed to install hook.");
    delete hook;
    return;
  }

  gHook = hook;
}

void HookDetach()
{
  if (gHook != nullptr)
  {
    gHook->Uninstall();
    delete gHook;
  }
}
