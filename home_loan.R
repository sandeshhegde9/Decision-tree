library(party)
library(tree)
input.dat<-input
a<-ctree_control(mincriterion = 0.05,minbucket = 1,minsplit = 1)
detree.tree<-ctree(Defaulted_income~Home_owner+Marital_status+Annual_income,data=input.dat,controls=a)
print(detree.tree)
plot(detree.tree)

