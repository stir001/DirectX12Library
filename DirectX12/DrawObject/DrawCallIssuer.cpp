#include "stdafx.h"
#include "DrawCallIssuer.h"


DrawCallIssuer::DrawCallIssuer() : mInstanceNum(0U), mPreInstanceNum(0U)
{
}


DrawCallIssuer::~DrawCallIssuer()
{
}

unsigned int DrawCallIssuer::CountUpInstanceNum()
{
	return ++mInstanceNum;
}

unsigned int DrawCallIssuer::GetInstanceNum() const
{
	return mInstanceNum;
}

void DrawCallIssuer::ResetInstanceNum()
{
	mPreInstanceNum = mInstanceNum;
	mInstanceNum = 0U;
}

unsigned int DrawCallIssuer::GetPreInstanceNum() const
{
	return mPreInstanceNum;
}
