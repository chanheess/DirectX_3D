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
