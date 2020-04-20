t=read.table('C:\\Mirabela\\Projects\\AtlasComparison\\Results\\VolumeComparison.txt',sep=',', header = TRUE)


c1 = seq(1,44)
c2 = seq(45,80)
c3 = seq(81,140)
c(mean(t$DicePy),mean(t$DicePy[c1]),mean(t$DicePy[c2]),mean(t$DicePy[c3]))
%[1] 0.9763559 0.9529150 0.9858019 0.9878783

c(sd(t$DicePy),sd(t$DicePy[c1]),sd(t$DicePy[c2]),sd(t$DicePy[c3]))
%[1] 0.04432585 0.06383523 0.02458997 0.02647802

c(mean(t$haussdorffDist),mean(t$haussdorffDist[c1]),mean(t$haussdorffDist[c2]),mean(t$haussdorffDist[c3]))
c(sd(t$haussdorffDist),sd(t$haussdorffDist[c1]),sd(t$haussdorffDist[c2]),sd(t$haussdorffDist[c3]))

c(mean(t$haussdorffDist),mean(t$haussdorffDist[c1]),mean(t$haussdorffDist[c2]),mean(t$haussdorffDist[c3]))
%[1] 3.683942 5.694090 3.348904 2.410857
c(sd(t$haussdorffDist),sd(t$haussdorffDist[c1]),sd(t$haussdorffDist[c2]),sd(t$haussdorffDist[c3]))
%[1] 4.356192 4.588686 5.489930 2.598430


 wilcox.test(t$volOld, t$volNew, paired=TRUE)

        Wilcoxon signed rank test with continuity correction

data:  t$volOld and t$volNew
V = 2857, p-value = 0.002512
alternative hypothesis: true location shift is not equal to 0


 t.test(t$volOld, t$volNew, paired=TRUE)

        Paired t-test

data:  t$volOld and t$volNew
t = 0.7564, df = 139, p-value = 0.4507
alternative hypothesis: true difference in means is not equal to 0
95 percent confidence interval:
 -319.8228  716.1489
sample estimates:
mean of the differences 
               198.1631 

wilcox.test(t$volOld[c1], t$volNew[c1], paired=TRUE)

        Wilcoxon signed rank test with continuity correction

data:  t$volOld[c1] and t$volNew[c1]
V = 655, p-value = 0.0037
alternative hypothesis: true location shift is not equal to 0

Warning message:
In wilcox.test.default(t$volOld[c1], t$volNew[c1], paired = TRUE) :
  cannot compute exact p-value with zeroes


wilcox.test(t$volOld[c2], t$volNew[c2], paired=TRUE)

        Wilcoxon signed rank test with continuity correction

data:  t$volOld[c2] and t$volNew[c2]
V = 70, p-value = 0.7764
alternative hypothesis: true location shift is not equal to 0

Warning message:
In wilcox.test.default(t$volOld[c2], t$volNew[c2], paired = TRUE) :
  cannot compute exact p-value with zeroes


wilcox.test(t$volOld[c3], t$volNew[c3], paired=TRUE)

        Wilcoxon signed rank test with continuity correction

data:  t$volOld[c3] and t$volNew[c3]
V = 392, p-value = 0.04733
alternative hypothesis: true location shift is not equal to 0

Warning message:
In wilcox.test.default(t$volOld[c3], t$volNew[c3], paired = TRUE) :
  cannot compute exact p-value with zeroes
 


t=read.table('C:\\Mirabela\\Projects\\AtlasComparison\\Results\\VolumeComparisonRevised.txt',sep=',', header = TRUE)


c1 = seq(1,44)
c2 = seq(45,80)
c3 = seq(81,140)
c(mean(t$DicePy),mean(t$DicePy[c1]),mean(t$DicePy[c2]),mean(t$DicePy[c3]))
#[1] 0.9763559 0.9529150 0.9858019 0.9878783
#0.9613781 0.9419318 0.9682609 0.9715089

c(sd(t$DicePy),sd(t$DicePy[c1]),sd(t$DicePy[c2]),sd(t$DicePy[c3]))
#[1] 0.04432585 0.06383523 0.02458997 0.02647802
#[1] 0.05467223 0.06117847 0.02719351 0.05882368


c(mean(t$haussdorffDist),mean(t$haussdorffDist[c1]),mean(t$haussdorffDist[c2]),mean(t$haussdorffDist[c3]))
#[1] 3.683942 5.694090 3.348904 2.410857
#5.276367 5.639166 5.901720 4.635102

c(sd(t$haussdorffDist),sd(t$haussdorffDist[c1]),sd(t$haussdorffDist[c2]),sd(t$haussdorffDist[c3]))
#[1] 4.356192 4.588686 5.489930 2.59843
#3.361628 2.635387 4.943956 2.511303