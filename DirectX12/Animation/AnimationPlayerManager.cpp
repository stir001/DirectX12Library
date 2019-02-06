#include "stdafx.h"
#include "AnimationPlayerManager.h"
#include "AnimationPlayer.h"

#include <Windows.h>

AnimationPlayerManager::AnimationPlayerManager() :mAnimations(), mIsThreadRun(true), mUpdate(&AnimationPlayerManager::CommonUpdate)
{
	mThreadObject = ((std::thread([](std::list<AnimatiomInfo>& animInfo) {
		for (auto& anim : animInfo)
		{
			if (anim.isPaly && (!anim.mAnimPlayer->mIsEnd || !anim.mAnimPlayer->mIsLoop))
			{
					anim.mAnimPlayer->Update();
			}
		}
	}, mAnimations)));
}

AnimationPlayerManager::~AnimationPlayerManager()
{
	if (mThreadObject.joinable())
	{
		mThreadObject.join();
	}
}

AnimationPlayerManager& AnimationPlayerManager::Instance()
{
	static AnimationPlayerManager* animplayer = nullptr;
	if (animplayer == nullptr)
	{
		animplayer = new AnimationPlayerManager();
	}
	return *animplayer;
}

int AnimationPlayerManager::SetAnimation(AnimationPlayer* anim)
{
	AnimatiomInfo animInfo;
	animInfo.id = FindMostSmallId();
	animInfo.mAnimPlayer = anim;
	animInfo.isPaly = true;
	mAnimations.push_back(animInfo);
	return animInfo.id;
}

void AnimationPlayerManager::StopAnimation(int id)
{
	if (id == -1)
	{
		return;
	}
	auto itr = FindAnimItr(id);
	itr->isPaly = false;
}

void AnimationPlayerManager::ReStartAnimation(int id)
{
	if (id == -1)
	{
		return;
	}
	auto itr = FindAnimItr(id);
	itr->isPaly = true;
}

void AnimationPlayerManager::UpdateAnimations()
{
	if (mIsThreadRun)
	{
		mThreadObject.detach();
		mIsThreadRun = false;
	}
	(this->*mUpdate)();
}

int AnimationPlayerManager::RemoveAnimation(int id)
{
	if (id == -1 || mAnimations.size() == 0)
	{
		return -1;
	}
	if (mIsThreadRun)
	{
		mThreadObject.join();
		mIsThreadRun = false;
	}
	auto itr = FindAnimItr(id);
	if (itr != mAnimations.end())
	{
		mAnimations.erase(itr);
	}
	return -1;
}

std::list<AnimationPlayerManager::AnimatiomInfo>::iterator AnimationPlayerManager::FindAnimItr(int id)
{
	if (id != -1)
	{
		for (auto itr = mAnimations.begin(); itr != mAnimations.end(); ++itr)
		{
			if (itr->id == id)
			{
				return itr;
			}
		}
	}
	return mAnimations.end();
}

int AnimationPlayerManager::FindMostSmallId()
{
	unsigned int id = 0;
	while(id < mAnimations.size())
	{
		unsigned int preID = id;
		for (auto& player : mAnimations)
		{
			if (player.id <= id)
			{
				id = player.id + 1;
				break;
			}
		}
		if (preID == id)
		{
			break;
		}
	}
	return id;
}

void AnimationPlayerManager::NonUpdate()
{
}

void AnimationPlayerManager::CommonUpdate()
{
	if (mAnimations.size() <= 0)
	{
		return;
	}
	mThreadObject = ((std::thread([](std::list<AnimatiomInfo>& animInfo) {
		std::list<std::thread> threadObject;
		for (auto& anim : animInfo)
		{
			if (anim.isPaly && (!anim.mAnimPlayer->mIsEnd || anim.mAnimPlayer->mIsLoop))
			{
				threadObject.push_back(std::thread([](AnimationPlayer* animPlayer) {
					animPlayer->Update();
				}, anim.mAnimPlayer));
			}
		}

		for (auto& t : threadObject)
		{
			t.join();
		}
	}, mAnimations)));
	mIsThreadRun = true;
}

void AnimationPlayerManager::WaitAnimation()
{
	if (mIsThreadRun)
	{
		if (mThreadObject.joinable()) {
			mThreadObject.join();
			mIsThreadRun = false;
		}
		//mResult.wait();
		mIsThreadRun = false;
	}
}

void AnimationPlayerManager::StopAllAnimation()
{
	mUpdate = &AnimationPlayerManager::NonUpdate;
}

void AnimationPlayerManager::ReStartAllAnimation()
{
	mUpdate = &AnimationPlayerManager::CommonUpdate;
}

