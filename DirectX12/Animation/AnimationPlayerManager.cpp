#include "stdafx.h"
#include "AnimationPlayerManager.h"
#include "AnimationPlayer.h"

#include <Windows.h>
#include <list>

AnimationPlayerManager::AnimationPlayerManager() :mAnimations(), mIsThreadRun(true), mUpdate(&AnimationPlayerManager::CommonUpdate)
{
	mThreadObject = ((std::thread([](std::map<int,AnimatiomInfo>& animInfo) {
		for (auto& anim : animInfo)
		{
			if (anim.second.isPaly && (!anim.second.mAnimPlayer->mIsEnd || !anim.second.mAnimPlayer->mIsLoop))
			{
					anim.second.mAnimPlayer->Update();
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
	mAnimations[animInfo.id] = animInfo;
	return animInfo.id;
}

void AnimationPlayerManager::StopAnimation(int id)
{
	if (id == -1)
	{
		return;
	}
	auto itr = FindAnimItr(id);
	itr->second.isPaly = false;
}

void AnimationPlayerManager::ReStartAnimation(int id)
{
	if (id == -1)
	{
		return;
	}
	auto itr = FindAnimItr(id);
	itr->second.isPaly = true;
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

std::map<int,AnimationPlayerManager::AnimatiomInfo>::iterator AnimationPlayerManager::FindAnimItr(int id)
{
	if (id != -1)
	{
		return mAnimations.find(id);
	}
	return mAnimations.end();
}

int AnimationPlayerManager::FindMostSmallId()
{
	int id = 0;
	bool isFind = false;
	while(id < mAnimations.size() && !isFind)
	{
		isFind = true;
		for (auto& player : mAnimations)
		{
			if (player.first <= id)
			{
				++id;
				for (auto& p : mAnimations)
				{
					if (p.first == id)
					{
						isFind = false;
						break;
					}
				}
				break;
			}
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
	mThreadObject = ((std::thread([](std::map<int,AnimatiomInfo>& animInfo) {
		std::list<std::thread> threadObject;
		for (auto& anim : animInfo)
		{
			if (anim.second.isPaly && (!anim.second.mAnimPlayer->mIsEnd || anim.second.mAnimPlayer->mIsLoop))
			{
				threadObject.push_back(std::thread([](AnimationPlayer* animPlayer) {
					animPlayer->Update();
				}, anim.second.mAnimPlayer));
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

