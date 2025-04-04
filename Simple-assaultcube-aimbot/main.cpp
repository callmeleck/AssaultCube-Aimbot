#include "includes.h"

namespace offsets
{
	constexpr auto entitylistptr = 0x18AC04;
	constexpr auto playerptraddress = 0x17E0A8;
	constexpr auto healthoffset = 0xEC;
	constexpr auto nameoffset = 0x205;
	constexpr auto entitiyheadpos = 0x4;
	constexpr auto viewanglepitch = 0x38;
	constexpr auto viewangleyaw = 0x34;

	constexpr auto entityspacing = 0x4;
}

int getFirstThreeDigits(int number) {
	while (number >= 1000) {
		number /= 10;
	}
	return number;
}

class headpos
{
public:
	float x = 0;
	float y = 0;
	float z = 0;
};
class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3()
	{
		x = y = z = 0;
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3& operator+(Vector3 arg)
	{
		x += arg.x;
		y += arg.y;
		z += arg.z;
		return *this;
	}
};

int main()
{
	headpos hp;
	headpos hpe;

	InitProcess("SDL_app");


	int smallnumhealth = 0;
	char smallnumname[32];
	int smallnumentityptr;
	float smallesnum('inf');

	double PI = 3.14159265358;

	char Entityname[32] = { 0 };
	int EntityListptr;
	int Entityptr;
	int entityhealth = 0;

	int localPlayerptr;
	float viewangleyaw = 0;
	float viewanglepitch = 0;

	DWORD baseAddres = GetModuleBaseAddress(pID, "ac_client.exe");

	ReadProccesMem((baseAddres + offsets::playerptraddress), &localPlayerptr, sizeof(localPlayerptr)); // get playerptr for offsets

	ReadProccesMem((baseAddres + offsets::entitylistptr), &EntityListptr, sizeof(EntityListptr)); // get entity list
	ReadProccesMem((EntityListptr + 0x4), &Entityptr, sizeof(Entityptr)); // read entity from list 

	int checkptr = getFirstThreeDigits(Entityptr);

	std::cout << R"(  

Assault Cube Aimbot

by Leck
		)" << std::endl;

	Sleep(2000);

	while (true)
	{
		float smallesnum('inf');

		for (int i = 0; i < 32; i++)
		{
			ReadProccesMem((EntityListptr + (i * offsets::entityspacing)), &Entityptr, sizeof(Entityptr)); // read entity from list 

			if (Entityptr <= 0) continue;

			int entityFirstThreeDigits = getFirstThreeDigits(Entityptr);

			if (entityFirstThreeDigits != checkptr) continue;

			ReadProccesMem((EntityListptr + (i * offsets::entityspacing)), &Entityptr, sizeof(Entityptr)); // pointer to entity from entity list

			ReadProccesMem((Entityptr + offsets::healthoffset), &entityhealth, sizeof(entityhealth)); // read health from entity 
			ReadProccesMem((Entityptr + offsets::nameoffset), &Entityname, sizeof(Entityname)); // get the name from entity

			ReadProccesMem((Entityptr + offsets::entitiyheadpos), &hpe, sizeof(hpe)); // get head pos of entity
			ReadProccesMem((localPlayerptr + offsets::entitiyheadpos), &hp, sizeof(hp)); //get localplayer head pos

			Vector3 deltaVec = { hpe.x - hp.x, hpe.y - hp.y, hpe.z - hp.z };

			float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

			if (entityhealth <= 100 && entityhealth > 0)
			{
				std::cout << "[Name]: " << Entityname << std::endl;
				std::cout << std::endl;
				std::cout << "[Health]: " << entityhealth << std::endl;
				std::cout << std::endl;
				std::cout << std::endl;
			}

			if (deltaVecLength < smallesnum)
			{
				smallesnum = deltaVecLength;
				memcpy(smallnumname, Entityname, sizeof(Entityname));
				smallnumentityptr = Entityptr;
				smallnumhealth = entityhealth;
			}

		}

		system("cls");

		ReadProccesMem((smallnumentityptr + offsets::entitiyheadpos), &hpe, sizeof(hpe)); // get head pos of entity from entity list
		ReadProccesMem((localPlayerptr + offsets::entitiyheadpos), &hp, sizeof(hp)); //get localplayer head pos

		Vector3 deltaVec = { hpe.x - hp.x, hpe.y - hp.y, hpe.z - hp.z };
		float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

		float pitch = asin(deltaVec.z / deltaVecLength) * (180 / PI);

		float yaw = atan2(deltaVec.x, -deltaVec.y) * (180 / PI);

		if (smallnumhealth < 0) 
		{
			continue;
		}
		else
		{
			if (GetAsyncKeyState(0x46) & 0x8000)
			{
				WriteProccesMem((localPlayerptr + offsets::viewanglepitch), &pitch, sizeof(pitch));
				WriteProccesMem((localPlayerptr + offsets::viewangleyaw), &yaw, sizeof(yaw));
			}
		}

	}
	return 0;
}
