#include "stdafx.h"
#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer():mAnimationId(-1)
{
}


AnimationPlayer::~AnimationPlayer()
{
}

int AnimationPlayer::GetID() const
{
	return mAnimationId;
}

bool AnimationPlayer::IsEnd() const
{
	return mIsEnd;
}
