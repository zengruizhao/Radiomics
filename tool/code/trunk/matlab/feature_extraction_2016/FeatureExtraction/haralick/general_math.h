
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

}
