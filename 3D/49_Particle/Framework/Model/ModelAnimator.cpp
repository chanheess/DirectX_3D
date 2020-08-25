#include "Framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(Shader * shader)
	: shader(shader)
{
	model = new Model();

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		D3DXMatrixIdentity(&worlds[i]);

	instanceBuffer = new VertexBuffer(worlds, MAX_MODEL_INSTANCE, sizeof(Matrix), 1, true);

	frameBuffer = new ConstantBuffer(&tweenDesc, sizeof(TweenDesc) * MAX_MODEL_INSTANCE);
	sFrameBuffer = shader->AsConstantBuffer("CB_AnimationFrame");

	computeShader = new Shader(L"30_Collider.fx");
	computeAttachBuffer = new ConstantBuffer(&attachDesc, sizeof(AttachDesc));

	sSrv = computeShader->AsSRV("Input");
	sUav = computeShader->AsUAV("Output");
	sComputeAttachBuffer = computeShader->AsConstantBuffer("CB_Attach");
	sComputeFrameBuffer = computeShader->AsConstantBuffer("CB_AnimationFrame");

	for (int i = 0; i < MONSTERCOUNT; i++)
	{
		FrameCycle[i] = 0;
	}
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(computeShader);
	SafeDelete(computeBuffer);
	SafeDelete(computeAttachBuffer);


	for (Transform* transform : transforms)
		SafeDelete(transform);

	SafeDelete(model);
	SafeDelete(frameBuffer);
	SafeDelete(instanceBuffer);
	SafeDeleteArray(clipTransforms);

	SafeRelease(texture);
	SafeRelease(srv);
}

void ModelAnimator::Update()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		TweenDesc& desc = tweenDesc[i];
		ModelClip* clip = model->ClipByIndex(desc.Curr.Clip);

		//현재 애니메이션
		{	//애니메이션을 부드럽게해줌
			desc.Curr.RunningTime += Time::Delta();

			float time = 1.0f / clip->FrameRate() / desc.Curr.Speed;
			if (desc.Curr.Time >= 1.0f)
			{
				desc.Curr.RunningTime = 0.0f;

				desc.Curr.CurrFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
				desc.Curr.NextFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();

				if (desc.Curr.NextFrame == 0)
				{
					FrameCycle[i]++;
				}
			}
			desc.Curr.Time = desc.Curr.RunningTime / time;
		}

		//다음 애니메이션
		if (desc.Next.Clip > -1)
		{
			ModelClip* nextClip = model->ClipByIndex(desc.Next.Clip);

			desc.RunningTime += Time::Delta();
			desc.TweenTime = desc.RunningTime / desc.TakeTime;

			if (desc.TweenTime >= 1.0f)
			{
				desc.Curr = desc.Next;

				desc.Next.Clip = -1;
				desc.Next.CurrFrame = 0;
				desc.Next.NextFrame = 0;
				desc.Next.Time = 0;
				desc.Next.RunningTime = 0.0f;

				desc.RunningTime = 0.0f;
				desc.TweenTime = 0.0f;
			}
			else
			{
				desc.Next.RunningTime += Time::Delta();

				float time = 1.0f / nextClip->FrameRate() / desc.Next.Speed;
				if (desc.Next.Time >= 1.0f)
				{
					desc.Next.RunningTime = 0.0f;

					desc.Next.CurrFrame = (desc.Next.CurrFrame + 1) % nextClip->FrameCount();
					desc.Next.NextFrame = (desc.Next.CurrFrame + 1) % nextClip->FrameCount();
				}
				desc.Next.Time = desc.Next.RunningTime / time;
			}
		}
	}

	frameBuffer->Apply();
	if (computeBuffer != NULL)
	{
		computeAttachBuffer->Apply();
		sComputeAttachBuffer->SetConstantBuffer(computeAttachBuffer->Buffer());
		sComputeFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());

		sSrv->SetResource(computeBuffer->SRV());
		sUav->SetUnorderedAccessView(computeBuffer->UAV());

		computeShader->Dispatch(0, 0, 1, 1, 1);
		computeBuffer->Copy(csOutput, sizeof(CS_OutputDesc) * MAX_MODEL_TRANSFORMS);
	}

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelAnimator::Render()
{
	if (texture == NULL)
	{
		CreateTexture();
		CreateComputeDesc();
	}

	instanceBuffer->Render();


	frameBuffer->Apply();
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());

	for (ModelMesh* mesh : model->Meshes())
	{
		wstring name = mesh->Name();

		switch (equip)
		{
		case 0:
			if (name == L"defaultobject") continue;
			else if (name == L"sword01") continue;
			break;
		case 1:
			if (name == L"defaultobject") continue;
			break;
		case 2:
			if (name == L"sword01") continue;
			break;

		}
		
		mesh->Render(transforms.size());
	}
}

void ModelAnimator::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

void ModelAnimator::ReadMesh(wstring file)
{
	model->ReadMesh(file);

	for (ModelMesh* mesh : model->Meshes())
		mesh->SetShader(shader);	//쉐이더적용
}

void ModelAnimator::ReadClip(wstring file)
{
	model->ReadClip(file);
}

void ModelAnimator::PlayClip(UINT instance, UINT clip, float speed, float takeTime)
{
	if (tweenDesc[instance].Curr.Clip == clip) return;	//

	tweenDesc[instance].TakeTime = takeTime;
	tweenDesc[instance].Next.Clip = clip;
	tweenDesc[instance].Next.Speed = speed;
}

UINT ModelAnimator::StopClip(UINT instance, UINT clip)
{
	if (tweenDesc[instance].Curr.Clip == clip)
	{
		int cycling = FrameCycle[instance] * 2;
		if (cycling != 0)
		{
			FrameCycle[instance] = 0;
			return true;
		}
	}
	else
	{
		FrameCycle[instance] = 0;
	}

	return false;
}

void ModelAnimator::Equipment(UINT weaponNum)
{
	switch(weaponNum)
	{
	case 0:
		equip = 0;
		break;
	case 1:
		equip = 1;
		break;
	case 2:
		equip = 2;
		break;
	}
}

void ModelAnimator::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

Transform * ModelAnimator::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void ModelAnimator::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MODEL_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}

Matrix ModelAnimator::GetAttachTransform(UINT index, UINT indexOfBone)
{
	attachDesc.AttachBoneIndex = indexOfBone;

	if (csOutput == NULL)
	{
		Matrix temp;
		D3DXMatrixIdentity(&temp);

		return temp;
	}

	Matrix transform = model->BoneByIndex(attachDesc.AttachBoneIndex)->Transform();	//본의 transform
	Matrix result = csOutput[index].Result;	//움직인 본의 위치
	Matrix world = GetTransform(index)->World();	//이동한 만큼의 월드

	return transform * result * world;
}

void ModelAnimator::CreateTexture()
{
	clipTransforms = new ClipTransform[model->ClipCount()];
	for (UINT i = 0; i < model->ClipCount(); i++)
		CreateClipTransform(i);

	//Create Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;	//4픽셀로 한것	//4바이트씩 할당해서 총 16
		desc.Height = MAX_MODEL_KEYFRAMES;	//프레임 갯수
		desc.ArraySize = model->ClipCount();	//총개수
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// 본수정안함
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//텍스쳐를 쉐이더에다가 불러서 행렬로만듦
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		UINT pageSize = MAX_MODEL_TRANSFORMS * 4 * 16 * MAX_MODEL_KEYFRAMES;	//한페이지의 크기
		//void* p = malloc(pageSize * model->ClipCount());
		void* p = VirtualAlloc(NULL, pageSize * model->ClipCount(), MEM_RESERVE, PAGE_READWRITE);	//페이지 크기 할당

		//MEMORY_BASIC_INFORMATION memory;
		//VirtualQuery(p, &memory, sizeof(MEMORY_BASIC_INFORMATION));

		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			UINT start = c * pageSize;	//첫페이지의 시작크기

			for (UINT y = 0; y < MAX_MODEL_KEYFRAMES; y++)
			{
				void* temp = (BYTE *)p + MAX_MODEL_TRANSFORMS * y * sizeof(Matrix) + start;	//페이지의 시작 바이트수

				VirtualAlloc(temp, MAX_MODEL_TRANSFORMS * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);	//페이지 넣기
				memcpy(temp, clipTransforms[c].Transform[y], MAX_MODEL_TRANSFORMS * sizeof(Matrix));	//페이지에서 y한줄을 메모리 복사
			}
		}

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[model->ClipCount()];	//클립수만큼 동적할당
		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			void* temp = (BYTE *)p + c * pageSize;	//페이지의 시작주소

			subResource[c].pSysMem = temp;	//페이지의 시작주소를 넣음
			subResource[c].SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);	//한줄의 크기
			subResource[c].SysMemSlicePitch = pageSize;	//한페이지의 크기
		}
		Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));

		SafeDeleteArray(subResource);	//읽어온뒤 삭제
		VirtualFree(p, 0, MEM_RELEASE);	//alloc의 공간삭제
	}


	//CreateSRV
	{	//cpu가 gpu에게 이런형식으로 리소스를 전달할 것이다가 SRV (쉐이더 리소스 뷰)
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;	//2d이상일때 array가 붙음 TEXTURE2DARRAY
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = model->ClipCount();

		Check(D3D::GetDevice()->CreateShaderResourceView(texture, &desc, &srv));
	}

	for (ModelMesh* mesh : model->Meshes())	//mesh가 텍스처를 받아서 shader에 넘겨주기때문에 mesh에 텍스처를 넣어줌
		mesh->TransformsSRV(srv);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_MODEL_TRANSFORMS];

	ModelClip* clip = model->ClipByIndex(index);
	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);

			Matrix parent;
			Matrix invGlobal = bone->Transform();
			D3DXMatrixInverse(&invGlobal, NULL, &invGlobal);

			int parentIndex = bone->ParentIndex();
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];


			Matrix animation;
			ModelKeyframe* frame = clip->Keyframe(bone->Name());
			if (frame != NULL)
			{
				ModelKeyframeData& data = frame->Transforms[f];

				Matrix S, R, T;
				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);

				animation = S * R * T;
			}
			else
			{
				D3DXMatrixIdentity(&animation);
			}

			bones[b] = animation * parent;
			clipTransforms[index].Transform[f][b] = invGlobal * bones[b];
		}
	}
}

void ModelAnimator::CreateComputeDesc()
{
	UINT clipCount = model->ClipCount();
	UINT inSize = clipCount * MAX_MODEL_KEYFRAMES * MAX_MODEL_TRANSFORMS;
	UINT outSize = MAX_MODEL_TRANSFORMS;

	csInput = new CS_InputDesc[inSize];
	UINT count = 0;

	for (UINT clipIndex = 0; clipIndex < clipCount; clipIndex++)
	{
		for (UINT frameIndex = 0; frameIndex < MAX_MODEL_KEYFRAMES; frameIndex++)
		{
			for (UINT boneIndex = 0; boneIndex < MAX_MODEL_TRANSFORMS; boneIndex++)
			{
				csInput[count].Bone = clipTransforms[clipIndex].Transform[frameIndex][boneIndex];
				count++;
			}
		}//for(frameIndex)
	}//for(clipIndex)

	computeBuffer = new StructuredBuffer
	(
		csInput,
		sizeof(CS_InputDesc), inSize,
		sizeof(CS_OutputDesc), outSize
	);

	if (csOutput == NULL)
	{
		csOutput = new CS_OutputDesc[outSize];

		for (UINT i = 0; i < outSize; i++)
			D3DXMatrixIdentity(&csOutput[i].Result);
	}
}
