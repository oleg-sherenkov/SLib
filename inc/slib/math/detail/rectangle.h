#ifndef CHECKHEADER_SLIB_MATH_DETAIL_RECTANGLE
#define CHECKHEADER_SLIB_MATH_DETAIL_RECTANGLE

#include "../rectangle.h"

namespace slib
{

	template <class T, class FT>
	SLIB_INLINE const RectangleT<T, FT>& RectangleT<T, FT>::zero()
	{
		return *(reinterpret_cast<RectangleT<T, FT> const*>(&_zero));
	}

	template <class T, class FT>
	SLIB_INLINE T RectangleT<T, FT>::getWidth() const
	{
		return right - left;
	}
	
	template <class T, class FT>
	SLIB_INLINE T RectangleT<T, FT>::getHeight() const
	{
		return bottom - top;
	}
	
	template <class T, class FT>
	SLIB_INLINE SizeT<T, FT> RectangleT<T, FT>::getSize() const
	{
		return {right - left, bottom - top};
	}
	
	template <class T, class FT>
	template <class O, class FO>
	SLIB_INLINE RectangleT<T, FT>& RectangleT<T, FT>::operator=(const RectangleT<O, FO>& other)
	{
		left = (T)(other.left);
		top = (T)(other.top);
		right = (T)(other.right);
		bottom = (T)(other.bottom);
		return *this;
	}
	
	
	template <class T, class FT>
	SLIB_INLINE RectangleT<T, FT> Interpolation< RectangleT<T, FT> >::interpolate(const RectangleT<T, FT>& a, const RectangleT<T, FT>& b, float factor)
	{
		return a.lerp(b, factor);
	}

}

#endif
