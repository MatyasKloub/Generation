#pragma once
class Chunk
{
public:
	bool loaded = false;
	int startX;
	int endX;
	int startY;
	int endY;

	Chunk() : loaded(false), startX(0), endX(), startY(0), endY(0) {}

	Chunk(int startX, int endX, int startY, int endY)
        : loaded(false), startX(startX), endX(endX), startY(startY), endY(endY) {}

	
};

