#ifndef _NUMBERS_H_
#define _NUMBERS_H_

#define DIGIT_MAX (6)

class Numbers
{
private:
	SPRITE_DATA sprData;

public:
	virtual void memberCopy(const Numbers& inputObj);
	//Numbers();
	Numbers(const Numbers& inputObj);
	virtual ~Numbers();
	const Numbers& Numbers::operator=(const Numbers& right);

	struct DIGIT
	{
		BYTE	val;	//値
		int		x, y;	//表示位置
		float	w, h;
	}digit[DIGIT_MAX];

	Numbers(SPRITE_DATA* fontData = nullptr);

	Vector3 pos;
	Vector3 speed;
	Vector3 speedAcc;
	Vector3 speedMax;

	Transform2D transform2D;

	int timer;
	int step;
	int alpha;

	int value;
	int digitNum;
	bool isVisibleAlways;
	bool isVisible;

	void setValue(int value, Vector3 size = Vector3(1.0f, 1.0f, 0.0f));

	virtual void clear();
	virtual void update();

	virtual void draw();

};


#endif // !_NUMBERS_H_
