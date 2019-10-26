/***

instructions:

put your offsets here, however many you need.
uintptr_t offsets[] = { 0x00, 0x00, 0x00 };

the last number is the number of offsets added above.
if you need to change a float, use <float>
rMemory<uintptr_t>(ac_proc, ac_ammo, offsets, 3);

if you don't need any offsets just set 'offsets' to NULL.

***/

#include <Windows.h>

template<typename T>
T rMemory(HANDLE proc, uintptr_t adr, uintptr_t *offsets, uintptr_t num)
{

	T val;
	
	if (num == 0)
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
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
T wMemory(HANDLE proc, uintptr_t adr, uintptr_t *offsets, uintptr_t num, T nval)
{

	uintptr_t val;
	uintptr_t baddr;
	uintptr_t faddr;

	if (num == 0)
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
	}
	else
	{
		ReadProcessMemory(proc, LPCVOID(adr), &val, sizeof(T), NULL);
		for (int i = 0; i < num; i++)
		{
			baddr = val;
			ReadProcessMemory(proc, LPCVOID(val + offsets[i]), &val, sizeof(T), NULL);
			faddr = baddr + offsets[i];
		}
	}

	if (num == 0)
	{
		unsigned long oProtection;
		VirtualProtectEx(proc, (LPVOID)(adr), nval, PAGE_EXECUTE_READWRITE, &oProtection);
		WriteProcessMemory(proc, LPVOID(adr), &nval, sizeof(T), NULL);
		VirtualProtectEx(proc, (LPVOID)(adr), nval, oProtection, NULL);
	}
	else
	{
		unsigned long oProtection;
		VirtualProtectEx(proc, (LPVOID)(faddr), nval, PAGE_EXECUTE_READWRITE, &oProtection);
		WriteProcessMemory(proc, LPVOID(faddr), &nval, sizeof(T), NULL);
		VirtualProtectEx(proc, (LPVOID)(faddr), nval, oProtection, NULL);
	}
	return 0;
}
