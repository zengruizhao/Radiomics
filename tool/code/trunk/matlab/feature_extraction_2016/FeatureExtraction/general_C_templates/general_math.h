
namespace genmath 
{

template <class T> inline T min(const T &x1, const T &x2)
{
   if(x1<x2)
      return x1;
   else
      return x2;
}

template <class T> inline T max(const T &x1, const T &x2)
{
   if(x1>x2)
      return x1;
   else
      return x2;
}

template <class T> inline T floor(const T &x)
{
	if((int)x == x) 
		return x;

   if(x<0)
      return (T) ((int) (x - 1));
   else
      return (T) ((int) x);
}

template <class T> inline T ceil(const T &x)
{
	if((int)x == x) 
		return x;

	if(x<0)
      return (T) ((int) x);
   else
      return (T) ((int) (x + 1));
}

template <class T> inline T clamp(const T &x, const T &lower, const T &upper)
{
   if(x>upper)
      return upper;
   else if(x<lower)
      return lower;
	else
		return x;
}

}
