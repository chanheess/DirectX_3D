#include "stdafx.h"
#include "PostEffectsDemo.h"

void PostEffectsDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 32, -67);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);


	shader = new Shader(L"51_SpotLight.fxo");	
	gBuffer = new GBuffer(shader);
	
	depthStencil = new DepthStencil();
	viewport = new Viewport(D3D::Width(), D3D::Height());

	vector<wstring> files;
	Path::GetFiles(&files, L"../../_Shaders/", L"*.fx", false);

	for (wstring& temp : files)
	{
		String::Replace(&temp, L"../../_Shaders/", L"");
		String::Replace(&temp, L".fx", L"");

		if (String::StartsWith(temp, L"55_"))
		{
			String::Replace(&temp, L"55_", L"");
			effectList.push_back(temp);
		}
	}

	for (int i = 0; i < 30; i++)
		selectItems[i] = new char[MAX_PATH];

	sourceRTV = new RenderTarget();

	final2D = new Render2D();
	final2D->GetTransform()->Position(D3D::Width() * 0.5f, D3D::Height() * 0.5f, 0);
	final2D->GetTransform()->Scale(D3D::Width(), D3D::Height(), 1);

	source2D = new Render2D();
	source2D->GetTransform()->Position(200, 120, 0);
	source2D->GetTransform()->Scale(355.5f, 200, 1);
	source2D->SRV(sourceRTV->SRV());

	
	sky = new CubeSky(L"Environment/GrassCube1024.dds");
	snow = new Snow(Vector3(300, 100, 500), (UINT)1e+4, L"Environment/Snow.png");

	Mesh();
	Airplane();
	Kachujin();

	AddPointLights();
	AddSpotLights();
}

void PostEffectsDemo::Destroy()
{

}

void PostEffectsDemo::Update()
{
	OnGUI();
	OnValueGUI();

	sky->Update();

	sphere->Update();
	cylinder->Update();
	cube->Update();
	grid->Update();

	airplane->Update();
	kachujin->Update();

	snow->Update();

	final2D->Update();
	source2D->Update();
}

void PostEffectsDemo::PreRender()
{
	//PackGBuffer
	{
		gBuffer->PreRender();

		Pass(3, 4, 5);

		wall->Render();
		sphere->Render();

		brick->Render();
		cylinder->Render();

		stone->Render();
		cube->Render();

		floor->Render();
		grid->Render();

		airplane->Render();
		kachujin->Render();
	}

	viewport->RSSetViewport();

	//Main Render
	{
		sourceRTV->Set(depthStencil);

		sky->Render();

		gBuffer->Render(sourceRTV);

		snow->Render();
	}

	for(UINT i = 0; i < selectList.size(); i++)
	{
		ID3D11ShaderResourceView* srv = NULL;
		if (i == 0)
			srv = sourceRTV->SRV();
		else
			srv = selectList[i - 1].second->RenderTargetSRV(0);

		selectList[i].second->Srv(0, srv);
		selectList[i].second->PreRender(depthStencil);
		selectList[i].second->Render();
	}
}

void PostEffectsDemo::Render()
{
	if (selectList.size() > 0)
		final2D->SRV(selectList[selectList.size() - 1].second->RenderTargetSRV(0));
	else
		final2D->SRV(sourceRTV->SRV());

	final2D->Render();

	source2D->Render();
}

void PostEffectsDemo::PostRender()
{
	
}

void PostEffectsDemo::OnGUI()
{
	float windowWidth = 250;

	float width = D3D::Width() - 200;
	float height = D3D::Height();

	bool bOpen = true;
	bOpen = ImGui::Begin("PostEffect", &bOpen);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(windowWidth, height));

	//Draw EffectList	/Ãß°¡
	{
		ImGui::Separator();

		for (UINT i = 0; i < effectList.size();i++)
		{
			wstring name = effectList[i];

			if (ImGui::Button(String::ToString(name).c_str(), ImVec2(114, 0)))
			{
				name = L"55_" + name + L".fxo";
				PostEffect* effect = new PostEffect(name, gBuffer->Srvs());
				selectList.push_back(EffectPair(effectList[i], effect));
			}

			if (i % 2 == 0)
				ImGui::SameLine();
		}
	}

	
	//Draw SelectList
	{
		ImGui::Spacing();
		ImGui::Separator();

		for (UINT i = 0; i < 30; i++)
		{
			strcpy_s(selectItems[i], 1, "");

			if (i < selectList.size())
			{
				string temp = String::ToString(selectList[i].first).c_str();
				strcpy_s(selectItems[i], MAX_PATH, temp.c_str());
			}
		}

		ImGui::PushItemWidth(234);
		ImGui::ListBox("", (int *)&selectIndex, selectItems, selectList.size(), 20);
		ImGui::PushItemWidth(-1);
	}

	ImGui::Spacing();
	if (ImGui::Button("MoveUp", ImVec2(114, 0)))
	{
		if (selectIndex > 0)
		{
			swap(selectList[selectIndex - 1], selectList[selectIndex]);
			selectIndex--;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("MoveDown", ImVec2(114, 0)))
	{
		if (selectIndex < selectList.size() - 1)
		{
			swap(selectList[selectIndex + 1], selectList[selectIndex]);

			selectIndex++;
		}
	}

	if (ImGui::Button("Remove", ImVec2(114, 0)))
	{
		selectList.erase(selectList.begin() + selectIndex);

		if (selectIndex > 0)
			selectIndex--;
	}
	ImGui::SameLine();

	if (ImGui::Button("Clear ALL", ImVec2(114, 0)))
		selectList.clear();

	ImGui::End();
}

void PostEffectsDemo::OnValueGUI()
{

	float windowWidth = 300;

	float width = D3D::Width();
	float height = D3D::Height();

	bool bOpen = true;
	bOpen = ImGui::Begin("PostEffectValue", &bOpen);
	ImGui::SetWindowPos(ImVec2(width - windowWidth, 0));
	ImGui::SetWindowSize(ImVec2(windowWidth, height));

	PostEffect::ValueDesc& valueDesc = PostEffect::GetValues();

	if (ImGui::CollapsingHeader("Saturation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat("9_Value", &valueDesc.Saturation, 0.1f);
	}
	if (ImGui::CollapsingHeader("Sharpness", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat("10_Value", &valueDesc.Sharpness, 0.1f);
	}
	if (ImGui::CollapsingHeader("Vignette", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat("11_Power", &valueDesc.VignettePower, 0.01f);

		ImGui::InputFloat("11_ScaleX", &valueDesc.VignetteScale.x, 0.01f);
		ImGui::InputFloat("11_ScaleY", &valueDesc.VignetteScale.y, 0.01f);
	}
	if (ImGui::CollapsingHeader("Interace", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat("12_Strength", &valueDesc.InteraceStrength, 0.01f);
		ImGui::InputInt("12_Value", &valueDesc.InteraceValue);
	}
	if (ImGui::CollapsingHeader("Distortion", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat("13_LensPower", &valueDesc.LensPower, 0.01f);

		static float Distortion = -0.02f;
		ImGui::InputFloat("13_Distortion", &Distortion, 0.001f);

		static float ChromaticDistortion = -0.01f;
		ImGui::InputFloat("13_ChromaticDistortion", &ChromaticDistortion, 0.001f);

		valueDesc.Distortion = Vector3(Distortion, 0, 0);
		valueDesc.Distortion.y = Distortion + ChromaticDistortion;
		valueDesc.Distortion.z = Distortion + 2 * ChromaticDistortion;
	}
	if (ImGui::CollapsingHeader("Laplacian", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat("16_Detection", &valueDesc.Laplacian, 1.0f);
	}

	ImGui::End();
}

void PostEffectsDemo::Mesh()
{
	//Create Material
	{
		floor = new Material(shader);
		floor->DiffuseMap("Floor.png");
		floor->Specular(1, 1, 1, 15);
		floor->SpecularMap("Floor_Specular.png");
		floor->NormalMap("Floor_Normal.png");

		stone = new Material(shader);
		stone->DiffuseMap("Stones.png");
		stone->Specular(1, 1, 1, 2);
		stone->SpecularMap("Stones_Specular.png");
		stone->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		stone->NormalMap("Stones_Normal.png");
		

		brick = new Material(shader);
		brick->DiffuseMap("Bricks.png");
		brick->Specular(1, 0.3f, 0.3f, 2);
		brick->SpecularMap("Bricks_Specular.png");
		brick->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		brick->NormalMap("Bricks_Normal.png");
		

		wall = new Material(shader);
		wall->DiffuseMap("Wall.png");
		wall->Specular(1, 1, 1, 2);
		wall->SpecularMap("Wall_Specular.png");
		wall->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		wall->NormalMap("Wall_Normal.png");
	}

	//Create Mesh
	{
		Transform* transform = NULL;

		cube = new MeshRender(shader, new MeshCube());
		transform = cube->AddTransform();
		transform->Position(0, 5, 0);
		transform->Scale(20, 10, 20);

		grid = new MeshRender(shader, new MeshGrid(5, 5));
		transform = grid->AddTransform();
		transform->Position(0, 0, 0);
		//transform->Position(0, 5, 0);
		transform->Scale(12, 1, 12);

		cylinder = new MeshRender(shader, new MeshCylinder(0.5f, 3.0f, 20, 20));
		sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
		for (UINT i = 0; i < 5; i++)
		{
			transform = cylinder->AddTransform();
			transform->Position(-30, 6, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = cylinder->AddTransform();
			transform->Position(30, 6, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);


			transform = sphere->AddTransform();
			transform->Position(-30, 15.5f, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = sphere->AddTransform();
			transform->Position(30, 15.5f, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);
		}
	}

	sphere->UpdateTransforms();
	cylinder->UpdateTransforms();
	cube->UpdateTransforms();
	grid->UpdateTransforms();

	meshes.push_back(sphere);
	meshes.push_back(cylinder);
	meshes.push_back(cube);
	meshes.push_back(grid);
}

void PostEffectsDemo::Airplane()
{
	airplane = new ModelRender(shader);
	airplane->ReadMaterial(L"B787/Airplane");
	airplane->ReadMesh(L"B787/Airplane");

	Transform* transform = airplane->AddTransform();
	transform->Position(2.0f, 9.91f, 2.0f);
	transform->Scale(0.004f, 0.004f, 0.004f);
	airplane->UpdateTransforms();

	models.push_back(airplane);
}

void PostEffectsDemo::Kachujin()
{
	weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword");
	weapon->ReadMesh(L"Weapon/Sword");

	kachujin = new ModelAnimator(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
	kachujin->ReadClip(L"Kachujin/Hip_Hop_Dancing");

	Transform attachTransform;
	attachTransform.Position(10, -3.5f, -10);
	attachTransform.Scale(1.28f, 1.28f, 1.28f);

	kachujin->GetModel()->Attach(shader, weapon, 35, &attachTransform);


	Transform* transform = NULL;

	transform = kachujin->AddTransform();
	transform->Position(-25, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(0, 0, 0.1f);

	transform = kachujin->AddTransform();
	transform->Position(-10, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(1, 1, 1.0f);

	transform = kachujin->AddTransform();
	transform->Position(10, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(2, 2, 0.3f);

	transform = kachujin->AddTransform();
	transform->Position(25, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(3, 3, 0.75f);

	kachujin->UpdateTransforms();

	animators.push_back(kachujin);
}

void PostEffectsDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

void PostEffectsDemo::AddPointLights()
{
	PointLight light;
	light =
	{
		Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
		Color(0.0f, 0.0f, 1.0f, 1.0f), //Diffuse
		Color(0.0f, 0.0f, 0.7f, 1.0f), //Specular
		Color(0.0f, 0.0f, 0.7f, 1.0f), //Emissive
		Vector3(-25, 10, -30), 5.0f, 0.9f
	};
	Lights::Get()->AddPointLight(light);

	light =
	{
		Color(0.0f, 0.0f, 0.0f, 1.0f),
		Color(1.0f, 0.0f, 0.0f, 1.0f),
		Color(0.6f, 0.2f, 0.0f, 1.0f),
		Color(0.6f, 0.3f, 0.0f, 1.0f),
		Vector3(25, 10, -30), 10.0f, 0.3f
	};
	Lights::Get()->AddPointLight(light);


	for (float z = -30; z <= 30; z += 30)
	{
		for (float x = -30; x <= 30; x += 30)
		{
			light =
			{
				Color(0, 0, 0, 1),
				Math::RandomColor3(),
				Color(0, 0, 0, 1),
				Color(0, 0, 0, 1),
				Vector3(x, 1, z), 5, Math::Random(0.1f, 1.0f)
			};

			Lights::Get()->AddPointLight(light);
		}
	}
}

void PostEffectsDemo::AddSpotLights()
{
	SpotLight light;
	light =
	{
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Color(0.7f, 1.0f, 0.0f, 1.0f),
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Vector3(-10, 20, -30), 25.0f,
		Vector3(0, -1, 0), 30.0f, 0.9f
	};
	Lights::Get()->AddSpotLight(light);

	light =
	{
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Vector3(10, 20, -30), 30.0f,
		Vector3(0, -1, 0), 40.0f, 0.9f
	};
	Lights::Get()->AddSpotLight(light);
}
