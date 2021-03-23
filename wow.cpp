#include "HackProcess.h" 
entry wow;
int myTeam;

bool keyPressed;

DWORD Pointer;
 
struct Structure
{   
	float r;
    float g;
    float b;
    float a;
    bool bo;
    bool bwo;
};

/* TO DO: add color customization */

Structure CBlue = { 0.2f, 0.4f, 0.7f, 1.f, false, true };
Structure CGreen ={ 0.f, 1.f, 0.f, 1.f, false, true };
Structure CYellow = { 1.f, 1.f, 0.f, 1.f, false, true };
Structure CRed ={ 1.f, 0.f, 0.f, 1.f, false, true };

struct players_t
{
    DWORD Entity;
    int Team;
    float Coordinates[3];
    int Inactive;
    int Health;
 
    void ReadPlayerInfo(int Player)
    {
        ReadProcessMemory(wow.__no, (PBYTE*)(wow.__Client + 0x04A1D3A4 + (Player * 16)),
            &Entity, sizeof(DWORD), 0);
        ReadProcessMemory(wow.__no, (PBYTE*)(Entity + 0xF0), &Team, sizeof(int), 0);
		ReadProcessMemory(wow.__no, (PBYTE*)(Entity + 0xE9), &Inactive, sizeof(int), 0);
        ReadProcessMemory(wow.__no, (PBYTE*)(Entity + 0x134), &Coordinates, sizeof(float[3]), 0);
		ReadProcessMemory(wow.__no, (PBYTE*)(Entity + 0xFC), &Health, sizeof(int), 0);
 
 
    }
}myPlayerList[32];
 
void DrawGlow(int Index, Structure Color)
{
    bool one = true;
	bool two = false;

    /* don't forget to add color customization */

    WriteProcessMemory(wow.__no, (PBYTE*)((Pointer + ((Index * 0x34) + 0x4))), &Color.r, sizeof(float), 0);
	WriteProcessMemory(wow.__no, (PBYTE*)((Pointer + ((Index * 0x34) + 0x8))), &Color.g, sizeof(float), 0);
    WriteProcessMemory(wow.__no, (PBYTE*)((Pointer + ((Index * 0x34) + 0xC))), &Color.b, sizeof(float), 0);
	WriteProcessMemory(wow.__no, (PBYTE*)((Pointer + ((Index * 0x34) + 0x10))), &Color.a, sizeof(float), 0);
	WriteProcessMemory(wow.__no, (PBYTE*)((Pointer + ((Index * 0x34) + 0x24))), &one, sizeof(bool), 0);
	WriteProcessMemory(wow.__no, (PBYTE*)((Pointer + ((Index * 0x34) + 0x25))), &two, sizeof(bool), 0);
}
 
 
void main()
{
 
    wow.runProcess();
 
    for (;;) // or for(;;)
    {
        for (int i = 0; i < 32; i++)
        {
            myPlayerList[i].ReadPlayerInfo(i);
            DWORD me;
            ReadProcessMemory(wow.__no, (PBYTE*)(wow.__Client + 0xA7AFBC), &me, sizeof(DWORD), 0);
            ReadProcessMemory(wow.__no, (PBYTE*)(me + 0xF0), &myTeam, sizeof(int), 0);
 
 
            if (GetAsyncKeyState(VK_INSERT)) 
            {
                keyPressed = !keyPressed;
                Sleep(200);
            }
 
            if (keyPressed)
            {
                DWORD g_playercurent;
                int g_playercurentteam;
                int g_playercurentglowindex;
 
 
                ReadProcessMemory(wow.__no, (PBYTE*)(wow.__Client + 0x04A1D3A4 + ((i)* 16)), &g_playercurent, sizeof(DWORD), 0);
				ReadProcessMemory(wow.__no, (PBYTE*)(wow.__Client + 0x04B303A4), &Pointer, sizeof(DWORD), 0);
				ReadProcessMemory(wow.__no, (PBYTE*)(g_playercurent + 0xF0), &g_playercurentteam, sizeof(int), 0);
				ReadProcessMemory(wow.__no, (PBYTE*)(g_playercurent + 0x1DCC), &g_playercurentglowindex, sizeof(int), 0);
                if (myPlayerList[i].Inactive)
                    continue;
 
                if (myPlayerList[i].Team == myTeam)
                {
                }
                else
                {
					if (Health>= 75)
					{
					 DrawGlow(g_playercurentglowindex, CGreen);
					};
					if ((Health >= 35) && (Health < 75))
					{
					 DrawGlow(g_playercurentglowindex, CYellow);
					};
					if (Health < 35)
					{
					 DrawGlow(g_playercurentglowindex, CRed);
					};  
				}
            }
        }
        Sleep(1);
 
    }
}