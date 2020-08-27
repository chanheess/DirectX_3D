#include "stdafx.h"
#include "ConvModel.h"
#include "Converter.h"

void ConvModel::Initialize()
{
	//Airplane();
	//Tower();
	//Kachujin();
	//Tank();

	//Weapons();
	//Monster();
	Player();
	//Trees();
}

void ConvModel::Airplane()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"B787/Airplane.fbx");
	conv->ExportMaterial(L"B787/Airplane", false);
	conv->ExportMesh(L"B787/Airplane");

	SafeDelete(conv);
}

void ConvModel::Tower()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tower/Tower.fbx");
	conv->ExportMaterial(L"Tower/Tower", false);
	conv->ExportMesh(L"Tower/Tower");

	SafeDelete(conv);
}

void ConvModel::Tank()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tank/Tank.fbx");
	conv->ExportMaterial(L"Tank/Tank", false);
	conv->ExportMesh(L"Tank/Tank");

	SafeDelete(conv);
}

void ConvModel::Kachujin()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Kachujin/Mesh.fbx");
	conv->ExportMaterial(L"Kachujin/Mesh", false);
	conv->ExportMesh(L"Kachujin/Mesh");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Idle.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Idle");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Running.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Running");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Jump.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Jump");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Hip_Hop_Dancing.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Hip_Hop_Dancing");
	SafeDelete(conv);
}

void ConvModel::Weapons()
{
	vector<wstring> names;
	names.push_back(L"Cutter.fbx");
	names.push_back(L"Cutter2.fbx");
	names.push_back(L"Dagger_epic.fbx");
	names.push_back(L"Dagger_small.fbx");
	names.push_back(L"Katana.fbx");
	names.push_back(L"LongArrow.obj");
	names.push_back(L"LongBow.obj");
	names.push_back(L"Rapier.fbx");
	names.push_back(L"Sword.fbx");
	names.push_back(L"Sword2.fbx");
	names.push_back(L"Sword_epic.fbx");

	for (wstring name : names)
	{
		Converter* conv = new Converter();
		conv->ReadFile(L"Weapon/" + name);


		String::Replace(&name, L".fbx", L"");
		String::Replace(&name, L".obj", L"");

		conv->ExportMaterial(L"Weapon/" + name, false);
		conv->ExportMesh(L"Weapon/" + name);
		SafeDelete(conv);
	}
}

void ConvModel::Player()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Player/Player.fbx");
	conv->ExportMaterial(L"Player/Player", false);
	conv->ExportMesh(L"Player/Player");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Idle.fbx");
	conv->ExportAnimClip(0, L"Player/Idle");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Move.fbx");
	conv->ExportAnimClip(0, L"Player/Move");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Sword_Melee_Attack.fbx");
	conv->ExportAnimClip(0, L"Player/Sword_Melee_Attack");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Sword_Attack_Idle.fbx");
	conv->ExportAnimClip(0, L"Player/Sword_Attack_Idle");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Melee_Punch.fbx");
	conv->ExportAnimClip(0, L"Player/Melee_Punch");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Death.fbx");
	conv->ExportAnimClip(0, L"Player/Death");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Forward Roll.fbx");
	conv->ExportAnimClip(0, L"Player/Forward Roll");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Standing_Aim_Recoil.fbx");
	conv->ExportAnimClip(0, L"Player/Standing_Aim_Recoil");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Standing_Aim_Overdraw.fbx");
	conv->ExportAnimClip(0, L"Player/Standing_Aim_Overdraw");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/Running.fbx");
	conv->ExportAnimClip(0, L"Player/Running");
	SafeDelete(conv);

	//arrow shoot move
	conv = new Converter();
	conv->ReadFile(L"Player/standing aim walk back.fbx");
	conv->ExportAnimClip(0, L"Player/standing aim walk back");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/standing aim walk forward.fbx");
	conv->ExportAnimClip(0, L"Player/standing aim walk forward");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/standing aim walk left.fbx");
	conv->ExportAnimClip(0, L"Player/standing aim walk left");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Player/standing aim walk right.fbx");
	conv->ExportAnimClip(0, L"Player/standing aim walk right");
	SafeDelete(conv);
}

void ConvModel::Monster()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Monster/Monster.fbx");
	conv->ExportMaterial(L"Monster/Monster", false);
	conv->ExportMesh(L"Monster/Monster");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Monster/LookAround_Idle.fbx");
	conv->ExportAnimClip(0, L"Monster/LookAround_Idle");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Monster/Monster Walk.fbx");
	conv->ExportAnimClip(0, L"Monster/Monster Walk");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Monster/Monster Attack.fbx");
	conv->ExportAnimClip(0, L"Monster/Monster Attack");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Monster/Monster Death.fbx");
	conv->ExportAnimClip(0, L"Monster/Monster Death");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Monster/Standing_Aim_Recoil.fbx");
	conv->ExportAnimClip(0, L"Monster/Standing_Aim_Recoil");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Monster/Standing_Aim_Overdraw.fbx");
	conv->ExportAnimClip(0, L"Monster/Standing_Aim_Overdraw");
	SafeDelete(conv);
}

void ConvModel::Trees()
{
	vector<wstring> names;
	names.push_back(L"Tree1.fbx");

	for (wstring name : names)
	{
		Converter* conv = new Converter();
		conv->ReadFile(L"Trees/" + name);

		String::Replace(&name, L".fbx", L"");
		String::Replace(&name, L".obj", L"");

		conv->ExportMaterial(L"Trees/" + name, false);
		conv->ExportMesh(L"Trees/" + name);
		SafeDelete(conv);
	}
}
