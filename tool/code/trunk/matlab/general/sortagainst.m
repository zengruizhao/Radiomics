function [reordered1,reordered2]=sortagainst(list1,list2)

[reordered1,neworder]=sort(list1);
reordered2=list2(neworder);
