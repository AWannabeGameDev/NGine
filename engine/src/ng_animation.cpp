#include <stdexcept>

#include "ngine/renderer/animation.hpp"

ng::Animation::Animation(size_t maxFrames)
{
	_frames.reserve(maxFrames);
}

void ng::Animation::addFrame(const ImageSampleData& frame, float duration)
{
	if(_frames.size() == _frames.capacity())
	{
		throw std::out_of_range {"Maximum number of frames cannot be exceeded."};
	}

	_frames.emplace_back(frame, duration);
}

void ng::Animation::setOnEnd(const std::function<void()>& onEndFunction)
{
	_onEnd = onEndFunction;
}

void ng::Animation::setLoop(bool loop)
{
	_loop = loop;
}

void ng::Animation::start(size_t initialFrame)
{
	_active = true;
	_currentFrameIdx = initialFrame;
	_timeSinceCurrentFrame = 0.0f;
}

void ng::Animation::resume()
{
	_active = true;
}

void ng::Animation::stop()
{
	_active = false;
}

const ng::ImageSampleData& ng::Animation::getNextFrame(float deltaTime)
{
	if(_active)
	{
		_timeSinceCurrentFrame += deltaTime;

		while(_timeSinceCurrentFrame >= _frames[_currentFrameIdx].duration)
		{
			if(_currentFrameIdx < (_frames.size() - 1))
			{
				_timeSinceCurrentFrame -= _frames[_currentFrameIdx].duration;
				_currentFrameIdx++;
			}
			else if(_loop)
			{
				_timeSinceCurrentFrame -= _frames[_currentFrameIdx].duration;
				_currentFrameIdx = 0;
			}
			else
			{
				_active = false;
				_onEnd();
				break;
			}
		}
	}

	return _frames[_currentFrameIdx].sampleData;
}

std::size_t ng::AnimationGroup::_HeteroStringHash::operator()(const std::string& key) const noexcept
{
	return std::hash<std::string> {} (key);
}

std::size_t ng::AnimationGroup::_HeteroStringHash::operator()(std::string_view key) const noexcept
{
	return std::hash<std::string_view> {} (key);
}

bool ng::AnimationGroup::_HeteroStringEquals::operator()(const std::string& lhs, const std::string& rhs) const noexcept
{
	return lhs == rhs;
}

bool ng::AnimationGroup::_HeteroStringEquals::operator()(std::string_view lhs, std::string_view rhs) const noexcept
{
	return lhs == rhs;
}

ng::AnimationGroup::AnimationGroup(size_t maxAnimations)
{
	_animations.reserve(maxAnimations);
}

ng::Animation& ng::AnimationGroup::addAnimation(std::string_view name, size_t maxFrames)
{
	if(_animations.size() == _animations.capacity())
	{
		throw std::out_of_range {"Maximum number of animations cannot be exceeded."};
	}

	Animation& ret {_animations.emplace_back(maxFrames)};
	_nameToAnimation.try_emplace(std::string {name}, &ret);

	return ret;
}

ng::Animation& ng::AnimationGroup::getAnimation(std::string_view name)
{
	auto nameAndAnimation {_nameToAnimation.find(name)};

	if(nameAndAnimation == _nameToAnimation.end())
	{
		throw std::out_of_range {"This animation name does not exist."};
	}

	return *nameAndAnimation->second;
}

void ng::AnimationGroup::setActiveAnimation(std::string_view name)
{
	_activeAnimation = &getAnimation(name);
}

ng::Animation& ng::AnimationGroup::activeAnimation()
{
	if(_activeAnimation == nullptr)
	{
		throw std::runtime_error {"No active animation was set."};
	}

	return *_activeAnimation;
}