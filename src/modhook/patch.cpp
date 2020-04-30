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

bool ReadData(LPVOID address, LPVOID data, DWORD length)
{
  DWORD old, junk;
  if (VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &old) == FALSE)
  {
    return false;
  }
  memcpy(data, address, length);
  VirtualProtect(address, length, old, &junk);
  return true;
}

bool PatchData(LPVOID address, LPCVOID data, DWORD length)
{
  DWORD old, junk;
  if (VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &old) == FALSE)
  {
    return false;
  }
  memcpy(address, data, length);
  VirtualProtect(address, length, old, &junk);
  return true;
}

bool PatchCode(LPVOID address, LPCVOID data, DWORD length)
{
  if (PatchData(address, data, length) == false)
  {
    return false;
  }

  if (FlushInstructionCache(GetCurrentProcess(), address, length) == FALSE)
  {
    return false;
  }

  return true;
}

bool PatchJumpInternal(LPVOID address, unsigned char opcode, LPVOID target, UnpatchJumpData* unpatch)
{
  unsigned char jump[5];

  jump[0] = opcode;
  *(DWORD*)(&jump[1]) = (DWORD)target;

  if (unpatch != NULL)
  {
    unpatch->address = address;
    ReadData(address, unpatch->code, 5);
  }

  return PatchCode(address, jump, 5);
}

bool PatchCall(LPVOID address, LPCVOID target) { return PatchCall(address, target, NULL); }

bool PatchCall(LPVOID address, LPCVOID target, UnpatchJumpData* unpatch)
{
  return PatchJumpInternal(address, 0xE8, (LPVOID)((unsigned char*)target - (unsigned char*)address - 5), unpatch);
}

bool PatchJump(LPVOID address, LPCVOID target) { return PatchJump(address, target, NULL); }

bool PatchJump(LPVOID address, LPCVOID target, UnpatchJumpData* unpatch)
{
  return PatchJumpInternal(address, 0xE9, (LPVOID)((unsigned char*)target - (unsigned char*)address - 5), unpatch);
}

void UnpatchJump(UnpatchJumpData* unpatch) { PatchCode(unpatch->address, unpatch->code, 5); }
