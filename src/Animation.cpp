#include "include/Animation.h"

Animation::Animation()
	: _currentFrame(0),
	  _frameCount(1),
	  _timer(0.0f),
	  _frameDuration(0.1f)
{
}

void Animation::update(float deltaTime)
{
	_timer += deltaTime;

	if (_timer >= _frameDuration)
	{
		_timer -= _frameDuration;

		_currentFrame++;

		if (_currentFrame >= _frameCount)
			_currentFrame = 0;
	}
}

int Animation::getCurrentFrame() const
{
	return _currentFrame;
}

void Animation::setFrameCount(int frameCount)
{
	_frameCount = frameCount;

	if (_currentFrame >= _frameCount)
		_currentFrame = 0;
}

void Animation::setFrameDuration(float duration)
{
	_frameDuration = duration;
}

void Animation::reset()
{
	_currentFrame = 0;
	_timer = 0.0f;
}