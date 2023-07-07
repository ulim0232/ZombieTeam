#pragma once

enum class SceneId
{
	None = -1,
	Dev1,
	Count
};

enum class ResourceTypes
{
	Texture,
	Font,
	SoundBuffer
};

enum class Origins
{
	TL,
	TC,
	TR,
	ML,
	MC,
	MR,
	BL,
	BC,
	BR,
	CUSTOM //임의의 origin을 가지는 것들
};