#include "stdafx.h"
#include "BlurDemo.h"

void BlurDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 32, -67);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);


	shader = new Shader(L"51_SpotLight.fxo");	
	gBuffer = new GBuffer(shader);

	renderTarget[0] = new RenderTarget();
	renderTarget[1] = new RenderTarget();
	renderTarget[2] = new RenderTarget();
	
	depthStencil = new DepthStencil();
	viewport = new Viewport(D3D::Width(), D3D::Height());

	postEffect = new PostEffect(L"53_Blur.fxo", gBuffer->Srvs());

	render2D = new Render2D();
	render2D->GetTransform()->Position(200, 120, 0);
	render2D->GetTransform()->Scale(355.5f, 200, 1);
	render2D->SRV(renderTarget[0]->SRV());

	
	sky = new CubeSky(L"Environment/GrassCube1024.dds");
	snow = new Snow(Vector3(300, 100, 500), (UINT)1e+4, L"Environment/Snow.png");

	Mesh();
	Airplane();
	Kachujin();

	AddPointLights();
	AddSpotLights();
}

void BlurDemo::Destroy()
{

}

void BlurDemo::Update()
{
	//Blur
	{
		ImGui::Separator();

		static UINT BlurCount = 8;
		ImGui::InputInt("BlurCount", (int *)&BlurCount);
		BlurCount %= 32;
		postEffect->GetShader()->AsScalar("BlurCount")->SetInt(BlurCount);
	}

	//RadialBlur
	{
		ImGui::Separator();

		static UINT BlurCount = 8;
		ImGui::InputInt("RadialBlurCount", (int *)&BlurCount);
		BlurCount %= 32;
		postEffect->GetShader()->AsScalar("RadialBlurCount")->SetInt(BlurCount);

		static float Radius = 0.6f;
		ImGui::InputFloat("RadialBlurRadius", &Radius, 0.01f);
		postEffect->GetShader()->AsScalar("RadialBlurRadius")->SetFloat(Radius);

		static float Amount = 0.04f;
		ImGui::InputFloat("RadialBlurAmount", &Amount, 0.001f);
		postEffect->GetShader()->AsScalar("RadialBlurAmount")->SetFloat(Amount);


		static float CenterX = 0.5f;
		ImGui::InputFloat("CenterX", &CenterX, 0.01f);

		static float CenterY = 0.5f;
		ImGui::InputFloat("CenterY", &CenterY, 0.01f);
		postEffect->GetShader()->AsVector("RadialCenter")->SetFloatVector(Vector2(CenterX, CenterY));
	}
	


	sky->Update();

	sphere->Update();
	cylinder->Update();
	cube->Update();
	grid->Update();

	airplane->Update();
	kachujin->Update();

	snow->Update();

	postEffect->Update();
	render2D->Update();
}

void BlurDemo::PreRender()
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

	//Main Render
	{
		renderTarget[0]->Set(depthStencil);
		viewport->RSSetViewport();

		sky->Render();

		gBuffer->Render(renderTarget[0]);

		snow->Render();
	}

	////Accumulate
	//{
	//	//BlurX
	//	{
	//		renderTarget[1]->Set(depthStencil);
	//		viewport->RSSetViewport();

	//		postEffect->Srv(0, renderTarget[0]->SRV());
	//		postEffect->Technique(1);
	//		postEffect->Pass(0);
	//		postEffect->Render();
	//	}

	//	//BlurY
	//	{
	//		renderTarget[2]->Set(depthStencil);
	//		viewport->RSSetViewport();

	//		postEffect->Srv(1, renderTarget[1]->SRV());
	//		postEffect->Technique(1);
	//		postEffect->Pass(1);
	//		postEffect->Render();
	//	}
	//}
	
	//BlurCombine
	{
		RenderTarget* rtvs[2];
		rtvs[0] = renderTarget[1];
		rtvs[1] = renderTarget[2];

		RenderTarget::Sets(rtvs, 2, depthStencil);
		viewport->RSSetViewport();

		postEffect->Srv(0, renderTarget[0]->SRV());
		postEffect->Technique(1);
		postEffect->Pass(2);
		postEffect->Render();
	}
}

void BlurDemo::Render()
{
	postEffect->Technique(0);

	static UINT pass = postEffect->GetShader()->PassCount() - 1;
	ImGui::InputInt("Pass", (int *)&pass);
	pass %= shader->PassCount();
	postEffect->Pass(pass);
	
	//Accumulate
	{
		//postEffect->Srv(2, renderTarget[1]->SRV());
		//postEffect->Srv(2, renderTarget[2]->SRV());
	}
	
	//Combine
	{
		postEffect->Srv(1, renderTarget[1]->SRV());
		postEffect->Srv(2, renderTarget[2]->SRV());
	}
	
	postEffect->Render();
	render2D->Render();
}

void BlurDemo::PostRender()
{
	
}

void BlurDemo::Mesh()
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

void BlurDemo::Airplane()
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

void BlurDemo::Kachujin()
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

void BlurDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

void BlurDemo::AddPointLights()
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

void BlurDemo::AddSpotLights()
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
