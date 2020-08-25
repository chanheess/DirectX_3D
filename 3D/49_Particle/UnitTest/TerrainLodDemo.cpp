#include "stdafx.h"
#include "TerrainLodDemo.h"
#include "Environment/Terrain.h"
#include "Utilities/Xml.h"
#include "Utilities/BinaryFile.h"

void TerrainLodDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 10, -30);
	((Freedom *)Context::Get()->GetCamera())->Speed(100, 2);


	shader = new Shader(L"47_TerrainLod.fxo");

	//Terrain
	{
		TerrainLod::InitializeDesc desc =
		{
			shader,
			L"Terrain/Gray512.png",
			1.0f, 16, 5
		};

		terrain = new TerrainLod(desc);
		terrain->BaseMap(L"Terrain/Dirt.png");
		terrain->LayerMap(L"Terrain/Grass (Lawn).jpg", L"Terrain/Gray512.png");
		terrain->NormalMap(L"Terrain/Dirt_Normal.png");
		//terrain->Pass(0);
		terrain->Pass(1);
	}

	//Billboard
	/*{
		billboardShader = new Shader(L"26_Billboard.fx");

		billboard = new Billboard(billboardShader);
		billboard->AddTexture(L"Terrain/Tree.png");
		billboard->AddTexture(L"Terrain/Tree2.png");
		billboard->AddTexture(L"Terrain/Tree3.png");
		billboard->AddTexture(L"Terrain/Tree4.png");
		billboard->AddTexture(L"Terrain/Tree5.png");
		startBillboard = false;
		treeType = 0;
		count = 0;
		erase = false;
	}*/
}

void TerrainLodDemo::Destroy()
{
	SafeDelete(terrain);
}

void TerrainLodDemo::Update()
{
	terrain->Update();

	//Biilboard
	//{
	//	Vector3 position = terrain->GetPickedPosition();

	//	ImGui::InputInt("Count", (int *)&count);
	//	count %= 20;

	//	ImGui::InputInt("TreeType", (int *)&treeType);
	//	treeType %= 5;

	//	if (Mouse::Get()->Down(0) && position != Vector3(-20, -20, -20))
	//	{
	//		//type과 range값을 가져옴	
	//		UINT type = terrain->OutType();
	//		UINT range = terrain->OutRange();

	//		if (type != 0)
	//		{
	//			if (type == 1)
	//			{
	//				if (count != 0)
	//				{
	//					for (UINT i = 0; i < count; i++)
	//					{
	//						Vector2 scale;
	//						scale.x = Math::Random(3.0f, 8.0f);
	//						scale.y = Math::Random(3.0f, 10.0f);

	//						Vector3 positions;
	//						positions.x = Math::Random(position.x - range, position.x + range);
	//						positions.z = Math::Random(position.z - range, position.z + range);
	//						positions.y = terrain->GetHeight(position) + scale.y * 0.5f;

	//						billboard->Add(positions, scale, treeType);
	//					}
	//				}
	//			}
	//			else if (type == 2)
	//			{
	//				if (count != 0)
	//				{
	//					//float dx = position.x - range;
	//					//float dz = position.z - range;
	//					//float dist = sqrt(dx * dx + dz * dz);

	//					for (UINT i = 0; i < count; i++)
	//					{
	//						Vector2 scale;
	//						scale.x = Math::Random(3.0f, 8.0f);
	//						scale.y = Math::Random(3.0f, 10.0f);

	//						Vector3 positions;
	//						//if (dist <= range)
	//						//{
	//						//	positions.x = Math::Random(position.x - range, position.x + range);
	//						//}
	//						positions.x = Math::Random(position.x - range, position.x + range);
	//						positions.z = Math::Random(position.z - range, position.z + range);
	//						positions.y = terrain->GetHeight(position) + scale.y * 0.5f;

	//						billboard->Add(position, scale, treeType);
	//					}
	//				}
	//			}

	//		}
	//		startBillboard = true;
	//	}

	//	if (ImGui::RadioButton("Erase", erase))
	//	{
	//		if (erase == true) erase = false;
	//		else erase = true;
	//	}
	//	if (erase == true)
	//	{
	//		count = 0;
	//		UINT range = terrain->OutRange();
	//		bool press = false;

	//		if (Mouse::Get()->Press(0))
	//			press = true;

	//		if (press == true)
	//		{
	//			for (int i = 0; i < billboard->SaveScale().size(); i++)
	//			{
	//				Vector3 posbill = billboard->SaveScale().at(i).Position;
	//				if (position.x + range >= posbill.x && position.x - range <= posbill.x &&
	//					position.z + range >= posbill.z && position.z - range <= posbill.z)
	//				{
	//					billboard->Erase(i);
	//				}
	//			}
	//		}
	//	}

	//	if (startBillboard == true)
	//		billboard->Update();

	//	std::wstring file;
	//	if (ImGui::Button("Save"))
	//	{
	//		Path::SaveFileDialog(file, Path::XmlFilter, L"../../_Save/", [=](std::wstring path) {

	//			auto billboardScale = billboard->SaveScale();

	//			string folder = String::ToString(Path::GetDirectoryName(L"../../_Save/"));
	//			string file = String::ToString(Path::GetFileName(path));

	//			Path::CreateFolders(folder);


	//			Xml::XMLDocument* document = new Xml::XMLDocument();

	//			Xml::XMLDeclaration* decl = document->NewDeclaration();
	//			document->LinkEndChild(decl);

	//			Xml::XMLElement* root = document->NewElement("Transforms");
	//			root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	//			root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	//			document->LinkEndChild(root);

	//			for (UINT i = 0; i < billboardScale.size(); i++)
	//			{
	//				Xml::XMLElement* node = document->NewElement("transform");
	//				root->LinkEndChild(node);

	//				Xml::XMLElement* element = NULL;

	//				element = document->NewElement("Count");
	//				element->SetAttribute("Count", billboardScale.at(i).Count);
	//				node->LinkEndChild(element);

	//				element = document->NewElement("Position");
	//				element->SetAttribute("X", billboardScale.at(i).Position.x);
	//				element->SetAttribute("Y", billboardScale.at(i).Position.y);
	//				element->SetAttribute("Z", billboardScale.at(i).Position.z);
	//				node->LinkEndChild(element);

	//				element = document->NewElement("Scale");
	//				element->SetAttribute("X", billboardScale.at(i).Scale.x);
	//				element->SetAttribute("Y", billboardScale.at(i).Scale.y);
	//				node->LinkEndChild(element);

	//			}

	//			document->SaveFile((folder + file).c_str());
	//			SafeDelete(document);
	//		});
	//	}

	//	if (ImGui::Button("Load"))
	//	{
	//		Path::OpenFileDialog(file, Path::XmlFilter, L"../../_Save/", [=](std::wstring path) {

	//			string folder = String::ToString(L"../../_Save/");
	//			string file = String::ToString(Path::GetFileName(path));

	//			//Xml 읽어지는 형식
	//			Xml::XMLDocument* document = new Xml::XMLDocument();   //이게 제일 첫번째 <?은 포함X
	//			Xml::XMLError error = document->LoadFile((folder + file).c_str());
	//			assert(error == Xml::XML_SUCCESS);

	//			Xml::XMLElement* root = document->FirstChildElement();   //document의 첫번째 element
	//			Xml::XMLElement* materialNode = root->FirstChildElement();   //root의 첫번째 element

	//			SafeDelete(billboard);
	//			billboard = new Billboard(billboardShader);
	//			billboard->AddTexture(L"Terrain/Tree.png");
	//			billboard->AddTexture(L"Terrain/Tree2.png");
	//			billboard->AddTexture(L"Terrain/Tree3.png");
	//			billboard->AddTexture(L"Terrain/Tree4.png");
	//			billboard->AddTexture(L"Terrain/Tree5.png");

	//			do {
	//				Material* material = new Material();   //불러서 저장

	//				Xml::XMLElement* node = NULL;   //빈 node 생성

	//				node = materialNode->FirstChildElement();   //node의 첫자식 ex) name
	//				//material->Name(String::ToWString(node->GetText()));
	//				wstring directory = Path::GetDirectoryName(String::ToWString(folder + file));
	//				String::Replace(&directory, L"../../_Save", L"");   //중첩될경우 제거

	//				Vector3 pos;
	//				Vector2 scale;
	//				UINT count;

	//				//여기부터....
	//				count = node->FloatAttribute("Count");
	//				node = node->NextSiblingElement();

	//				pos.x = node->FloatAttribute("X");
	//				pos.y = node->FloatAttribute("Y");
	//				pos.z = node->FloatAttribute("Z");

	//				node = node->NextSiblingElement();
	//				scale.x = node->FloatAttribute("X");
	//				scale.y = node->FloatAttribute("Y");

	//				billboard->Add(pos, scale, count);

	//				materialNode = materialNode->NextSiblingElement();
	//			} while (materialNode != NULL);
	//		});
	//	}
	//}
}

void TerrainLodDemo::Render()
{
	terrain->Render();
}