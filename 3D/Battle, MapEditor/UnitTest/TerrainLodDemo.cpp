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
			L"Terrain/Gray512_test.png",
			L"Terrain/Alpha512.png",
			1.0f, 16, 5
		};

		terrain = new TerrainLod(desc, true);
		terrain->BaseMap(L"Terrain/Dirt.png");
		//terrain->LayerMap(L"Terrain/Grass (Lawn).jpg", 0);
		terrain->NormalMap(L"Terrain/Dirt_Normal.png");
		//terrain->Pass(0);
		terrain->Pass(2);
	}

	//Billboard
	{
		billboardShader = new Shader(L"26_Billboard.fxo");

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
	}

	Models();

	Waters();
}

void TerrainLodDemo::Destroy()
{
	SafeDelete(terrain);
}

void TerrainLodDemo::Update()
{
	ImGui::Begin("Editor");
	{
		static UINT passTerrain = 0;
		ImGui::InputInt("TerrainPass", (int *)&passTerrain);
		passTerrain %= 3;

		terrain->Pass(passTerrain);
	}
	ImGui::End();

	terrain->Update();

	//billboard
	Billboarding();
	if (startBillboard == true)
	{
		billboard->Update();
	}

	//Model
	{
		//ModelCreate();
		//
		//for (int i = 0; i < models.size(); i++)
		//{
		//	models[i]->UpdateTransforms();
		//	models[i]->Update();
		//}
	}

	//Water
	{
		WaterCreate();

		for (int i = 0; i < water.size(); i++)
			water[i]->Update();
	}
}

void TerrainLodDemo::PreRender()
{
	//shadow->PreRender();
	//skyDome->PreRender();

	//Depth
	{
		/*shadow->PreRender();

		Pass(0, 1, 2);

		player->Render();
		monster->Render();
		rangeMonster->Render();*/

		//Model
		for (int i = 0; i < models.size(); i++)
		{
			models[i]->Render();
		}
	}
	//Reflection
	{
		for (int i = 0; i < water.size(); i++)
			water[i]->PreRender_Reflection();

		//skyDome->Pass(10, 11, 12);
		//skyDome->Render();

		/*Pass(13, 14, 15);

		player->Render();
		monster->Render();
		rangeMonster->Render();*/
	}

	//Refraction
	{
		for (int i = 0; i < water.size(); i++)
			water[i]->PreRender_Refraction();

		//skyDome->Pass(4, 5, 6);
		//skyDome->Render();

		/*Pass(7, 8, 9);

		player->Render();
		monster->Render();
		rangeMonster->Render();*/
	}
}

void TerrainLodDemo::Render()
{
	terrain->Render();

	//billboard
	if (startBillboard == true)
	{
		billboard->Render();
	}

	//Model
	//for (int i = 0; i < models.size(); i++)
	//{
	//	models[i]->Render();
	//}


	//Water
	for (int i = 0; i < water.size(); i++)
	{
		water[i]->Pass(16);
		water[i]->Render();
	}
}

void TerrainLodDemo::Billboarding()
{
	//Biilboard
	ImGui::Begin("Editor");
	{
		ImGui::Checkbox("Billboard", &billOn);
		if (billOn == true)
		{
			ImGui::Begin("Billboarded");
			{
				Vector3 position = terrain->GetPickedPosition();
				terrain->GetBrushDesc()->Location = position;
				
				static Vector2 scaled = { 1, 1 };

				ImGui::InputInt("Count", (int *)&count);
				count %= 21;

				ImGui::InputInt("TreeType", (int *)&treeType);
				treeType %= 5;

				terrain->GetBrushDesc()->Type = 1;

				ImGui::InputInt("Range", (int *)&terrain->GetBrushDesc()->Range);
				terrain->GetBrushDesc()->Range %= 21;

				if (scaled.x >= 20.0f)
					scaled.x = 20.0f;
				else if (scaled.x <= 1.0f)
					scaled.x = 1.0f;

				if (scaled.y >= 20.0f)
					scaled.y = 20.0f;
				else if (scaled.y <= 1.0f)
					scaled.y = 1.0f;

				ImGui::InputFloat2("Scale", scaled);

				static bool clickOn = false;
				ImGui::Checkbox("== ClickOn/Off : P ==", &clickOn);
				if (Keyboard::Get()->Down('P'))
				{
					if (clickOn == true)
						clickOn = false;
					else if (clickOn == false)
						clickOn = true;
				}

				if (Mouse::Get()->Down(0) && clickOn == true)
				{
					if (terrain->GetBrushDesc()->Type != 0)
					{
						if (terrain->GetBrushDesc()->Type == 1)
						{
							if (count != 0)
							{
								for (UINT i = 0; i < count; i++)
								{
									Vector2 scale;
									scale.x = Math::Random(scaled.x * 0.5f, scaled.x * 1.5f);
									scale.y = Math::Random(scaled.y * 0.7f, scaled.y * 1.7f);

									Vector3 positions;
									positions.x = Math::Random(position.x - terrain->GetBrushDesc()->Range, position.x + terrain->GetBrushDesc()->Range);
									positions.z = Math::Random(position.z - terrain->GetBrushDesc()->Range, position.z + terrain->GetBrushDesc()->Range);
									positions.y = terrain->GetHeight(position) + scale.y * 0.5f;

									billboard->Add(positions, scale, treeType);
								}
							}
						}
					}
					startBillboard = true;
				}

				if (ImGui::RadioButton("Erase", erase))
				{
					if (erase == true) erase = false;
					else erase = true;
				}
				if (erase == true)
				{
					count = 0;

					if (Mouse::Get()->Press(0))
					{
						for (int i = 0; i < billboard->SaveScale().size(); i++)
						{
							Vector3 posbill = billboard->SaveScale().at(i).Position;
							if (position.x + terrain->GetBrushDesc()->Range >= posbill.x && position.x - terrain->GetBrushDesc()->Range <= posbill.x &&
								position.z + terrain->GetBrushDesc()->Range >= posbill.z && position.z - terrain->GetBrushDesc()->Range <= posbill.z)
							{
								billboard->Erase(i);
							}
						}
					}
				}

				if (startBillboard == true)
					billboard->Update();

				std::wstring file;
				if (ImGui::Button("Save"))
				{
					Path::SaveFileDialog(file, Path::XmlFilter, L"../../_Save/", [=](std::wstring path) {

						auto billboardScale = billboard->SaveScale();

						string folder = String::ToString(Path::GetDirectoryName(L"../../_Save/"));
						string file = String::ToString(Path::GetFileName(path));

						Path::CreateFolders(folder);


						Xml::XMLDocument* document = new Xml::XMLDocument();

						Xml::XMLDeclaration* decl = document->NewDeclaration();
						document->LinkEndChild(decl);

						Xml::XMLElement* root = document->NewElement("Transforms");
						root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
						root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
						document->LinkEndChild(root);

						for (UINT i = 0; i < billboardScale.size(); i++)
						{
							Xml::XMLElement* node = document->NewElement("transform");
							root->LinkEndChild(node);

							Xml::XMLElement* element = NULL;

							element = document->NewElement("Count");
							element->SetAttribute("Count", billboardScale.at(i).Count);
							node->LinkEndChild(element);

							element = document->NewElement("Position");
							element->SetAttribute("X", billboardScale.at(i).Position.x);
							element->SetAttribute("Y", billboardScale.at(i).Position.y);
							element->SetAttribute("Z", billboardScale.at(i).Position.z);
							node->LinkEndChild(element);

							element = document->NewElement("Scale");
							element->SetAttribute("X", billboardScale.at(i).Scale.x);
							element->SetAttribute("Y", billboardScale.at(i).Scale.y);
							node->LinkEndChild(element);

						}

						document->SaveFile((folder + file).c_str());
						SafeDelete(document);
					});
				}
				ImGui::SameLine();
				if (ImGui::Button("Load"))
				{
					Path::OpenFileDialog(file, Path::XmlFilter, L"../../_Save/", [=](std::wstring path) {

						string folder = String::ToString(L"../../_Save/");
						string file = String::ToString(Path::GetFileName(path));

						//Xml 읽어지는 형식
						Xml::XMLDocument* document = new Xml::XMLDocument();   //이게 제일 첫번째 <?은 포함X
						Xml::XMLError error = document->LoadFile((folder + file).c_str());
						assert(error == Xml::XML_SUCCESS);

						Xml::XMLElement* root = document->FirstChildElement();   //document의 첫번째 element
						Xml::XMLElement* materialNode = root->FirstChildElement();   //root의 첫번째 element

						SafeDelete(billboard);
						billboard = new Billboard(billboardShader);
						billboard->AddTexture(L"Terrain/Tree.png");
						billboard->AddTexture(L"Terrain/Tree2.png");
						billboard->AddTexture(L"Terrain/Tree3.png");
						billboard->AddTexture(L"Terrain/Tree4.png");
						billboard->AddTexture(L"Terrain/Tree5.png");

						do {
							Material* material = new Material();   //불러서 저장

							Xml::XMLElement* node = NULL;   //빈 node 생성

							node = materialNode->FirstChildElement();   //node의 첫자식 ex) name
							//material->Name(String::ToWString(node->GetText()));
							wstring directory = Path::GetDirectoryName(String::ToWString(folder + file));
							String::Replace(&directory, L"../../_Save", L"");   //중첩될경우 제거

							Vector3 pos;
							Vector2 scale;
							UINT count;

							//여기부터....
							count = node->FloatAttribute("Count");
							node = node->NextSiblingElement();

							pos.x = node->FloatAttribute("X");
							pos.y = node->FloatAttribute("Y");
							pos.z = node->FloatAttribute("Z");

							node = node->NextSiblingElement();
							scale.x = node->FloatAttribute("X");
							scale.y = node->FloatAttribute("Y");

							billboard->Add(pos, scale, count);

							materialNode = materialNode->NextSiblingElement();
						} while (materialNode != NULL);
					});
				}
			}
			ImGui::End();
		}//billon

	}
	ImGui::End();
}

void TerrainLodDemo::Models()
{
	modelShader = new Shader(L"45_Scattering.fxo");

	for (UINT i = 0; i < ModelNumCount; i++)
		models.push_back(new ModelRender(modelShader));

	models[0]->ReadMaterial(L"Trees/Tree1");
	models[0]->ReadMesh(L"Trees/Tree1");

	models[1]->ReadMaterial(L"B787/Airplane");
	models[1]->ReadMesh(L"B787/Airplane");

	models[2]->ReadMaterial(L"Tower/Tower");
	models[2]->ReadMesh(L"Tower/Tower");
}

void TerrainLodDemo::ModelCreate()
{
	ImGui::Begin("Editor");
	{
		static bool modelOn = false;
		ImGui::Checkbox("ModelCreate", &modelOn);
		if (modelOn == true)
		{
			ImGui::Begin("ModelEditor");
			{
				//terrainPos
				{
					terrain->GetBrushDesc()->Location = terrain->GetPickedPosition();
					terrain->GetBrushDesc()->Type = 1;
					terrain->GetBrushDesc()->Range = 1;
				}

				static UINT modelNumber = 0;
				ImGui::InputInt("ModelNum", (int *)&modelNumber);
				modelNumber %= 4;


				static bool clickOn = false;
				static bool eraseOn = false;
				//OnButton
				{
					ImGui::Checkbox("== ClickOn/Off : P ==", &clickOn);
					if (Keyboard::Get()->Down('P'))
					{
						if (clickOn == true)
							clickOn = false;
						else if (clickOn == false)
						{
							clickOn = true;
						}
					}

					ImGui::Checkbox("== EraseOn/Off : O ==", &eraseOn);
					if (Keyboard::Get()->Down('O'))
					{
						if (eraseOn == true)
						{
							eraseOn = false;
						}
						else if (eraseOn == false)
						{
							eraseOn = true;
							clickOn = false;
						}
					}
				}

				//Create
				if (Mouse::Get()->Down(0) &&  clickOn == true)
				{
					eraseOn = false;

					Vector3 pos;
					pos = terrain->GetPickedPosition();
					pos.y = terrain->GetHeight(terrain->GetPickedPosition());
					Transform* transform = NULL;

					switch (modelNumber)
					{
					case 0:
						
						break;
					}



					transform = models[modelNumber]->AddTransform();
					transform->Position(pos);
					transform->Scale(0.01f, 0.01f, 0.01f);

					models[modelNumber]->UpdateTransforms();
					modelCount[modelNumber]++;
				}

				//Delete
				if (eraseOn == true)
				{
					if (Mouse::Get()->Press(0))
					{
						Vector3 position = terrain->GetPickedPosition();

						//for (int i = 0; i < models.size(); i++)
						//{
						//	Vector3 modelPos;
						//	models[i]->GetTransform()->Position(&modelPos);
						//	models[i]->GetTransform()
						//
						//	if (position.x + terrain->GetBrushDesc()->Range >= waterPos.x && position.x - terrain->GetBrushDesc()->Range <= waterPos.x &&
						//		position.z + terrain->GetBrushDesc()->Range >= waterPos.z && position.z - terrain->GetBrushDesc()->Range <= waterPos.z)
						//	{
						//		water.erase(water.begin() + i);
						//		waterCount--;
						//	}
						//}
					}
				}


				//ImGui::InputInt("Range", (int *)&terrain->GetBrushDesc()->Range);
			}
			ImGui::End();

		}
	}
	ImGui::End();

	for (ModelRender* temp : models)
		temp->Pass(8);
}

void TerrainLodDemo::ModelSave()
{
}

void TerrainLodDemo::ModelLoad()
{
}

void TerrainLodDemo::Waters()
{
	waterShader = new Shader(L"45_Scattering.fxo");
}

void TerrainLodDemo::WaterCreate()
{
	ImGui::Begin("Editor");
	{
		static bool waterOn = false;
		ImGui::Checkbox("WaterCreate", &waterOn);
		if (waterOn == true)
		{
			ImGui::Begin("WaterEditor");
			{
				terrain->GetBrushDesc()->Location = terrain->GetPickedPosition();
				terrain->GetBrushDesc()->Type = 1;

				ImGui::InputInt("Range", (int *)&terrain->GetBrushDesc()->Range);
				terrain->GetBrushDesc()->Range %= 129;

				static float posY = 0.0f;
				ImGui::InputFloat("PosY / ShiftCurrY", &posY);
				if (posY >= 255.0f)
					posY = 255.0f;
				if (posY <= -255.0f)
					posY = -255.0f;

				//Save Load
				{
					ImGui::InputInt("Save/LoadNum", (int *)&WaterSaveLoadNum);
					WaterSaveLoadNum %= 3;

					if (ImGui::Button("Save"))
					{
						switch (WaterSaveLoadNum)
						{
						case 0:
							WaterSave(L"Water/waterSaveFile");
							break;
						case 1:
							WaterSave(L"Water/waterSaveFile1");
							break;
						case 2:
							WaterSave(L"Water/waterSaveFile2");
							break;
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("Load"))
					{
						switch (WaterSaveLoadNum)
						{
						case 0:
							WaterLoad(L"Water/waterSaveFile");
							break;
						case 1:
							WaterLoad(L"Water/waterSaveFile1");
							break;
						case 2:
							WaterLoad(L"Water/waterSaveFile2");
							break;
						}
					}
				}

				static bool clickOn = false;
				static bool eraseOn = false;
				//OnButton
				{
					ImGui::Checkbox("== ClickOn/Off : P ==", &clickOn);
					if (Keyboard::Get()->Down('P'))
					{
						if (clickOn == true)
							clickOn = false;
						else if (clickOn == false)
						{
							clickOn = true;
						}
					}

					ImGui::Checkbox("== EraseOn/Off : O ==", &eraseOn);
					if (Keyboard::Get()->Down('O'))
					{
						if (eraseOn == true)
						{
							eraseOn = false;
						}
						else if (eraseOn == false)
						{
							eraseOn = true;
							clickOn = false;
						}
					}
				}
				
				//Create
				if (Mouse::Get()->Down(0) && terrain->GetBrushDesc()->Range >= 1 && clickOn == true)
				{
					eraseOn = false;
					water.push_back(new Water(waterShader, terrain->GetBrushDesc()->Range));
					Vector3 pos;
					pos = terrain->GetPickedPosition();
					if (Keyboard::Get()->Press(VK_LSHIFT))
						pos.y = terrain->GetHeight(terrain->GetPickedPosition()) + 0.5f + (waterCount * 0.1f);
					else
						pos.y = posY;

					water[waterCount]->GetTransform()->Position(pos);
					waterCount++;
				}
				//Delete
				if (eraseOn == true)
				{
					if (Mouse::Get()->Press(0))
					{
						Vector3 position = terrain->GetPickedPosition();

						for (int i = 0; i < water.size(); i++)
						{
							Vector3 waterPos;
							water[i]->GetTransform()->Position(&waterPos);

							if (position.x + terrain->GetBrushDesc()->Range >= waterPos.x && position.x - terrain->GetBrushDesc()->Range <= waterPos.x &&
								position.z + terrain->GetBrushDesc()->Range >= waterPos.z && position.z - terrain->GetBrushDesc()->Range <= waterPos.z)
							{
								water.erase(water.begin() + i);
								waterCount--;
							}
						}
					}
				}

			}
			ImGui::End();
		}
	}
	ImGui::End();
}

void TerrainLodDemo::WaterSave(wstring savePath)
{
	//WaterSave
	{
		savePath = L"../../_Textures/" + savePath + L".png";
		Path::CreateFolders(Path::GetDirectoryName(savePath));

		BinaryWriter* w = new BinaryWriter();
		w->Open(savePath);

		w->UInt(waterCount);

		for (UINT i = 0; i < water.size(); i++)
		{
			w->Int(water[i]->GetRadius());

			Vector3 pos;
			water[i]->GetTransform()->Position(&pos);
			w->Vector3(pos);
		}

		w->Close();
		SafeDelete(w);
	}
}

void TerrainLodDemo::WaterLoad(wstring savePath)
{
	//AlphaMap Load & Splatting Texture Load
	{
		savePath = L"../../_Textures/" + savePath + L".png";

		BinaryReader* r = new BinaryReader();
		r->Open(savePath);

		if (waterCount > 0)
			water.clear();

		waterCount = r->UInt();

		for (UINT i = 0; i < waterCount; i++)
		{
			water.push_back(new Water(waterShader, r->Int()));
			water[i]->GetTransform()->Position(r->Vector3());
		}
		r->Close();
		SafeDelete(r);
	}
}

