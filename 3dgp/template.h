#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

template <typename T>
class Singleton
{
public:

	static T* getInstance() {
		static T instance;
		return &instance;
	};

};


// TODO : Overload float's operator == and !=
//template<typename T>
//bool operator==(const T& a_leftValue,const T& a_rightValue) const
//{
//	if (fabsf(a_leftValue - a_rightValue) < FLT_EPSILON)
//	{
//		return true;
//	} 
//	else
//	{
//		return false;
//	}
//}
//
//template<typename T>
//bool operator!=(const T& a_leftValue, const T& a_rightValue) const
//{
//	if (fabsf(a_leftValue - a_rightValue) > FLT_EPSILON)
//	{
//		return true;
//	} 
//	else
//	{
//		return false;
//	}
//}



#endif // !_TEMPLATE_H_
