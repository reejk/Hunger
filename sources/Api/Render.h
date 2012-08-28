#ifndef RENDER_H
#define RENDER_H

class World;

class Render
{
public:
	class Painter
	{
	public:
		virtual ~Painter() = 0;
		virtual void setColor(char r, char g, char b, char a = 0xFF) = 0;
		virtual void setLineWidth(int width) = 0;
		virtual void pixel(float x, float y, float z = 99.0f) const = 0;
		virtual void line(float fromX, float fromY, float toX, float toY, float z = 99.0f) const = 0;
		virtual void rect(float fromX, float fromY, float toX, float toY, float z = 99.0f, bool fill = true) const = 0;
		virtual void rectx(float fromX, float fromY, float toX, float toY, float z = 99.0f,
				  float fromTexX = 0, float fromTexY = 0, float toTexX = 1, float toTexY = 1) const = 0;
	};

	virtual ~Render() = 0;

	virtual void stopRender() = 0;
	virtual void resumeRender() = 0;
	virtual void setResolution(int width, int height) = 0;
	virtual void render() const = 0;

	virtual const World * getWorld() const = 0;
	virtual void setWorld(const World *world) = 0;

	static Painter * getPainter() {
		return _Painter;
	}

protected:
	static Painter *_Painter;
};

#endif // RENDER_H
