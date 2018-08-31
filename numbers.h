#ifndef _NUMBERS_H_
#define _NUMBERS_H_

#define DIGIT_MAX (6)

class Numbers
{
private:
	SPRITE_DATA m_sprData;

public:
	virtual void memberCopy(const Numbers& a_inputObj);
	//Numbers();
	Numbers(const Numbers& a_inputObj);
	virtual ~Numbers();
	const Numbers& Numbers::operator=(const Numbers& a_right);

	struct DIGIT
	{
		BYTE	val;	//値
		int		x, y;	//表示位置
		float	w, h;
	}digit[DIGIT_MAX];

	Numbers(SPRITE_DATA* a_fontData = nullptr);

	Vector3 m_pos;
	Vector3 m_speed;
	Vector3 m_speedAcc;
	Vector3 m_speedMax;

	CUSTOM m_custom;

	int m_timer;
	int m_step;
	int m_alpha;

	int m_value;
	int m_digitNum;
	bool m_isVisibleAlways;
	bool m_isVisible;

	void setValue(int a_value, Vector3 a_size = Vector3(1.0f, 1.0f, 0.0f));

	virtual void clear();
	virtual void update();

	virtual void draw();

};


#endif // !_NUMBERS_H_
