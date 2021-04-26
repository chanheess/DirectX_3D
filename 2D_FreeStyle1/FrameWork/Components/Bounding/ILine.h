#pragma once

struct ILine
{
	virtual void GetLine(vector<Vertex>* vertices, D3DXMATRIX world) = 0;
};