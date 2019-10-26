/***

instructions:

put your offsets here, however many you need.
uintptr_t offsets[] = { 0x00, 0x00, 0x00 };

the last number is the number of offsets added above.
rMemory<uintptr_t>(ac_proc, ac_ammo, offsets, 3);

if you don't need any offsets just set 'offsets' to NULL.

***/

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>

template<typename T>
T rMemory(HANDLE proc, uintptr_t adr, uintptr_t *offsets, uintptr_t num)
{

	T val;

	printf("number of offsets = %d\n", num);

	if (num == 0)
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
	}
	if (num == 1)
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
		ReadProcessMemory(proc, LPCVOID(val + offsets[0]), &val, sizeof(T), NULL);
	}
	else
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
		for (int i = 0; i < num; i++)
		{
			ReadProcessMemory(proc, LPCVOID(val + offsets[i]), &val, sizeof(T), NULL);
		}
	}
	return val;
}

template<typename T>
T wMemory(HANDLE proc, uintptr_t adr, uintptr_t *offsets, uintptr_t num, uintptr_t nval)
{

	T val;
	T baddr;
	T faddr;

	printf("number of offsets = %d\n", num);

	if (num == 0)
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
	}
	if (num == 1)
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
		ReadProcessMemory(proc, LPCVOID(val + offsets[0]), &val, sizeof(T), NULL);
	}
	else
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
		//printf("current mem value is %d\n", val);
		for (int i = 0; i < num; i++)
		{
			baddr = val;
			ReadProcessMemory(proc, LPCVOID(val + offsets[i]), &val, sizeof(T), NULL);
			faddr = baddr + offsets[i];
		}
	}

	if (num == 0)
	{
		unsigned long OldProtection;
		VirtualProtect((LPVOID)(adr), nval, PAGE_EXECUTE_READWRITE, &OldProtection);
		WriteProcessMemory(proc, LPVOID(adr), &nval, sizeof(T), NULL);
		VirtualProtect((LPVOID)(adr), nval, OldProtection, NULL);
	}
	else
	{
		unsigned long OldProtection;
		VirtualProtect((LPVOID)(faddr), nval, PAGE_EXECUTE_READWRITE, &OldProtection);
		WriteProcessMemory(proc, LPVOID(faddr), &nval, sizeof(T), NULL);
		VirtualProtect((LPVOID)(faddr), nval, OldProtection, NULL);
	}
	return 0;
}
