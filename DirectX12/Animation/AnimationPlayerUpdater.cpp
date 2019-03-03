#include "stdafx.h"
#include "AnimationPlayerUpdater.h"
#include "AnimationPlayer.h"

#include <Windows.h>
#include <list>

AnimationPlayerUpdater::AnimationPlayerUpdater() :mAnimations(), mIsThreadRun(true), mUpdate(&AnimationPlayerUpdater::CommonUpdate)
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

AnimationPlayerUpdater::~AnimationPlayerUpdater()
{
	if (mThreadObject.joinable())
	{
		mThreadObject.join();
	}
}

AnimationPlayerUpdater& AnimationPlayerUpdater::Instance()
{
	static AnimationPlayerUpdater* animplayer = nullptr;
	if (animplayer == nullptr)
	{
		animplayer = new AnimationPlayerUpdater();
	}
	return *animplayer;
}

int AnimationPlayerUpdater::SetAnimation(AnimationPlayer* anim)
{
	AnimatiomInfo animInfo;
	animInfo.id = FindMostSmallId();
	animInfo.mAnimPlayer = anim;
	animInfo.isPaly = true;
	mAnimations[animInfo.id] = animInfo;
	return animInfo.id;
}

void AnimationPlayerUpdater::StopAnimation(int id)
{
	if (id == -1)
	{
		return;
	}
	auto itr = FindAnimItr(id);
	itr->second.isPaly = false;
}

void AnimationPlayerUpdater::ReStartAnimation(int id)
{
	if (id == -1)
	{
		return;
	}
	auto itr = FindAnimItr(id);
	itr->second.isPaly = true;
}

void AnimationPlayerUpdater::UpdateAnimations()
{
	if (mIsThreadRun)
	{
		mThreadObject.detach();
		mIsThreadRun = false;
	}
	(this->*mUpdate)();
}

int AnimationPlayerUpdater::RemoveAnimation(int id)
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

std::map<int,AnimationPlayerUpdater::AnimatiomInfo>::iterator AnimationPlayerUpdater::FindAnimItr(int id)
{
	if (id != -1)
	{
		return mAnimations.find(id);
	}
	return mAnimations.end();
}

int AnimationPlayerUpdater::FindMostSmallId()
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

void AnimationPlayerUpdater::NonUpdate()
{
}

void AnimationPlayerUpdater::CommonUpdate()
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

void AnimationPlayerUpdater::WaitAnimation()
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

void AnimationPlayerUpdater::StopAllAnimation()
{
	mUpdate = &AnimationPlayerUpdater::NonUpdate;
}

void AnimationPlayerUpdater::ReStartAllAnimation()
{
	mUpdate = &AnimationPlayerUpdater::CommonUpdate;
}

