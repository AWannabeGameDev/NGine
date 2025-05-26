#ifndef NG_ANIMATION_HPP
#define NG_ANIMATION_HPP

#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <deque>

#include "ngine/renderer/renderer_data.hpp"

namespace ng
{

class Animation
{
private :
	struct _Frame
	{
		ImageSampleData sampleData;
		float duration;
	};

	std::vector<_Frame> _frames {};
	std::function<void()> _onEnd {[] () {}};
	bool _loop {false};
	
	size_t _currentFrameIdx {0};
	float _timeSinceCurrentFrame {0.0f};
	bool _active {false};

public:
	Animation(size_t maxFrames);

	void addFrame(const ImageSampleData& frame, float duration);

	/**
	 * Use caution when setting this to a reference/pointer capturing lambda, because then
	 * the safety of calling the on-end callback depends on the lifetime of the referred object
	 * (the referred object must live at least until the lifetime of the animation).
	 */
	void setOnEnd(const std::function<void()>& onEndFunction);

	void setLoop(bool loop);

	void start(size_t initialFrame);
	void resume();
	void stop();
	const ImageSampleData& getNextFrame(float deltaTime);
};

class AnimationGroup
{
private :
	struct _HeteroStringHash
	{
		// enable heterogeneous lookup
		using is_transparent = void;

		std::size_t operator()(const std::string& key) const noexcept;
		std::size_t operator()(std::string_view key) const noexcept;
	};

	struct _HeteroStringEquals
	{
		// enable heterogeneous lookup
		using is_transparent = void;

		bool operator()(const std::string& lhs, const std::string& rhs) const noexcept;
		bool operator()(std::string_view lhs, std::string_view rhs) const noexcept;
	};

	std::vector<Animation> _animations {};
	std::unordered_map<std::string, Animation*, _HeteroStringHash, _HeteroStringEquals> _nameToAnimation {};

	Animation* _activeAnimation {nullptr};

public :
	AnimationGroup(size_t maxAnimations);

	// These require special handling for _activeAnimation and _nameToAnimation, will do in the future
	AnimationGroup(const AnimationGroup& other) = delete;
	const AnimationGroup& operator=(const AnimationGroup& other) = delete;

	Animation& addAnimation(std::string_view name, size_t maxFrames);
	Animation& getAnimation(std::string_view name);

	void setActiveAnimation(std::string_view name);
	Animation& activeAnimation();
};

}

#endif