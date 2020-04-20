#source("C:\\Users\\mirab\\Documents\\papers\\ccipd_code_newPath\\trunk\\R\\Mirabela\\atlas\\EstablishStatSign.R")

tab_newRat = read.table("C:\\Mirabela\\Projects\\AtlasComparison\\Results\\VolumeComparison.txt", header=TRUE, sep = ",")
disSbj = read.table("C:\\Mirabela\\Projects\\AtlasComparison\\Results\\DiseaseSubjects.txt",sep = " ")$V4
norSbj = read.table("C:\\Mirabela\\Projects\\AtlasComparison\\Results\\NormalSubjects.txt",sep = " ")$V4

disSbj2  = seq(1,length(disSbj))
for (i in 1:length(disSbj))
{
	l = nchar(paste(disSbj[i]))
	if (substr(disSbj[i],1,5)!="STUDY")
		disSbj2[i] = paste(substr(disSbj[i],l-3,l))
	else
		disSbj2[i] = paste(disSbj[i])

}
disSbj = sort(disSbj2)

norSbj2  = seq(1,length(norSbj))
for (i in 1:length(norSbj))
{
	l = nchar(paste(norSbj[i]))
	if (substr(norSbj[i],1,5)!="STUDY")
		norSbj2[i] = paste(substr(norSbj[i],l-3,l))
	else
		norSbj2[i] = paste(norSbj[i])

}
norSbj = sort(norSbj2)

disStatus  = rep(0,2*length(norSbj)) # normal = 0 ; disease = 1

for (i in 1:dim(tab_newRat)[1])
{
	l = nchar(paste(tab_newRat[i,1]))
	if (substr(tab_newRat[i,1],1,5)=="STUDY")
		caseID = paste(substr(tab_newRat[i,1],1,5),'-',substr(tab_newRat[i,1],7,9),sep='')
	else
		caseID = substr(tab_newRat[i,1],1,4)
		
	found = 0
	for (j in 1:length(disSbj))
	{
		if (caseID==disSbj[j])
			found  = 1
		#print (paste("  ",i, caseID,j,disSbj[j]))
	}
	if (found ==1)
		disStatus[i] = 1
	else
		disStatus[i] = 0
}

disTab = tab_newRat[disStatus==1,]
normTab = tab_newRat[disStatus==0,]

print(dim(distTab))

prIdx = seq(2,70,2)
cgIdx = seq(1,70,2)

prIdx1 = seq(2,22,2)
cgIdx1 = seq(1,22,2)

prIdx2 = seq(24,40,2)
cgIdx2 = seq(23,40,2)

prIdx3 = seq(42,70,2)
cgIdx3 = seq(41,70,2)

pfx = c("Al","UH","UC","SV")

i = 1
tabDR1 = disTab[prIdx,3]
tabDR2 = disTab[prIdx,4]
tabNR1 = normTab[prIdx,3]
tabNR2 = normTab[prIdx,4]

wt = wilcox.test(tabDR1,tabNR1)
print(paste(pfx[i],"PR1",round(mean(tabDR1)/1000,1),round(sd(tabDR1)/1000,1),round(mean(tabNR1)/1000,1),round(sd(tabNR1)/1000,1), round(wt$p.value,3)))
wt = wilcox.test(tabDR2,tabNR2)
print(paste(pfx[i],"PR2",round(mean(tabDR2)/1000,1),round(sd(tabDR2)/1000,1),round(mean(tabNR2)/1000,1),round(sd(tabNR2)/1000,1), round(wt$p.value,3)))
wtd = wilcox.test(tabDR1,tabDR2,Paired=TRUE)
wtn = wilcox.test(tabNR1,tabNR2,Paired=TRUE)
print(paste(pfx[i],"IR", round(wtd$p.value,3),round(wtn$p.value,3)))

tabDR1 = disTab[cgIdx,3]
tabDR2 = disTab[cgIdx,4]
tabNR1 = normTab[cgIdx,3]
tabNR2 = normTab[cgIdx,4]

wt = wilcox.test(tabDR1,tabNR1)
print(paste(pfx[i],"CG1",round(mean(tabDR1)/1000,1),round(sd(tabDR1)/1000,1),round(mean(tabNR1)/1000,1),round(sd(tabNR1)/1000,1), round(wt$p.value,3)))
wt = wilcox.test(tabDR2,tabNR2)
print(paste(pfx[i],"CG2",round(mean(tabDR2)/1000,1),round(sd(tabDR2)/1000,1),round(mean(tabNR2)/1000,1),round(sd(tabNR2)/1000,1), round(wt$p.value,3)))
wtd = wilcox.test(tabDR1,tabDR2,Paired=TRUE)
wtn = wilcox.test(tabNR1,tabNR2,Paired=TRUE)
print(paste(pfx[i],"IR", round(wtd$p.value,3),round(wtn$p.value,3)))

i = 2
prIdx = prIdx1
cgIdx = cgIdx1


tabDR1 = disTab[prIdx,3]
tabDR2 = disTab[prIdx,4]
tabNR1 = normTab[prIdx,3]
tabNR2 = normTab[prIdx,4]

wt = wilcox.test(tabDR1,tabNR1)
print(paste(pfx[i],"PR1",round(mean(tabDR1)/1000,1),round(sd(tabDR1)/1000,1),round(mean(tabNR1)/1000,1),round(sd(tabNR1)/1000,1), round(wt$p.value,3)))
wt = wilcox.test(tabDR2,tabNR2)
print(paste(pfx[i],"PR2",round(mean(tabDR2)/1000,1),round(sd(tabDR2)/1000,1),round(mean(tabNR2)/1000,1),round(sd(tabNR2)/1000,1), round(wt$p.value,3)))
wtd = wilcox.test(tabDR1,tabDR2,Paired=TRUE)
wtn = wilcox.test(tabNR1,tabNR2,Paired=TRUE)
print(paste(pfx[i],"IR", round(wtd$p.value,3),round(wtn$p.value,3)))

tabDR1 = disTab[cgIdx,3]
tabDR2 = disTab[cgIdx,4]
tabNR1 = normTab[cgIdx,3]
tabNR2 = normTab[cgIdx,4]

wt = wilcox.test(tabDR1,tabNR1)
print(paste(pfx[i],"CG1",round(mean(tabDR1)/1000,1),round(sd(tabDR1)/1000,1),round(mean(tabNR1)/1000,1),round(sd(tabNR1)/1000,1), round(wt$p.value,3)))
wt = wilcox.test(tabDR2,tabNR2)
print(paste(pfx[i],"CG2",round(mean(tabDR2)/1000,1),round(sd(tabDR2)/1000,1),round(mean(tabNR2)/1000,1),round(sd(tabNR2)/1000,1), round(wt$p.value,3)))
wtd = wilcox.test(tabDR1,tabDR2,Paired=TRUE)
wtn = wilcox.test(tabNR1,tabNR2,Paired=TRUE)
print(paste(pfx[i],"IR", round(wtd$p.value,3),round(wtn$p.value,3)))


i = 3
prIdx = prIdx2
cgIdx = cgIdx2


tabDR1 = disTab[prIdx,3]
tabDR2 = disTab[prIdx,4]
tabNR1 = normTab[prIdx,3]
tabNR2 = normTab[prIdx,4]

wt = wilcox.test(tabDR1,tabNR1)
print(paste(pfx[i],"PR1",round(mean(tabDR1)/1000,1),round(sd(tabDR1)/1000,1),round(mean(tabNR1)/1000,1),round(sd(tabNR1)/1000,1), round(wt$p.value,3)))
wt = wilcox.test(tabDR2,tabNR2)
print(paste(pfx[i],"PR2",round(mean(tabDR2)/1000,1),round(sd(tabDR2)/1000,1),round(mean(tabNR2)/1000,1),round(sd(tabNR2)/1000,1), round(wt$p.value,3)))
wtd = wilcox.test(tabDR1,tabDR2,Paired=TRUE)
wtn = wilcox.test(tabNR1,tabNR2,Paired=TRUE)
print(paste(pfx[i],"IR", round(wtd$p.value,3),round(wtn$p.value,3)))

tabDR1 = disTab[cgIdx,3]
tabDR2 = disTab[cgIdx,4]
tabNR1 = normTab[cgIdx,3]
tabNR2 = normTab[cgIdx,4]

wt = wilcox.test(tabDR1,tabNR1)
print(paste(pfx[i],"CG1",round(mean(tabDR1)/1000,1),round(sd(tabDR1)/1000,1),round(mean(tabNR1)/1000,1),round(sd(tabNR1)/1000,1), round(wt$p.value,3)))
wt = wilcox.test(tabDR2,tabNR2)
print(paste(pfx[i],"CG2",round(mean(tabDR2)/1000,1),round(sd(tabDR2)/1000,1),round(mean(tabNR2)/1000,1),round(sd(tabNR2)/1000,1), round(wt$p.value,3)))
wtd = wilcox.test(tabDR1,tabDR2,Paired=TRUE)
wtn = wilcox.test(tabNR1,tabNR2,Paired=TRUE)
print(paste(pfx[i],"IR", round(wtd$p.value,3),round(wtn$p.value,3)))



i = 4
prIdx = prIdx3
cgIdx = cgIdx3


tabDR1 = disTab[prIdx,3]
tabDR2 = disTab[prIdx,4]
tabNR1 = normTab[prIdx,3]
tabNR2 = normTab[prIdx,4]

wt = wilcox.test(tabDR1,tabNR1)
print(paste(pfx[i],"PR1",round(mean(tabDR1)/1000,1),round(sd(tabDR1)/1000,1),round(mean(tabNR1)/1000,1),round(sd(tabNR1)/1000,1), round(wt$p.value,3)))
wt = wilcox.test(tabDR2,tabNR2)
print(paste(pfx[i],"PR2",round(mean(tabDR2)/1000,1),round(sd(tabDR2)/1000,1),round(mean(tabNR2)/1000,1),round(sd(tabNR2)/1000,1), round(wt$p.value,3)))
wtd = wilcox.test(tabDR1,tabDR2,Paired=TRUE)
wtn = wilcox.test(tabNR1,tabNR2,Paired=TRUE)
print(paste(pfx[i],"IR", round(wtd$p.value,3),round(wtn$p.value,3)))

tabDR1 = disTab[cgIdx,3]
tabDR2 = disTab[cgIdx,4]
tabNR1 = normTab[cgIdx,3]
tabNR2 = normTab[cgIdx,4]

wt = wilcox.test(tabDR1,tabNR1)
print(paste(pfx[i],"CG1",round(mean(tabDR1)/1000,1),round(sd(tabDR1)/1000,1),round(mean(tabNR1)/1000,1),round(sd(tabNR1)/1000,1), round(wt$p.value,3)))
wt = wilcox.test(tabDR2,tabNR2)
print(paste(pfx[i],"CG2",round(mean(tabDR2)/1000,1),round(sd(tabDR2)/1000,1),round(mean(tabNR2)/1000,1),round(sd(tabNR2)/1000,1), round(wt$p.value,3)))

wtd = wilcox.test(tabDR1,tabDR2,Paired=TRUE)
wtn = wilcox.test(tabNR1,tabNR2,Paired=TRUE)
print(paste(pfx[i],"IR", round(wtd$p.value,3),round(wtn$p.value,3)))



# [1] 70  7
# [1] "Al PR1 42.1 18.5 60.6 32 0.013"
# [1] "Al PR2 41.4 18.3 60 33.4 0.018"
# [1] "Al CG1 17.8 12.7 34.4 24.6 0"
# [1] "Al CG2 18 13.2 34.7 26.7 0.001"
# [1] "UH PR1 40.1 21 32.6 14.6 0.438"
# [1] "UH PR2 39.2 20.6 29.9 12.1 0.3"
# [1] "UH CG1 20.2 17 16.6 9.5 0.949"
# [1] "UH CG2 20 17.1 15.2 9.8 0.699"
# [1] "UC PR1 41.1 13.5 79.5 32.9 0.001"
# [1] "UC PR2 40.9 13.6 81.5 35.3 0.001"
# [1] "UC CG1 15.9 6.5 48 29.2 0.001"
# [1] "UC CG2 16 6.5 50.7 33.9 0.001"
# [1] "SV PR1 44.2 20.2 69.8 27.9 0.01"
# [1] "SV PR2 43.3 20 69.2 28.1 0.008"
# [1] "SV CG1 17.2 12.5 39.2 22.9 0.002"
# [1] "SV CG2 17.8 13.7 39.3 22.9 0.002"






prIdx = seq(2,140,2)
cgIdx = seq(1,140,2)


i = 1
tabDR1 = tab_newRat[prIdx,6]
tabNR1 = tab_newRat[cgIdx,6]

print(paste(pfx[i],"PR1",round(mean(tabDR1),2),round(sd(tabDR1),3)))
print(paste(pfx[i],"CG1",round(mean(tabNR1),2),round(sd(tabNR1),3)))



tab_newRat = read.table("C:\\Mirabela\\Projects\\AtlasComparison\\Results\\VolumeComparison.txt", header=TRUE, sep = ",")

prIdx = seq(2,140,2)
cgIdx = seq(1,140,2)

PrKappa = tab_newRat[prIdx,8]
CGKappa = tab_newRat[cgIdx,8]

print(paste('Pr',mean(PrKappa),sd(PrKappa)))
print(paste('CG',mean(CGKappa),sd(CGKappa)))

#[1] "Pr 0.980212188803229 0.0310841975390057"
#[1] "CG 0.972065827684171 0.0548085862252105"

