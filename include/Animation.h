#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{
public:
	Animation();

	void update(float deltaTime);

	int getCurrentFrame() const;

	void setFrameCount(int frameCount);
	void setFrameDuration(float duration);

	void reset();

private:
	int _currentFrame;
	int _frameCount;

	float _timer;
	float _frameDuration;
};

#endif // !ANIMATION_H
